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

#ifndef EP_GAME_VARIABLES_H
#define EP_GAME_VARIABLES_H

// Headers
#include "game_scoped_storage.h"
#include "output.h"
#include <lcf/reader_util.h>
#include <lcf/data.h>
#include <lcf/rpg/database.h>
#include "utils.h"
#include "rand.h"
#include <cmath>
#include "string_view.h"
#include <cstdint>
#include <string>

/**
 * Game_Variables class
 */
class Game_Variables : public Game_VariablesBase {
public:
	using Variables_t = std::vector<Var_t>;

	static constexpr int max_warnings = 10;

	static constexpr Var_t min_2k = -999999;
	static constexpr Var_t max_2k = 999999;
	static constexpr Var_t min_2k3 = -9999999;
	static constexpr Var_t max_2k3 = 9999999;

	Game_Variables(Var_t minval, Var_t maxval);

	std::string_view GetName(int id) const override;

	std::vector<Var_t> GetRange(int variable_id, int length); //TODO!
	Var_t GetIndirect(int variable_id) const;

	Var_t Add(int variable_id, Var_t value);
	Var_t Sub(int variable_id, Var_t value);
	Var_t Mult(int variable_id, Var_t value);
	Var_t Div(int variable_id, Var_t value);
	Var_t Mod(int variable_id, Var_t value);
	Var_t BitOr(int variable_id, Var_t value);
	Var_t BitAnd(int variable_id, Var_t value);
	Var_t BitXor(int variable_id, Var_t value);
	Var_t BitShiftLeft(int variable_id, Var_t value);
	Var_t BitShiftRight(int variable_id, Var_t value);

	void AddRange(int first_id, int last_id, Var_t value);
	void SubRange(int first_id, int last_id, Var_t value);
	void MultRange(int first_id, int last_id, Var_t value);
	void DivRange(int first_id, int last_id, Var_t value);
	void ModRange(int first_id, int last_id, Var_t value);
	void BitOrRange(int first_id, int last_id, Var_t value);
	void BitAndRange(int first_id, int last_id, Var_t value);
	void BitXorRange(int first_id, int last_id, Var_t value);
	void BitShiftLeftRange(int first_id, int last_id, Var_t value);
	void BitShiftRightRange(int first_id, int last_id, Var_t value);

	void SetRangeVariable(int first_id, int last_id, int var_id);
	void AddRangeVariable(int first_id, int last_id, int var_id);
	void SubRangeVariable(int first_id, int last_id, int var_id);
	void MultRangeVariable(int first_id, int last_id, int var_id);
	void DivRangeVariable(int first_id, int last_id, int var_id);
	void ModRangeVariable(int first_id, int last_id, int var_id);
	void BitOrRangeVariable(int first_id, int last_id, int var_id);
	void BitAndRangeVariable(int first_id, int last_id, int var_id);
	void BitXorRangeVariable(int first_id, int last_id, int var_id);
	void BitShiftLeftRangeVariable(int first_id, int last_id, int var_id);
	void BitShiftRightRangeVariable(int first_id, int last_id, int var_id);

	void SetRangeVariableIndirect(int first_id, int last_id, int var_id);
	void AddRangeVariableIndirect(int first_id, int last_id, int var_id);
	void SubRangeVariableIndirect(int first_id, int last_id, int var_id);
	void MultRangeVariableIndirect(int first_id, int last_id, int var_id);
	void DivRangeVariableIndirect(int first_id, int last_id, int var_id);
	void ModRangeVariableIndirect(int first_id, int last_id, int var_id);
	void BitOrRangeVariableIndirect(int first_id, int last_id, int var_id);
	void BitAndRangeVariableIndirect(int first_id, int last_id, int var_id);
	void BitXorRangeVariableIndirect(int first_id, int last_id, int var_id);
	void BitShiftLeftRangeVariableIndirect(int first_id, int last_id, int var_id);
	void BitShiftRightRangeVariableIndirect(int first_id, int last_id, int var_id);

