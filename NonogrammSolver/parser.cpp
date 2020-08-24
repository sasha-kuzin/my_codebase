#pragma once
#include "parser.h"

namespace Parser {

	bool ValidateBoard(const std::vector<std::string>& board) {
		bool checker = true;

		if (board.size() == 0) {
			return false;
		}
		else {
			auto sz = board.size();
			for (int i = 0; i < sz; ++i) {
				if (board[i].size() != sz) {
					checker =  false;
				}
				for (auto elem : board[i]) {
					checker = (checker && (elem == '.' || elem == '*'));
				}
			}
		}

		return checker;
	}

	DecisionBase::Matrix<bool> ConvertBoardFromStrings(const std::vector<std::string> strings, char sym_true) {
		DecisionBase::Matrix<bool> result;
		result.reserve(strings.size());
		for (int i = 0; i < result.size(); ++i) {
			result[i].reserve(strings.size());
			for (char elem : strings[i]) {
				result[i].push_back(elem == sym_true);
			}
		}
		return move(result);
	}

	ParserResponse::ParserResponse(const std::vector<std::string>& game_board, const std::vector<std::string>& mask) {
		error = ErrorMsg::OK;
		if (ValidateBoard(game_board) == false) {
			error = ErrorMsg::INVALID_GAMEBOARD;
		}
		else if (ValidateBoard(mask) == false) {
			error = ErrorMsg::INVALID_MASK;
		}
		else if (game_board.size() != mask.size()) {
			error = ErrorMsg::COMPOSE_ERROR;
		}

		if (error == ErrorMsg::OK) {
			decision_base = DecisionBase::DecisionBaseBuilder()
				.SetSize(mask.size())
				.SetGameBoard(DecisionBase::ConvertBoardFromStrings(game_board, '*'))
				.SetMask(DecisionBase::ConvertBoardFromStrings(mask, '*'))
				.Build();
		}
		else {
			decision_base = std::nullopt;
		}
	}

}