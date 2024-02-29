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

#ifndef EP_SCENE_ITEM_EX_H
#define EP_SCENE_ITEM_EX_H

 // Headers
#include "scene.h"
#include "scene.h"
#include "window_help.h"
#include "window_item.h"

/**
 * Scene_Item_Ex class.
 * This variant of the default style Item scene adds a new window & introduces
 * the ability to switch between different item categories.
 * The names for these categories are hardcoded to be read from string variables:
 * string_id = 20 + [Item.easyrpg_category]
 * 
 * default is easyrpg_category = 0 -> this should be used as 'Unsorted'
 * This special category tab will only show up, if the player actually has an item
 * with no set category in their inventory.
 */
class Scene_Item_Ex : public Scene {

public:
	/**
	 * Constructor.
	 *
	 * @param item_index index to select.
	 */
	Scene_Item_Ex(int category_index = 1, int item_index = 0);

	void Start() override;
	void Continue(SceneType prev_scene) override;
	void vUpdate() override;
	void TransitionOut(Scene::SceneType next_scene) override;

private:
	/** Displays description about the selected item. */
	std::unique_ptr<Window_Help> help_window;
	/** Index of item selected on startup. */
	int item_index;

	int category_min;
	int category_max;

	int category_index;
	std::vector<Window_Item*> item_windows;

	std::unique_ptr<Window_Help> category_window;

	void UpdateCategoryWindow();
	void UpdateArrows();
	int arrow_frame = 0;
};

#endif