	void SetRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void AddRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void SubRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void MultRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void DivRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void ModRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void BitOrRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void BitAndRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void BitXorRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void BitShiftLeftRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);
	void BitShiftRightRangeRandom(int first_id, int last_id, Var_t minval, Var_t maxval);

	void EnumerateRange(int first_id, int last_id, Var_t value);
	void SortRange(int first_id, int last_id, bool asc);
	void ShuffleRange(int first_id, int last_id);

	void SetArray(int first_id_a, int last_id_a, int first_id_b);
	void AddArray(int first_id_a, int last_id_a, int first_id_b);
	void SubArray(int first_id_a, int last_id_a, int first_id_b);
	void MultArray(int first_id_a, int last_id_a, int first_id_b);
	void DivArray(int first_id_a, int last_id_a, int first_id_b);
	void ModArray(int first_id_a, int last_id_a, int first_id_b);
	void BitOrArray(int first_id_a, int last_id_a, int first_id_b);
	void BitAndArray(int first_id_a, int last_id_a, int first_id_b);
	void BitXorArray(int first_id_a, int last_id_a, int first_id_b);
	void BitShiftLeftArray(int first_id_a, int last_id_a, int first_id_b);
	void BitShiftRightArray(int first_id_a, int last_id_a, int first_id_b);
	void SwapArray(int first_id_a, int last_id_a, int first_id_b);

	int GetMaxDigits() const;

protected:
	void AssignOpImpl(Var_t& target, Var_t value) const;

	void ValidateRangeOp(int first_id, int last_id, Var_t value, const char* op) const;
	void ValidateRangeVarOp(int first_id, int last_id, int var_id, const char* op) const;
	void ValidateRangeVarIndirectOp(int first_id, int last_id, int var_id, const char* op) const;
	void ValidateRangeRandomOp(int first_id, int last_id, Var_t minval, Var_t maxval, const char* op) const;

	template<typename F>
	void WriteRangeVariable(int first_id, const int last_id, const int var_id, F&& op);

	template <typename... Args>
	void PrepareArray(const int first_id_a, const int last_id_a, const int first_id_b, const char* warn, Args... args);
	template <typename F>
	void WriteArray(const int first_id_a, const int last_id_a, const int first_id_b, F&& op);
};

inline void Game_VariablesBase::AssignOpImpl(Var_t& target, Var_t value) const {
	target = Utils::Clamp(value, GetMinValue(), GetMaxValue());
}

inline Game_Variables::Var_t Game_Variables::GetIndirect(int variable_id) const {
	auto val_indirect = Get(variable_id);
	return Get(static_cast<int>(val_indirect));
}

inline void Game_Variables::ValidateRangeOp(int first_id, int last_id, Var_t value, const char* op) const {
	if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
		Output::Debug("Invalid write {} {} {}!", this->FormatLValue(first_id, last_id), op, this->FormatRValue(value));
		--_warnings;
	}
}

inline void Game_Variables::ValidateRangeVarOp(int first_id, int last_id, int var_id, const char* op) const {
	if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
		Output::Debug("Invalid write {} {} {}!", this->FormatLValue(first_id, last_id), op, this->FormatRValue(var_id, "var"));
		--_warnings;
	}
}

inline void Game_Variables::ValidateRangeVarIndirectOp(int first_id, int last_id, int var_id, const char* op) const {
	if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
		Output::Debug("Invalid write {} {} var[var[{}]]!", this->FormatLValue(first_id, last_id), op, var_id);
		--_warnings;
	}
}

inline void Game_Variables::ValidateRangeRandomOp(int first_id, int last_id, Var_t minval, Var_t maxval, const char* op) const {
	if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
		Output::Debug("Invalid write {} {} rand({},{})!", this->FormatLValue(first_id, last_id), op, minval, maxval);
		--_warnings;
	}
}

#endif
