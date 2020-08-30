#include "solver.h"

namespace Solver {

	bool ValidateLengths(size_t value, const std::vector<int>& checker) {
		int pos = 0;
		int current = 0;
		while (value > 0) {
			if (value & 1) {
				++current;
			}
			else {
				if (current > 0) {
					if (pos >= checker.size() || checker[pos] != current) {
						return false;
					}
					++pos;
					current = 0;
				}
			}
			value >>= 1;
		}
		if (current > 0) {
			if (pos >= checker.size() || checker[pos] != current) {
				return false;
			}
			++pos;
		}
		else if (pos == 0 && checker.size() == 1 && checker[0] == 0) {
			return true;
		}

		return pos == checker.size();
	}

	bool ValidateSolution(int solution, const std::vector<int>& game_row) {
		for (auto i = 0; i < game_row.size(); ++i) {
			if (game_row[i] == 1) {
				if ((solution & (1 << i)) == 0) {
					return false;
				}
			}
			if (game_row[i] == 0) {
				if ((solution & (1 << i)) != 0) {
					return false;
				}
			}
		}
		return true;
	}

	Solver::Row::Row(size_t length, std::vector<int> game_row, const std::vector<int>& figures_lengths):
	figures_lengths(figures_lengths),
	game_row(move(game_row)){
		size_t limit = (1 << this->game_row.size());
		for (auto i = 0; i < limit; ++i) {
			if (ValidateLengths(i, this->figures_lengths)) {
				solutions.insert(i);
			}
		}
	}

	Solver::Solver(size_t size, Matrix<int> horizontal, Matrix<int> vertical):
	size(size)
	{
		horizontal_solver.reserve(size);
		vertical_solver.reserve(size);
		for (auto i = 0; i < size; ++i) {
			if (std::accumulate(begin(horizontal[i]), end(horizontal[i]), 0) + horizontal[i].size() - 1 > size) {
				throw std::invalid_argument("summary length of the elements larger than gameboard");
			}
			horizontal_solver.push_back({ size, std::vector<int>(size, -1), move(horizontal[i]) });
		}
		for (auto i = 0; i < size; ++i) {
			if (std::accumulate(begin(vertical[i]), end(vertical[i]), 0) + vertical[i].size() - 1 > size) {
				throw std::invalid_argument("summary length of the elements larger than gameboard");
			}
			vertical_solver.push_back({ size, std::vector<int>(size, -1), move(vertical[i]) });
		}
		status = Status::INIT;
	}


	Solver::Solver(const DecisionBase::DecisionBase& base):
	size(base.GetSize())
	{
		DecisionBase::CryptedPosition crypted_position = base.BuildCryptedPosition();
		Matrix<int> game_board = base.GetMaskedBoard();

		auto InvertBoard = [](const Matrix<int>& board) {
			Matrix<int> result = board;
			for (auto i = 0; i < board.size(); ++i) {
				for (int j = 0; j < board.size(); ++j) {
					std::swap(result[i][j], result[j][i]);
				}
			}
			return move(result);
		};
		Matrix<int> inverted_board = InvertBoard(game_board);


		for (auto i = 0; i < size; ++i) {
			horizontal_solver.push_back({ size, move(game_board[i]), crypted_position.rows[i] });
			vertical_solver.push_back({ size, move(inverted_board[i]), crypted_position.columns[i] });
		}
		status = Status::INIT;
	}

	bool Solver::CompareSolutionsSet(Dimension dimension, size_t index, const std::set<int>& solutions) const{
		return (dimension == Dimension::horizontal ? 
			horizontal_solver[index].solutions == solutions : 
			vertical_solver[index].solutions == solutions);
	}

	Solver& Solver::Solve() {
		status = Status::IN_PROCESS;
		while (status == Status::IN_PROCESS) {
			status = RunSolveIteration();
		}

		if (status == Status::SOLVED) {
			solution = Matrix<bool>(size);
			for (auto i = 0; i < size; ++i) {
				solution.value()[i] = horizontal_solver[i].AsBooleanRow();
			}
		}
		return *this;
	}

	std::vector<bool> Solver::Row::AsBooleanRow() const {
		std::vector<bool> result(game_row.size());
		for (auto i = 0; i < game_row.size(); ++i) {
			result[i] = static_cast<bool>(game_row[i]);
		}
		return move(result);
	}

	Status Solver::RunSolveIteration() {
		bool was_modifyed = false;
		for (auto i = 0; i < size; ++i) {
			if (horizontal_solver[i].DistillateSolutions()) {
				was_modifyed = true;
				for (auto j = 0; j < size; ++j) {
					vertical_solver[j].game_row[i] = horizontal_solver[i].game_row[j];
				}
			}
		}

		
		for (auto i = 0; i < size; ++i) {
			if (vertical_solver[i].DistillateSolutions()) {
				was_modifyed = true;
				for (auto j = 0; j < size; ++j) {
					horizontal_solver[j].game_row[i] = vertical_solver[i].game_row[j];
				}
			}
		}

		if (was_modifyed) {
			return Status::IN_PROCESS;
		}
		else {
			return (CheckUniqueSolution() ? Status::SOLVED : Status::NO_SOLUTION);
		}
	}

	bool Solver::CheckUniqueSolution() {
		bool result = true;
		for (auto& elem : horizontal_solver) {
			result = (result && elem.solutions.size() == 1);
		}
		return result;
	}

	bool Solver::Row::DistillateSolutions() {
		std::set<int> old_solutions = move(solutions);
		int colored_mask = (1 << game_row.size()) - 1;
		int empty_mask = 0;
		for (auto solution : old_solutions) {
			if (ValidateSolution(solution, game_row)) {
				solutions.insert(solution);
				colored_mask &= solution;
				empty_mask |= solution;
			}
		}

		bool result_flag = false;
		for (auto i = 0; i < game_row.size(); ++i) {

			int checker = (1 << i);

			if ((colored_mask & checker) == checker) {
				if (game_row[i] == -1) {
					result_flag = true;
					game_row[i] = 1;
				}
				else if (game_row[i] == 0) {
					throw new std::invalid_argument("Gameboard has a conflict");
				}
			}

			if ((empty_mask & checker) == 0) {
				if (game_row[i] == -1) {
					result_flag = true;
					game_row[i] = 0;
				}
				else if (game_row[i] == 1) {
					throw new std::invalid_argument("Gameboard has a conflict");
				}
			}
		}

		return result_flag;
	}

	Status Solver::GetSolutionStatus() const {
		return status;
	}

	std::optional<std::vector<std::string>> Solver::GetSolution() const {
		if (status == Status::SOLVED) {
			std::vector<std::string> result(size);
			for (auto i = 0; i < size; ++i) {
				result[i].reserve(size);
				for (auto elem : solution.value()[i]) {
					result[i].push_back(elem ? '*' : '.');
				}
			}

			return move(result);
		}
		else {
			return std::nullopt;
		}
	}
}