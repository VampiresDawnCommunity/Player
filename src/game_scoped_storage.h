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

#ifndef EP_GAME_SCOPED_STORAGE_H
#define EP_GAME_SCOPED_STORAGE_H

// Headers
#include <vector>
#include <string>
#include <string_view.h>
#include <lcf/rpg/database.h>
#include "lcf/data.h"
#ifndef SCOPEDVARS_LIBLCF_STUB
#include <lcf/rpg/savescopedswitchdata.h>
#include <lcf/rpg/savescopedvariabledata.h>
#endif
#include <lcf/rpg/saveeventexecframe.h>
#include "compiler.h"
#include "output.h"
#include <lcf/reader_util.h>

enum DataScopeType {
	eDataScope_Global = 0,

	eDataScope_COUNT
};

// Use a whole byte for switches instead of a bitmask?
// (std::vector implementation differs for boolean data types)
//typedef unsigned char game_bool;
typedef bool game_bool;

namespace VarStorage {

	enum DataStorageType : std::int8_t {
		eStorageType_Switch = 0,
		eStorageType_Variable = 1,
		eStorageType_String = 2,
		eStorageType_Json = 3
	};

	enum DataStorageMode {
		eStorageMode_Vector = 0,
		eStorageMode_Map,
		eStorageMode_Mixed
	};

	constexpr std::string_view TypeToStr(int type) {
		switch (type) {
			case eStorageType_Switch: return "Sw";
			case eStorageType_Variable: return "Var";
			case eStorageType_String: return "VarStr";
			case eStorageType_Json: return "Json";
		}
		return "Unk";
	}

	template <DataStorageMode storage_mode>
	constexpr bool IsVector = (storage_mode == eStorageMode_Vector);

	template <DataStorageMode storage_mode>
	constexpr bool IsMap = (storage_mode == eStorageMode_Map);

	template <DataStorageMode storage_mode>
	constexpr bool IsMixed = (storage_mode == eStorageMode_Mixed);

	template <typename V>
	struct vector_map_mixed {
		size_t bounds_vector = 200;
		int max_key_map = 0;
		std::vector<V> vec;
		std::unordered_map<size_t, V> map;

		size_t size() noexcept {
			if (map.empty())
				return vec.size();
			return static_cast<size_t>(max_key_map);
		}
	};

	template <typename V, DataStorageMode storage_mode>
	struct DataStorage_t {
		using storage_t = std::conditional_t<storage_mode == eStorageMode_Vector,
			std::vector<V>,
			std::conditional_t<storage_mode == eStorageMode_Map,
				std::unordered_map<int, V>,
				vector_map_mixed<V>
			>
		>;

		using reference = std::conditional_t<std::is_same<V, bool>::value, std::vector<bool>::reference, V&>;
		using const_reference = std::conditional_t<std::is_same<V, bool>::value, std::vector<bool>::const_reference, V const&>;

	/*	template <DataStorageMode S = storage_mode, typename std::enable_if<IsMixed<S>, int>::type = 0>
		inline void setVectorBounds(size_t bounds) noexcept {
			this->data.bounds_vector = bounds;
		}*/

		template <DataStorageMode S = storage_mode, typename std::enable_if<IsVector<S>, int>::type = 0>
		inline const bool containsKey(const int id) const {
			if constexpr (IsVector<storage_mode>) {
				return id > 0 && id <= static_cast<int>(data.size());
			} else if constexpr (IsMap<storage_mode>) {
				auto it = data.find(id);
				return it != data.end();
			} else {
				return (id <= data.bounds_vector)
					? (id > 0 && id <= static_cast<int>(data.vec.size()))
					: (data.map.find(id) != data.map.end());
			}
		};

		inline reference operator[](const int id) noexcept {
			return data[id - 1];
		};

		inline const const_reference operator[](const int id) const noexcept {
			return data[id - 1];
		};
		template <DataStorageMode S = storage_mode, typename std::enable_if<IsMap<S>, int>::type = 0>
		inline reference operator[](const int id) noexcept {
			auto it = data.find(id);
			return it->second;
		};

		template <DataStorageMode S = storage_mode, typename std::enable_if<IsMap<S>, int>::type = 0>
		inline const reference operator[](const int id) const noexcept {
			auto it = data.find(id);
			return it->second;
		};

