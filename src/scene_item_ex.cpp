/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include "scene.h"
#include "scene_item.h"
#include "scene_item_ex.h"
#include "game_map.h"
#include "game_party.h"
#include "game_player.h"
#include "game_strings.h"
#include "game_switches.h"
#include "game_system.h"
#include "game_targets.h"
#include "input.h"
#include <lcf/reader_util.h>
#include "scene_actortarget.h"
#include "scene_map.h"
#include "scene_teleport.h"
#include "output.h"
#include "transition.h"

Scene_Item_Ex::Scene_Item_Ex(int category_index, int item_index) :
		category_index(category_index), item_index(item_index) {
	Scene::type = Scene::Item;

	item_windows.reserve(8);
}

void Scene_Item_Ex::Start() {
	int menu_help_height = 32;
	int menu_categories_height = 32;

	help_window.reset(new Window_Help(Player::menu_offset_x, Player::menu_offset_y, MENU_WIDTH, menu_help_height));
	category_window.reset(new Window_Help(Player::menu_offset_x, Player::menu_offset_y + MENU_HEIGHT - menu_categories_height, MENU_WIDTH, menu_categories_height));

	Main_Data::game_party->GetMinMaxItemCategories(this->category_min, this->category_max);
	if (category_min == 0 && !Main_Data::game_party->HasItemCategoryInInventory(0))
		category_min = 1;
	int first_category_index_with_items = category_min;
	for (int i = category_min; i < category_max; i++) {
		if (Main_Data::game_party->HasItemCategoryInInventory(i)) {
			first_category_index_with_items = i;
			break;
		}
	}
	category_index = std::max(category_index, first_category_index_with_items);

	for (int i = 0; i <= category_max; i++) {
		item_windows.push_back(new Window_Item(
			Player::menu_offset_x,
			Player::menu_offset_y + menu_help_height,
			MENU_WIDTH,
			MENU_HEIGHT - menu_help_height - menu_categories_height));
		item_windows[i]->SetHelpWindow(help_window.get());
		item_windows[i]->SetIndex(item_index);
		item_windows[i]->SetCategoryIndex(i);
		item_windows[i]->Refresh();

		item_windows[i]->SetActive(category_index == i);
		item_windows[i]->SetVisible(category_index == i);
	}

	UpdateCategoryWindow();
	UpdateArrows();
}

void Scene_Item_Ex::UpdateCategoryWindow() {
	category_window->Clear();
	category_window->AddText(" ");

	constexpr int offset_category_strings = 20;
	for (int i = category_min; i <= category_max; i++) {

		if (i == category_index) {
			category_window->AddText(Main_Data::game_strings->Get(offset_category_strings + i).to_string(), Font::ColorCritical);
		}
		else {
			category_window->AddText(Main_Data::game_strings->Get(offset_category_strings + i).to_string());
		}
		category_window->AddText("   ");
	}
}

void Scene_Item_Ex::Continue(SceneType /* prev_scene */) {
	for (int i = category_min; i <= category_max; i++) {
		item_windows[i]->Refresh();
	}
}

void Scene_Item_Ex::vUpdate() {
	help_window->Update();
	category_window->Update();

	Window_Item* active_item_window = nullptr;

	if (category_index >= category_min && category_index <= category_max /* && !category_window->GetActive() */ ) {
		active_item_window = item_windows[category_index];
	}

	bool window_changed = false;
	bool reactivate_window = false;
	if (active_item_window) {
		if (Input::IsRepeated(Input::LEFT)) {
			int index = active_item_window->GetIndex();
			if ((index % active_item_window->GetColumnMax()) == 0) {
				category_index--;
				window_changed = true;
				if (category_index < category_min) {
					category_index = category_min;
					window_changed = false;
				}
				active_item_window->SetActive(false);
				active_item_window->SetVisible(false);
				active_item_window = item_windows[category_index];
				active_item_window->SetActive(false);
				active_item_window->SetVisible(true);
				reactivate_window = true;
				if (window_changed) {
					index += active_item_window->GetColumnMax() - 1;
					active_item_window->SetIndex(index);
				}
			}
		}
		else if (Input::IsRepeated(Input::RIGHT)) {
			int index = active_item_window->GetIndex();
			if ((index +1) == active_item_window->GetItemMax() || ((index - 1) % active_item_window->GetColumnMax()) == 0) {
				category_index++;
				window_changed = true;
				if (category_index > category_max) {
					category_index = category_max;
					window_changed = false;
				}
				active_item_window->SetActive(false);
				active_item_window->SetVisible(false);
				active_item_window = item_windows[category_index];
				active_item_window->SetActive(false);
				active_item_window->SetVisible(true);
				reactivate_window = true;
				if (window_changed) {
					index -= (active_item_window->GetColumnMax() - 1);
					if (index < 0)
						index = 0;
					active_item_window->SetIndex(index);
				}
			}
		}

		if (window_changed) {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cursor));
			UpdateCategoryWindow();
		}
	}

	for (int i = category_min; i <= category_max; i++) {
		item_windows[i]->Update();
	}

	if (reactivate_window) {
		active_item_window->SetActive(true);
	}

	if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));
		Scene::Pop();
	}
	else if (Input::IsTriggered(Input::DECISION) && active_item_window) {
		int item_id = active_item_window->GetItem() == NULL ? 0 : active_item_window->GetItem()->ID;

		// The party only has valid items
		if (item_id > 0 && active_item_window->CheckEnable(item_id)) {
			active_item_window->UseItem();
			item_index = active_item_window->GetIndex();
		}
		else {
			Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Buzzer));
		}
	}

	UpdateArrows();
}

void Scene_Item_Ex::UpdateArrows() {
	constexpr int arrow_animation_frames = 20;

	bool show_left_arrow = (category_index > category_min);
	bool show_right_arrow = (category_index < category_max);

	if (show_left_arrow || show_right_arrow) {
		arrow_frame = (arrow_frame + 1) % (arrow_animation_frames * 2);
	}
	bool arrow_visible = (arrow_frame < arrow_animation_frames);
	category_window->SetLeftArrow(show_left_arrow && arrow_visible);
	category_window->SetRightArrow(show_right_arrow && arrow_visible);
}

void Scene_Item_Ex::TransitionOut(Scene::SceneType next_scene) {
	const lcf::rpg::Item* item = nullptr;
	const lcf::rpg::Skill* skill = nullptr;

	if (category_index >= category_min && category_index <= category_max) {
		item = item_windows[category_index]->GetItem();
	}
	if (item && item->type == lcf::rpg::Item::Type_special && item->skill_id > 0) {
		skill = lcf::ReaderUtil::GetElement(lcf::Data::skills, item->skill_id);
	}

	if (next_scene == Map && skill && skill->type == lcf::rpg::Skill::Type_escape) {
		Transition::instance().InitErase(Transition::TransitionFadeOut, this);
	}
	else {
		Scene::TransitionOut(next_scene);
	}
}
