#pragma once
#include <vector>
#include <set>
#include <string>
#include <optional>
#include <algorithm>
#include <stdexcept>
#include "decision_base.h"

namespace Solver {
	template <typename T>
	using Matrix = std::vector<std::vector<T>>;

	enum class Dimension {
		horizontal,
		vertical
	};

	enum class Status {
		INIT,
		IN_PROCESS,
		NO_SOLUTION,
		SOLVED
	};

	class Solver {

		struct Row {
			std::vector<int> figures_lengths;
			std::vector<int> game_row;
			std::set<int> solutions;
			Row() = default;
			Row(size_t length, std::vector <int> game_row, const std::vector<int>& figures_lengths);
			bool DistillateSolutions();
			std::vector<bool> AsBooleanRow() const;
		};
		std::vector<Row> horizontal_solver;
		std::vector<Row> vertical_solver;

		std::optional<Matrix<bool>> solution;
		size_t size;
		Status status;
		
	public:
		Solver(const DecisionBase::DecisionBase& base);
		Solver& Solve();

		Status GetSolutionStatus() const;
		std::optional<std::vector<std::string>> GetSolution() const;

		//this function only for testing
		bool CompareSolutionsSet(Dimension dimension, size_t index, const std::set<int>& solutions) const;
	private:
		Status RunSolveIteration();
		bool CheckUniqueSolution();
	};

}