		template <DataStorageMode S = storage_mode, typename std::enable_if<IsVector<S>, int>::type = 0>
		inline void prepare(const int first_id, const int last_id)  {
			if constexpr (IsVector<storage_mode>) {
				if (EP_UNLIKELY(last_id > static_cast<int>(data.size()))) {
					data.resize(last_id, false);
				}
			} else if constexpr (IsMixed<storage_mode>) {
				if (EP_UNLIKELY(last_id <= data.bounds_vector && last_id > static_cast<int>(data.vec.size()))) {
					data.vec.resize(last_id, false);
				} else if (EP_UNLIKELY(last_id > data.max_key_map)) {
					data.max_key_map = last_id;
				}
			}
		};
		const size_t size() const noexcept {
			return data.size();
		}

		template <typename V2 = V, typename std::enable_if<std::is_same<V2, bool>::value, int>::type = 0>
		inline void flip(const int id) {
			this->data.flip(id - 1);
		}

		inline void setData(std::vector<V> data) {
			if constexpr (IsVector<storage_mode>) {
				this->data = std::move(data);
			} else if constexpr (IsMap<storage_mode>) {
				//TODO
			} else {
				//TODO
			}
		}
		template <DataStorageMode S = storage_mode, typename std::enable_if<IsVector<S>, int>::type = 0>
		inline const std::vector<V> getData() const {
			if constexpr (IsVector<storage_mode>) {
				return this->data;
			} else if constexpr (IsMap<storage_mode>) {
				//TODO
			} else {
				//TODO
			}
		}

		inline void prepare_iterate(const int first_id, const int last_id) {
			static_assert(!IsMap<storage_mode>);

			if constexpr (IsVector<storage_mode>) {
				prepare(first_id, last_id);
			} else if constexpr (IsMixed<storage_mode>) {
				if (EP_UNLIKELY(last_id <= data.bounds_vector && last_id > static_cast<int>(data.vec.size()))) {
					data.vec.resize(last_id, false);
				} else if (EP_UNLIKELY(last_id > data.max_key_map)) {
					data.max_key_map = last_id + 1;
					data.bounds_vector = last_id;
					data.vec.resize(last_id, false);

					for (auto& it = data.map.begin(); it != data.map.end();) {
						if (it.first < data.max_key_map) {
							it = data.map.erase(it);
						} else {
							it++;
						}
					}
				}
			}
		}

		inline typename std::vector<V>::iterator begin() noexcept {
			static_assert(!IsMap<storage_mode>);
			return data.begin();
		}

		inline typename std::vector<V>::const_iterator begin() const noexcept {
			static_assert(!IsMap<storage_mode>);
			return data.begin();
		}

		inline typename std::vector<V>::iterator end() noexcept {
			static_assert(!IsMap<storage_mode>);
			return data.end();
		}

		inline typename std::vector<V>::const_iterator end() const noexcept {
			static_assert(!IsMap<storage_mode>);
			return data.end();
		}
	private:
		storage_t data;
	};
}

namespace DynamicScope {
	constexpr int count_global_scopes = 1;

	constexpr bool IsGlobalScope(DataScopeType scope) {
		return scope == eDataScope_Global;
	}

	constexpr std::string_view ScopeToStr(DataScopeType scope) {
		switch (scope) {
			case eDataScope_Global: return "";
			default:
				return "Unk";
		}
	}

	template <typename Data_t, typename V>
	class ScopedDataStorage {
	public:
		enum Flags : uint8_t {
			eFlag_ReadOnly = 0x01,
			eFlag_AutoReset = 0x02,
			eFlag_ValueDefined = 0x04,
			eFlag_DefaultValueDefined = 0x08,
			eFlag_MapGrpInheritedValue = 0x10
		};

		inline const bool containsKey(const int id) const {
			return data.size();
		};
		inline typename Data_t::reference operator[](const int id) noexcept {
			return data[id];
		};
		inline typename Data_t::const_reference operator[](const int id) const noexcept {
			return data[id];
		};
		inline void prepare(const int first_id, const int last_id) {
			data.prepare(first_id, last_id);
		};
		const size_t size() const noexcept {
			return data.size();
		}
		inline void setData(std::vector<V> data) {
			return this->data.setData(data);
		}
		inline std::vector<V> getData() const {
			return this->data.getData();
		}
		bool valid = false;
		int map_id = 0, evt_id = 0;
		std::unordered_map<int, int> flags;
	private:
		Data_t data;
	};

