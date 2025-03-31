/*
 * This file is part of liblcf. Copyright (c) 2020 liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#include "lcf/rpg/database.h"
#include "lcf/data.h"

namespace lcf {

namespace Data {
	rpg::Database data;

	std::vector<rpg::Actor>& actors = data.actors;
	std::vector<rpg::Skill>& skills = data.skills;
	std::vector<rpg::Item>& items = data.items;
	std::vector<rpg::Enemy>& enemies = data.enemies;
	std::vector<rpg::Troop>& troops = data.troops;
	std::vector<rpg::Terrain>& terrains = data.terrains;
	std::vector<rpg::Attribute>& attributes = data.attributes;
	std::vector<rpg::State>& states = data.states;
	std::vector<rpg::Animation>& animations = data.animations;
	std::vector<rpg::Chipset>& chipsets = data.chipsets;
	std::vector<rpg::CommonEvent>& commonevents = data.commonevents;
	rpg::BattleCommands& battlecommands = data.battlecommands;
	std::vector<rpg::Class>& classes = data.classes;
	std::vector<rpg::BattlerAnimation>& battleranimations = data.battleranimations;
	rpg::Terms& terms = data.terms;
	rpg::System& system = data.system;
	std::vector<rpg::Switch>& switches = data.switches;
	std::vector<rpg::Variable>& variables = data.variables;
	std::vector<rpg::StringVariable>& maniac_string_variables = data.maniac_string_variables;

	rpg::TreeMap treemap;

#ifdef SCOPEDVARS_LIBLCF_STUB

	std::vector<rpg::Switch> CreateFrameSwitches() {
		std::vector<rpg::Switch> tmp;
		tmp.push_back({ 1, DBString("FrameSw 1") });
		tmp.push_back({ 2, DBString("FrameSw 2") });
		tmp.push_back({ 3, DBString("FrameSw 3") });
		tmp.push_back({ 4, DBString("FrameSw 4") });
		tmp.push_back({ 5, DBString("FrameSw 5") });
		return tmp;
	}
	std::vector<rpg::Variable> CreateFrameVars() {
		std::vector<rpg::Variable> tmp;
		tmp.push_back({ 1, DBString("FrameVar 1") });
		tmp.push_back({ 2, DBString("FrameVar 2") });
		tmp.push_back({ 3, DBString("FrameVar 3") });
		tmp.push_back({ 4, DBString("FrameVar 4") });
		tmp.push_back({ 5, DBString("FrameVar 5") });
		return tmp;
	}
	std::vector<rpg::ScopedSwitch> CreateMapSwitches() {
		std::vector<rpg::ScopedSwitch> tmp;
		tmp.push_back({ 1, DBString("A"), false, false, false, false, false, false });
		tmp.push_back({ 2, DBString("B"), false, false, false, false, false, false });
		tmp.push_back({ 3, DBString("C"), false, false, false, false, false, true });
		tmp.push_back({ 4, DBString("D"), false, false, false, false, true, false });
		tmp.push_back({ 5, DBString("E"), false, false, false, false, true, true });
		return tmp;
	}
	std::vector<rpg::ScopedVariable> CreateMapVars() {
		std::vector<rpg::ScopedVariable> tmp;
		tmp.push_back({ 1, DBString("A"), 4, true, false, false, false, false });
		tmp.push_back({ 2, DBString("B"), 0, false, false, false, false, false });
		tmp.push_back({ 3, DBString("C"), 0, false, false, false, false, false });
		tmp.push_back({ 4, DBString("D"), 0, false, false, false, false, false });
		tmp.push_back({ 5, DBString("E"), 0, false, false, false, false, false });
		return tmp;
	}
	std::vector<rpg::ScopedSwitch> CreateSelfSwitches() {
		std::vector<rpg::ScopedSwitch> tmp;
		tmp.push_back({ 1, DBString("A"), false, false, false, false, false, false });
		tmp.push_back({ 2, DBString("B"), false, false, false, false, false, false });
		tmp.push_back({ 3, DBString("C"), false, false, true, false, false, false });
		tmp.push_back({ 4, DBString("D"), false, false, false, false, true, false });
		tmp.push_back({ 5, DBString("E"), false, false, false, false, false, false });
		return tmp;
	}
	std::vector<rpg::ScopedVariable> CreateSelfVars() {
		std::vector<rpg::ScopedVariable> tmp;
		tmp.push_back({ 1, DBString("A"), 0, false, false, false, false, false });
		tmp.push_back({ 2, DBString("B"), 0, false, false, false, false, false });
		tmp.push_back({ 3, DBString("C"), 0, false, false, false, false, false });
		tmp.push_back({ 4, DBString("D"), 0, false, false, false, false, false });
		tmp.push_back({ 5, DBString("E"), 0, false, false, false, false, false });
		return tmp;
	}

	std::vector<rpg::Switch> data_easyrpg_frame_switches = CreateFrameSwitches();
	std::vector<rpg::Variable> data_easyrpg_frame_variables = CreateFrameVars();
	std::vector<rpg::ScopedSwitch> data_easyrpg_map_switches = CreateMapSwitches();
	std::vector<rpg::ScopedVariable> data_easyrpg_map_variables = CreateMapVars();
	std::vector<rpg::ScopedSwitch> data_easyrpg_self_switches = CreateSelfSwitches();
	std::vector<rpg::ScopedVariable> data_easyrpg_self_variables = CreateSelfVars();

	std::vector<rpg::Switch>& easyrpg_frame_switches = data_easyrpg_frame_switches;
	std::vector<rpg::Variable>& easyrpg_frame_variables = data_easyrpg_frame_variables;
	std::vector<rpg::ScopedSwitch>& easyrpg_map_switches = data_easyrpg_map_switches;
	std::vector<rpg::ScopedVariable>& easyrpg_map_variables = data_easyrpg_map_variables;
	std::vector<rpg::ScopedSwitch>& easyrpg_self_switches = data_easyrpg_self_switches;
	std::vector<rpg::ScopedVariable>& easyrpg_self_variables = data_easyrpg_self_variables;
#else
	std::vector<rpg::Switch>& easyrpg_frame_switches = data.easyrpg_frame_switches;
	std::vector<rpg::Variable>& easyrpg_frame_variables = data.easyrpg_frame_variables;
	std::vector<rpg::ScopedSwitch>& easyrpg_map_switches =  data.easyrpg_map_switches;
	std::vector<rpg::ScopedVariable>& easyrpg_map_variables = data.easyrpg_map_variables;
	std::vector<rpg::ScopedSwitch>& easyrpg_self_switches = data.easyrpg_self_switches;
	std::vector<rpg::ScopedVariable>& easyrpg_self_variables = data.easyrpg_self_variables;
#endif
}

void Data::Clear() {
	actors.clear();
	skills.clear();
	items.clear();
	enemies.clear();
	troops.clear();
	terrains.clear();
	attributes.clear();
	states.clear();
	animations.clear();
	chipsets.clear();
	commonevents.clear();
	battlecommands = rpg::BattleCommands();
	classes.clear();
	battleranimations.clear();
	terms = rpg::Terms();
	system = rpg::System();
	switches.clear();
	variables.clear();
	maniac_string_variables.clear();
	treemap.active_node = 0;
	treemap.maps.clear();
	treemap.tree_order.clear();
}

} //namespace lcf
