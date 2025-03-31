/*
 * This file is part of liblcf. Copyright (c) 2020 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#define SCOPEDVARS_LIBLCF_STUB 1

#ifndef LCF_DATA_H
#define LCF_DATA_H

#include <string>
#include <vector>
#include "lcf/rpg/actor.h"
#include "lcf/rpg/skill.h"
#include "lcf/rpg/item.h"
#include "lcf/rpg/enemy.h"
#include "lcf/rpg/troop.h"
#include "lcf/rpg/attribute.h"
#include "lcf/rpg/state.h"
#include "lcf/rpg/terrain.h"
#include "lcf/rpg/animation.h"
#include "lcf/rpg/chipset.h"
#include "lcf/rpg/terms.h"
#include "lcf/rpg/system.h"
#include "lcf/rpg/commonevent.h"
#include "lcf/rpg/class.h"
#include "lcf/rpg/battlecommand.h"
#include "lcf/rpg/battleranimation.h"
#include "lcf/rpg/sound.h"
#include "lcf/rpg/music.h"
#include "lcf/rpg/eventcommand.h"
#include "lcf/rpg/treemap.h"
#include "lcf/rpg/database.h"
#ifndef SCOPEDVARS_LIBLCF_STUB
#include "lcf/rpg/scopedswitch.h"
#include "lcf/rpg/scopedvariable.h"
#endif

#ifdef SCOPEDVARS_LIBLCF_STUB

namespace lcf {
	namespace rpg {
		class ScopedSwitch {
		public:
			int ID = 0;
			lcf::DBString name;
			bool default_value = false;
			bool default_value_defined = false;
			bool is_readonly = false;
			bool show_in_editor = false;
			bool auto_reset = false;
			bool map_group_inherited_value = false;
		};
		class ScopedVariable {
		public:
			int ID = 0;
			lcf::DBString name;
			int32_t default_value = false;
			bool default_value_defined = false;
			bool is_readonly = false;
			bool show_in_editor = false;
			bool auto_reset = false;
			bool map_group_inherited_value = false;
		};
		class SaveScopedSwitchData {
		public:
			int32_t id = 1;
			int32_t scope = 0;
			bool on = false;
			int32_t map_id = 0;
			int32_t event_id = 0;
			bool auto_reset = false;
		};
		class SaveScopedVariableData {
		public:
			int32_t id = 1;
			int32_t scope = 0;
			int32_t value = 0;
			int32_t map_id = 0;
			int32_t event_id = 0;
			bool auto_reset = false;
		};
	}
}

#endif

namespace lcf {
/**
 * Data namespace
 */
namespace Data {
	/** Database Data (ldb) */
	extern rpg::Database data;
	/** @{ */
	extern std::vector<rpg::Actor>& actors;
	extern std::vector<rpg::Skill>& skills;
	extern std::vector<rpg::Item>& items;
	extern std::vector<rpg::Enemy>& enemies;
	extern std::vector<rpg::Troop>& troops;
	extern std::vector<rpg::Terrain>& terrains;
	extern std::vector<rpg::Attribute>& attributes;
	extern std::vector<rpg::State>& states;
	extern std::vector<rpg::Animation>& animations;
	extern std::vector<rpg::Chipset>& chipsets;
	extern std::vector<rpg::CommonEvent>& commonevents;
	extern rpg::BattleCommands& battlecommands;
	extern std::vector<rpg::Class>& classes;
	extern std::vector<rpg::BattlerAnimation>& battleranimations;
	extern rpg::Terms& terms;
	extern rpg::System& system;
	extern std::vector<rpg::Switch>& switches;
	extern std::vector<rpg::Variable>& variables;

	extern std::vector<rpg::Switch>& easyrpg_frame_switches;
	extern std::vector<rpg::Variable>& easyrpg_frame_variables;
	extern std::vector<rpg::ScopedSwitch>& easyrpg_map_switches;
	extern std::vector<rpg::ScopedVariable>& easyrpg_map_variables;
	extern std::vector<rpg::ScopedSwitch>& easyrpg_self_switches;
	extern std::vector<rpg::ScopedVariable>& easyrpg_self_variables;
	extern std::vector<rpg::StringVariable>& maniac_string_variables;
	/** @} */

	/** TreeMap (lmt) */
	extern rpg::TreeMap treemap;

	/**
	 * Clears database data.
	 */
	void Clear();
}

} //namespace lcf

#endif
