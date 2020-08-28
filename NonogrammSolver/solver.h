#pragma once
#include <vector>
#include <set>
#include <string>
#include <optional>
#include <algorithm>
#include "decision_base.h"

namespace Solver {
	template <typename T>
	using Matrix = std::vector<std::vector<T>>;

	enum class Dimension {
		horizontal,
		vertical
	};

	class Solver {

		struct Row {
			std::vector<int> figures_lengths;
			std::vector<int> game_row;
			std::set<int> solutions;
			Row() = default;
			Row(size_t length, std::vector <int> game_row, const std::vector<int>& figures_lengths);
		};
		std::vector<Row> horizontal_solver;
		std::vector<Row> vertical_solver;

		std::optional<Matrix<bool>> solution;
		size_t size;
		
	public:
		Solver(const DecisionBase::DecisionBase& base);
		Solver& Solve();

		//this function only for testing
		bool CompareSolutionsSet(Dimension dimension, size_t index, const std::set<int>& solutions) const;
	};

}