#pragma once
#include <vector>
#include <string>

namespace DecisionBase {
	template <typename T>
	using Matrix = std::vector<std::vector<T>>;

	struct CryptedPosition {
		size_t size;
		Matrix<int> rows;
		Matrix<int> columns;
		CryptedPosition() = default;
		CryptedPosition(size_t);
		void CutZeroesAndShrink();
	};

	class DecisionBase {
		size_t size;
		Matrix<bool> game_board;
		Matrix<bool> mask;

	public:
		DecisionBase() = default;
		DecisionBase(std::size_t, const Matrix<bool>&, const Matrix<bool>&) = delete;
		DecisionBase(std::size_t, Matrix<bool>&&, Matrix<bool>&& );
		CryptedPosition BuildCryptedPosition() const;
		const Matrix<bool>& GetBoard();
		std::vector<std::string> PrintBoard();

		Matrix<int> GetMaskedBoard() const;
		std::vector<std::string> PrintMaskedBoard() const;

		size_t GetSize() const;
	private:
		template <typename T, typename Container>
		std::vector<Container> CombineBoardAndMask(T filled, T empty, T undefined) const {
			std::vector<Container> result(size);
			for (auto i = 0; i < size; ++i) {
				result[i].reserve(size);
				for (auto j = 0; j < size; ++j) {
					if (mask[i][j]) {
						result[i].push_back(game_board[i][j] ? filled : empty);
					}
					else {
						result[i].push_back(undefined);
					}
				}
			}

			return move(result);
		}
	};

	class DecisionBaseBuilder {
		size_t size;
		Matrix<bool> game_board;
		Matrix<bool> mask;
	public:
		DecisionBaseBuilder& SetSize(std::size_t);
		DecisionBaseBuilder& SetGameBoard(const Matrix<bool>&);
		DecisionBaseBuilder& SetMask(const Matrix<bool>&);

		DecisionBaseBuilder& SetGameBoard(Matrix<bool>&&);
		DecisionBaseBuilder& SetMask(Matrix<bool>&&);

		DecisionBase Build();
	};


}