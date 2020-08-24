#include "decision_base.h"

namespace DecisionBase {
	DecisionBase::DecisionBase(std::size_t size, Matrix<bool>&& game_board, Matrix<bool>&& mask):
		size(size),
		game_board(std::move(game_board)),
		mask(std::move(mask))
	{}

	CryptedPosition DecisionBase::BuildCryptedPosition() const {
		
		CryptedPosition result(size);

		for (auto i = 0; i < size; ++i) {
			for (auto j = 0; j < size; ++j) {
				if (game_board[i][j]) {
					result.rows[i].back()++;
					result.columns[j].back()++;
				}
				else {
					if (result.rows[i].back()) {
						result.rows[i].push_back(0);
					}
					if (result.columns[j].back()) {
						result.columns[j].push_back(0);
					}
				}
			}
		}

		result.CutZeroesAndShrink();
		return std::move(result);
	}

	const Matrix<bool>& DecisionBase::GetBoard() {
		return game_board;
	}

	std::vector<std::string> DecisionBase::PrintBoard() {
		std::vector<std::string> result;
		result.resize(size);
		for (auto i = 0; i < size; ++i) {
			result[i].reserve(size);
			for (auto elem : game_board[i]) {
				result[i].push_back(elem ? '*' : '.');
			}
		}
		return std::move(result);
	}

	DecisionBaseBuilder& DecisionBaseBuilder::SetSize(std::size_t size) {
		this->size = size;
		return *this;
	}
	DecisionBaseBuilder& DecisionBaseBuilder::SetGameBoard(const Matrix<bool>& game_board) {
		this->game_board = game_board;
		return *this;
	}
	DecisionBaseBuilder& DecisionBaseBuilder::SetMask(const Matrix<bool>& mask) {
		this->mask = mask;
		return *this;
	}
	DecisionBase DecisionBaseBuilder::Build() {
		return { size, move(game_board), move(mask) };
	}

	DecisionBaseBuilder& DecisionBaseBuilder::SetGameBoard(Matrix<bool>&& game_board) {
		this->game_board = std::move(game_board);
		return *this;
	}
	DecisionBaseBuilder& DecisionBaseBuilder::SetMask(Matrix<bool>&& mask) {
		this->mask = move(mask);
		return *this;
	}


	CryptedPosition::CryptedPosition(size_t size) :
		size(size) {
		columns.reserve(size);
		rows.reserve(size);

		for (std::size_t i = 0; i < size; ++i) {
			rows[i].reserve(size);
			columns[i].reserve(size);

			rows[i].push_back(0);
			columns[i].push_back(0);
		}
	}

	template <typename T>
	void PopValuesAndShrink(std::vector<T> vect, const T& value = T()) {
		if (vect.back() == value) {
			vect.pop_back();
		}
		vect.shrink_to_fit();
	}

	void CryptedPosition::CutZeroesAndShrink() {
		for (std::size_t i = 0; i < size; ++i) {
			PopValuesAndShrink(rows[i]);
			PopValuesAndShrink(columns[i]);
		}
	}


	Matrix<int> DecisionBase::GetMaskedBoard() const {
		return CombineBoardAndMask<int, std::vector<int>>(1, 0, -1);
	}

	std::vector<std::string> DecisionBase::PrintMaskedBoard() const{
		return CombineBoardAndMask<char, std::string>('*', '.', '?');
	}
}