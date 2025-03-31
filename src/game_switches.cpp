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
#include "game_switches.h"
#include "output.h"
#include <lcf/reader_util.h>
#include <lcf/data.h>

Game_Switches::Game_Switches()
	: Game_SwitchesBase() {
}

std::string_view Game_Switches::GetName(int id) const {
	const lcf::rpg::Switch* sw = lcf::ReaderUtil::GetElement(lcf::Data::switches, id);

	if (!sw) {
		// No warning, is valid because the switch array resizes dynamic during runtime
		return {};
	} else {
		return sw->name;
	}
}

game_bool Game_Switches::Flip(int id) {
	if (EP_UNLIKELY(ShouldWarn(id, id))) {
		Output::Debug("Invalid flip {}!", FormatLValue(id, 0));
		--_warnings;
	}

	if (id <= 0) {
		return false;
	}
	auto& storage = GetStorageForEdit();
	storage.prepare(id, id);

	if constexpr (std::is_same<game_bool, bool>::value) {
		storage[id].flip();
		return (bool)storage[id];
	} else {
		/*game_bool& b = storage[id];
		b = (b > 0) ? 0 : 1;
		return b;*/
	}
}

void Game_Switches::FlipRange(int first_id, int last_id) {
	if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
		Output::Debug("Invalid flip {}!", FormatLValue(first_id, last_id));
		--_warnings;
	}

	auto& storage = GetStorageForEdit();
	storage.prepare(first_id, last_id);

	for (int i = std::max(1, first_id); i <= last_id; ++i) {
		if constexpr (std::is_same<game_bool, bool>::value) {
			storage[i].flip();
		} else {
			/*game_bool& b = storage[i];
			b = (b > 0) ? 0 : 1;*/
		}
	}
}