	using limit_array_t = std::array <size_t, (eDataScope_COUNT)>;
	template<typename Data_t> using globals_array_t = std::array<Data_t, DynamicScope::count_global_scopes>;

	template<DataScopeType, typename Data_t> constexpr bool IsValid(limit_array_t const& limits, globals_array_t<Data_t> const& globals, const int id);
	template<DataScopeType> constexpr size_t GetLimit(limit_array_t const& limits);
	template<DataScopeType> constexpr void SetLowerLimit(limit_array_t& limits, size_t limit);
	template<DataScopeType, typename Data_t> constexpr int GetSize(globals_array_t<Data_t> const& globals);
	template<DataScopeType, typename Data_t> constexpr int GetSizeWithLimit(limit_array_t const& limits, globals_array_t<Data_t> const& globals);

	constexpr void InitLimits(limit_array_t& limits) {
		limits[eDataScope_Global] = 0;
	}

	template<DataScopeType S, typename Data_t> constexpr bool IsValid(limit_array_t const& limits, globals_array_t<Data_t> const& globals, const int id) {
		if constexpr (DynamicScope::IsGlobalScope(S)) {
			return id > 0 && id <= GetSizeWithLimit<S>(limits, globals);
		}
		return false;
	}

	template<DataScopeType S> constexpr size_t GetLimit(limit_array_t const& limits) {
		static_assert(S < eDataScope_COUNT);

		return limits[static_cast<int>(S)];
	}

	template<DataScopeType S> constexpr void SetLowerLimit(limit_array_t& limits, size_t limit) {
		if constexpr (DynamicScope::IsGlobalScope(S)) {
			limits[static_cast<int>(S)] = limit;
		}
	}

	template<DataScopeType S, typename Data_t> constexpr int GetSize(globals_array_t<Data_t> const& globals) {
		return static_cast<int>(globals[static_cast<int>(S)].size());
	}

	template<DataScopeType S, typename Data_t> constexpr int GetSizeWithLimit(limit_array_t const& limits, globals_array_t<Data_t> const& globals) {
		return std::max<int>(static_cast<int>(GetLimit<S>(limits)), static_cast<int>(globals[static_cast<int>(S)].size()));
	}

	template<DataScopeType S, typename Data_t> const Data_t& GetStorage(globals_array_t<Data_t> const& globals) {
		return globals[static_cast<int>(S)];
	}

	template<DataScopeType S, typename Data_t> Data_t& GetStorageForEdit(globals_array_t<Data_t>& globals) {
		return globals[static_cast<int>(S)];
	}
}

template <typename T, typename V>
class Game_VectorDataStorageBase {
public:
	typedef VarStorage::DataStorage_t<V, VarStorage::eStorageMode_Vector> Data_t;

