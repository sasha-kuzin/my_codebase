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
	}

	bool Solver::CompareSolutionsSet(Dimension dimension, size_t index, const std::set<int>& solutions) const{
		return (dimension == Dimension::horizontal ? 
			horizontal_solver[index].solutions == solutions : 
			vertical_solver[index].solutions == solutions);
	}
}