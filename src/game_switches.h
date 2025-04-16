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

#ifndef EP_GAME_SWITCHES_H
#define EP_GAME_SWITCHES_H

// Headers
#include <vector>
#include <string>
#include <string_view.h>
#include "game_scoped_storage.h"
#include <lcf/rpg/database.h>
#include "compiler.h"
#include "output.h"

/**
 * Game_Switches class
 */
class Game_Switches : public Game_SwitchesBase {
public:
	using Switches_t = std::vector<game_bool>;

	Game_Switches();

	game_bool Flip(int id);
	void FlipRange(int first_id, int last_id);

	std::string_view GetName(int id) const override;

	int GetInt(int switch_id) const;
};

inline int Game_Switches::GetInt(int switch_id) const {
	return Get(switch_id) ? 1 : 0;
}

#endif

