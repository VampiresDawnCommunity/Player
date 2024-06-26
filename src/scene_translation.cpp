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
#include "scene_translation.h"
#include "audio.h"
#include "bitmap.h"
#include "input.h"
#include "game_system.h"
#include "cache.h"
#include "input_buttons.h"
#include "input_source.h"
#include "keys.h"
#include "main_data.h"
#include "options.h"
#include "player.h"
#include "baseui.h"
#include "output.h"
#include "utils.h"
#include "scene_end.h"
#include "window_about.h"
#include "window_command_horizontal.h"
#include "window_help.h"
#include "window_input_settings.h"
#include "window_numberinput.h"
#include "window_selectable.h"
#include "window_settings.h"
#include <memory>

#ifdef EMSCRIPTEN
#  include <emscripten.h>
#endif

Scene_Translation::Scene_Translation() {
	Scene::type = Scene::Translation;
}

void Scene_Translation::CreateTitleGraphic() {
	// Load Title Graphic
	if (lcf::Data::system.title_name.empty()) {
		return;
	}
	title = std::make_unique<Sprite>();
	FileRequestAsync* request = AsyncHandler::RequestFile("Title", lcf::Data::system.title_name);
	request->SetGraphicFile(true);
	request_id = request->Bind(&Scene_Translation::OnTitleSpriteReady, this);
	request->Start();
}


void Scene_Translation::CreateTranslationWindow() {
	// Build a list of 'Default' and all known languages.
	std::vector<std::string> lang_names;
	lang_names.push_back("Deutsch");
	lang_dirs.push_back("");
	lang_helps.push_back("Play the game in its original language.");

	// Push menu entries with the display name, but also save the directory location and help text.
	for (const Language& lg : Player::translation.GetLanguages()) {
		lang_names.push_back(lg.lang_name);
		lang_dirs.push_back(lg.lang_dir);
		lang_helps.push_back(lg.lang_desc);
	}

	// Allow overwriting text of the default language
	const Language& def = Player::translation.GetDefaultLanguage();
	if (!def.lang_name.empty()) {
		lang_names.front() = def.lang_name;
	}
	if (!def.lang_desc.empty()) {
		lang_helps.front() = def.lang_desc;
	}

	translate_window = std::make_unique<Window_Command>(lang_names, -1, lang_names.size() > 9 ? 9 : lang_names.size());
	translate_window->UpdateHelpFn = [this](Window_Help& win, int index) {
		if (index >= 0 && index < static_cast<int>(lang_helps.size())) {
			win.SetText(lang_helps[index]);
		}
		else {
			win.SetText("");
		}
	};
	translate_window->SetX(Player::screen_width / 2 - translate_window->GetWidth() / 2);
	translate_window->SetY(Player::screen_height / 2 - translate_window->GetHeight() / 2);

	if (Player::IsRPG2k3E() && lcf::Data::battlecommands.transparency == lcf::rpg::BattleCommands::Transparency_transparent) {
		translate_window->SetBackOpacity(160);
	}

	translate_window->SetVisible(false);
}

void Scene_Translation::CreateHelpWindow() {
	help_window.reset(new Window_Help(0, 0, Player::screen_width, 32));

	if (Player::IsRPG2k3E() && lcf::Data::battlecommands.transparency == lcf::rpg::BattleCommands::Transparency_transparent) {
		help_window->SetBackOpacity(160);
	}

	help_window->SetVisible(false);
	translate_window->SetHelpWindow(help_window.get());
}

void Scene_Translation::Start() {
	CreateTitleGraphic();
	CreateTranslationWindow();
	CreateHelpWindow();

	translate_window->SetActive(true);
	translate_window->SetVisible(true);
	help_window->SetVisible(true);
}


void Scene_Translation::vUpdate() {
	translate_window->Update();
	help_window->Update();


	if (Input::IsTriggered(Input::DECISION)) {
		int index = translate_window->GetIndex();
		ChangeLanguage(lang_dirs.at(index));
	}
	else if (Input::IsTriggered(Input::CANCEL)) {
		Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Cancel));

		Scene::Pop();
	}

}

void Scene_Translation::OnTitleSpriteReady(FileRequestResult* result) {
	BitmapRef bitmapRef = Cache::Title(result->file);

	title->SetBitmap(bitmapRef);

	// If the title sprite doesn't fill the screen, center it to support custom resolutions
	if (bitmapRef->GetWidth() < Player::screen_width) {
		title->SetX(Player::menu_offset_x);
	}
	if (bitmapRef->GetHeight() < Player::screen_height) {
		title->SetY(Player::menu_offset_y);
	}
}

void Scene_Translation::ChangeLanguage(const std::string& lang_str) {
	Main_Data::game_system->SePlay(Main_Data::game_system->GetSystemSE(Main_Data::game_system->SFX_Decision));

	// No-op?
	if (lang_str == Player::translation.GetCurrentLanguage().lang_dir) {
		Scene::Pop();
		return;
	}

	// First change the language
	Player::translation.SelectLanguage(lang_str);
}

void Scene_Translation::OnTranslationChanged() {
	Start();

	Scene::Pop();

	Scene::OnTranslationChanged();
}