	Game_VectorDataStorageBase() { DynamicScope::InitLimits(_limits); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline bool IsValid(const int id) const { return DynamicScope::IsValid<S, Data_t>(_limits, _globals, id); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline size_t GetLimit() const { return DynamicScope::GetLimit<S>(_limits); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline void SetLowerLimit(size_t limit) { DynamicScope::SetLowerLimit<S>(_limits, limit); }

	template<DataScopeType S = eDataScope_Global>
	inline int GetSize() const { return DynamicScope::GetSize<S, Data_t>(_globals); }

	template<DataScopeType S = eDataScope_Global>
	inline int GetSizeWithLimit() const { return DynamicScope::GetSizeWithLimit<S, Data_t>(_limits, _globals); }

	template<DataScopeType = eDataScope_Global, typename C = V>
	void SetData(std::vector<V> data);

	template<DataScopeType = eDataScope_Global, typename C = V>
	std::vector<V> GetData() const;

protected:
	template<DataScopeType S = eDataScope_Global>
	const Data_t& GetStorage() const { return DynamicScope::GetStorage<S, Data_t>(_globals); }

	template<DataScopeType S = eDataScope_Global>
	Data_t& GetStorageForEdit() { return DynamicScope::GetStorageForEdit<S, Data_t>(_globals); }

private:
	DynamicScope::globals_array_t<Data_t> _globals;
	DynamicScope::limit_array_t _limits;
};

template <typename T, typename V>
class Game_MapDataStorageBase {
public:
	typedef VarStorage::DataStorage_t<V, VarStorage::eStorageMode_Map> Data_t;

	static_assert(!std::is_same<V, bool>::value);

	Game_MapDataStorageBase() { DynamicScope::InitLimits(_limits); }

protected:
	template<DataScopeType S = eDataScope_Global>
	const Data_t& GetStorage() const { return DynamicScope::GetStorage<S, Data_t>(_globals); }

	template<DataScopeType S = eDataScope_Global>
	Data_t& GetStorageForEdit() { return DynamicScope::GetStorageForEdit<S, Data_t>(_globals); }

private:
	DynamicScope::globals_array_t<Data_t> _globals;
	DynamicScope::limit_array_t _limits;
};

template <typename T, typename V>
class Game_MixedDataStorageBase {
public:
	typedef VarStorage::DataStorage_t<V, VarStorage::eStorageMode_Mixed> Data_t;

	Game_MixedDataStorageBase() { DynamicScope::InitLimits(_limits); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline bool IsValid(const int id) const { return DynamicScope::IsValid<S, Data_t>(_limits, _globals, id); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline size_t GetLimit() const { return DynamicScope::GetLimit<S>(_limits); }

	template<DataScopeType S = eDataScope_Global, typename... Args>
	inline void SetLowerLimit(size_t limit) {
		DynamicScope::SetLowerLimit<S>(_limits, limit);
		for (int i = 0; i < _globals.size(); i++)
			_globals[i].bounds_vector = limit;
	}

	template<DataScopeType S = eDataScope_Global>
	inline int GetSize() const { return DynamicScope::GetSize<S, Data_t>(_globals); }

	template<DataScopeType S = eDataScope_Global>
	inline int GetSizeWithLimit() const { return DynamicScope::GetSizeWithLimit<S, Data_t>(_limits, _globals); }

	template<DataScopeType = eDataScope_Global, typename C = V>
	void SetData(std::vector<V> data);
	template<DataScopeType = eDataScope_Global, typename C = V>
	std::vector<V> GetData() const;

protected:
	template<DataScopeType S = eDataScope_Global>
	const Data_t& GetStorage() const { return DynamicScope::GetStorage<S, Data_t>(_globals); }

	template<DataScopeType S = eDataScope_Global>
	Data_t& GetStorageForEdit() { return DynamicScope::GetStorageForEdit<S, Data_t>(_globals); }

private:
	DynamicScope::globals_array_t<Data_t> _globals;
	DynamicScope::limit_array_t _limits;
};


template <typename T, typename V, VarStorage::DataStorageMode storage_mode>
using Game_DataStorageBase = std::conditional_t<storage_mode == VarStorage::eStorageMode_Vector,
	Game_VectorDataStorageBase<T, V>,
	std::conditional_t<storage_mode == VarStorage::eStorageMode_Map,
		Game_MapDataStorageBase<T, V>,
		Game_MixedDataStorageBase<T, V>
	>
>;

template <typename D, typename T, typename V, VarStorage::DataStorageMode storage_mode>
class Game_DataStorage : public Game_DataStorageBase<T, V, storage_mode> {
public:
	using BaseType = Game_DataStorageBase<T, V, storage_mode>;
	using typename Game_DataStorageBase<T, V, storage_mode>::Data_t;

	static constexpr int kMaxWarnings = 10;

	Game_DataStorage(int type)
		: Game_DataStorageBase<T, V, storage_mode>(), _type(type) {	}

	~Game_DataStorage() = default;

	virtual std::string_view GetName(int id) const = 0;

	void SetWarning(int w) {
		_warnings = w;
	}

	V Get(int id) const {
		if (EP_UNLIKELY(ShouldWarn(id, id))) {
			WarnGet(id);
		}
		const Data_t& storage = this->GetStorage();
		if (!storage.containsKey(id))
			return false;
		return storage[id];
	}

	V Set(int id, V value) {
		return PerformOperation(id, std::forward<V>(value), [](V o, V n) {
			(void)o;
			return n;
		}, "=");
	}

	void SetRange(int first_id, int last_id, V value) {
		if (EP_UNLIKELY(ShouldWarn(first_id, last_id))) {
			Output::Debug("Invalid write {} = {}!", this->FormatLValue(first_id, last_id), this->FormatRValue(value));
			--_warnings;
		}
		PrepareRange(first_id, last_id);
		PerformRangeOperation(first_id, last_id, value, [](V o, V n) {
			(void)o;
			return n;
		});
	}

protected:
	using op_Func = V(*)(const V o, const V n);

	bool ShouldWarn(int first_id, int last_id) const {
		if constexpr (storage_mode == VarStorage::eStorageMode_Vector) {
			return (first_id <= 0 || last_id > this->GetSizeWithLimit()) && (_warnings > 0);
		}
		return (first_id <= 0 && _warnings > 0);
	}

	void WarnGet(int id) const {
		Output::Debug("Invalid read {}!", FormatLValue(id, 0));
		--_warnings;
	}

	void WarnSet(int id, V value, const char* op) const {
		Output::Debug("Invalid write {} {} {}!", FormatLValue(id, 0), op, FormatRValue(value));
		--_warnings;
	}

	void WarnSetRange(int first_id, int last_id, V value, const char* op) const {
		Output::Debug("Invalid write {} {} {}!", FormatLValue(first_id, last_id), op, FormatRValue(value));
		--_warnings;
	}

	V PerformOperation(const int id, const V value, op_Func&& op, const char* warnOp) {
		if (EP_UNLIKELY(ShouldWarn(id, id))) {
			WarnSet(id, value, warnOp);
		}

		if (id <= 0) {
			return _defaultValue;
		}
		auto& storage = this->GetStorageForEdit();
		storage.prepare(id, id);

		if constexpr (std::is_same<V, bool>::value) {
			bool v = storage[id];
			AssignOp(v, op(v, value));
			storage[id] = v;
			return v;
		} else {
			V& v = storage[id];
			AssignOp(v, op(v, value));
			return v;
		}
	}

	void PrepareRange(const int first_id, const int last_id) {
		if constexpr (storage_mode == VarStorage::eStorageMode_Vector) {
			auto& storage = this->GetStorageForEdit();
			storage.prepare(first_id, last_id);
		}
	}

	void PerformRangeOperation(const int first_id, const int last_id, const V value, op_Func&& op) {
		return PerformRangeOperationD(first_id, last_id, [&value]() { return value; }, std::forward<op_Func>(op));
	}

	template<typename F>
	void PerformRangeOperationD(const int first_id, const int last_id, F&& value, op_Func&& op) {
		auto& storage = this->GetStorageForEdit();
		for (int i = std::max(1, first_id); i <= last_id; ++i) {
			if constexpr (std::is_same<V, bool>::value) {
				bool v = storage[i];
				AssignOp(v, op(v, value()));
				storage[i] = v;
			} else {
				V& v = storage[i];
				AssignOp(v, op(v, value()));
			}
		}
	}

	void AssignOp(V& target, V value) const;

	auto FormatLValue(int first_id, int last_id) const {
		return last_id == 0
			? fmt::format("{}[{}]", VarStorage::TypeToStr(_type), first_id)
			: fmt::format("{}[{},{}]", VarStorage::TypeToStr(_type), first_id, last_id);
	}

	auto FormatRValue(V v, const char* operandType = nullptr) const {
		//special handling for string storage
		if constexpr (std::is_same<V, std::string>::value) {
			auto str = static_cast<std::string>(v);
			if (str.length() > 8)
				v = str.substr(0, 5) + "...";
		}

		if (operandType == nullptr) {
			return fmt::format("{}", v);
		}
		return fmt::format("{}[{}]", operandType, v);
	}

	int _type = -1;
	mutable int _warnings = kMaxWarnings;
private:
	using parent_id_Func = int(*)(const int id);

	V _defaultValue = 0;
};

template <typename T, typename V>
template<DataScopeType S, typename C>
inline void Game_VectorDataStorageBase<T, V>::SetData(std::vector<V> data) {
	static_assert(static_cast<int>(S) < DynamicScope::count_global_scopes);

	if constexpr (std::is_same<C, V>::value) {
		_globals[static_cast<int>(S)].setData(std::move(data));
	} else {
		std::vector<V> data_conv;
		data_conv.resize(data.size());
		for (int i = 0; i < data.size(); i++)
			data_conv = static_cast<V>(data[i]);
		_globals[static_cast<int>(S)].setData(std::move(data_conv));
	}
}

template <typename T, typename V>
template<DataScopeType S, typename C>
inline std::vector<V> Game_VectorDataStorageBase<T, V>::GetData() const {
	static_assert(static_cast<int>(S) < DynamicScope::count_global_scopes);

	if constexpr (std::is_same<C, V>::value) {
		return _globals[static_cast<int>(S)].getData();
	} else {
		auto data = _globals[static_cast<int>(S)].getData();
		auto data_conv = std::vector<C>(data.size());
		for (int i = 0; i < data.size(); i++)
			data[i] = static_cast<C>(data[i]);
		return data;
	}
}

template <typename T, typename V>
template<DataScopeType S, typename C>
inline void Game_MixedDataStorageBase<T, V>::SetData(std::vector<V> data) {
	static_assert(static_cast<int>(S) < DynamicScope::count_global_scopes);

	if constexpr (std::is_same<C, V>::value) {
		_globals[static_cast<int>(S)].setData(std::move(data));
	} else {
		std::vector<V> data_conv;
		data_conv.resize(data.size());
		for (int i = 0; i < data.size(); i++)
			data_conv = static_cast<V>(data[i]);
		_globals[static_cast<int>(S)].setData(std::move(data_conv));
	}
}

template <typename T, typename V>
template<DataScopeType S, typename C>
inline std::vector<V> Game_MixedDataStorageBase<T, V>::GetData() const {
	static_assert(static_cast<int>(S) < DynamicScope::count_global_scopes);

	if constexpr (std::is_same<C, V>::value) {
		return _globals[static_cast<int>(S)].getData();
	} else {
		auto data = _globals[static_cast<int>(S)].getData();
		auto data_conv = std::vector<C>(data.size());
		for (int i = 0; i < data.size(); i++)
			data[i] = static_cast<C>(data[i]);
		return data;
	}
}



/* CRTP for inlining the otherwise 'virtual' member function "AssignOp" */

class Game_SwitchesBase : public Game_DataStorage<Game_SwitchesBase, lcf::rpg::SaveScopedSwitchData, game_bool, VarStorage::eStorageMode_Vector> {
public:
	using Var_t = game_bool;

	inline Game_SwitchesBase()
		: Game_DataStorage<Game_SwitchesBase, lcf::rpg::SaveScopedSwitchData, game_bool, VarStorage::eStorageMode_Vector>(VarStorage::DataStorageType::eStorageType_Variable) {
	}

private:
	void AssignOpImpl(game_bool& target, game_bool value) const;

	friend class Game_DataStorage<Game_SwitchesBase, lcf::rpg::SaveScopedSwitchData, game_bool, VarStorage::eStorageMode_Vector>;
};

class Game_VariablesBase : public Game_DataStorage<Game_VariablesBase, lcf::rpg::SaveScopedVariableData, int32_t, VarStorage::eStorageMode_Vector> {
public:
	using Var_t = int32_t;

	inline Game_VariablesBase(Var_t minval, Var_t maxval)
		: Game_DataStorage<Game_VariablesBase, lcf::rpg::SaveScopedVariableData, int32_t, VarStorage::eStorageMode_Vector>(VarStorage::DataStorageType::eStorageType_Variable),
		_min(minval), _max(maxval) {
		if (minval >= maxval) {
			Output::Error("Variables: Invalid var range: [{}, {}]", minval, maxval);
		}
	}

	inline Var_t GetMaxValue() const { return _max; };
	inline Var_t GetMinValue() const {	return _min; }

private:
	void AssignOpImpl(Var_t& target, Var_t value) const;

	Var_t _min = 0;
	Var_t _max = 0;

	friend class Game_DataStorage<Game_VariablesBase, lcf::rpg::SaveScopedVariableData, int32_t, VarStorage::eStorageMode_Vector>;
};

template class Game_DataStorage<Game_SwitchesBase, lcf::rpg::SaveScopedSwitchData, game_bool, VarStorage::eStorageMode_Vector>;
template class Game_DataStorage<Game_VariablesBase, lcf::rpg::SaveScopedVariableData, int32_t, VarStorage::eStorageMode_Vector>;

template <typename D, typename T, typename V, VarStorage::DataStorageMode storage_mode>
inline void Game_DataStorage<D, T, V, storage_mode>::AssignOp(V& target, V value) const {
	reinterpret_cast< const D&>(*this).AssignOpImpl(target, value);
}

#endif
