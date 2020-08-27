#pragma once
#include "decision_base.h"
#include <optional>

namespace Parser{
	enum class ErrorMsg {
		OK = 0,
		INVALID_GAMEBOARD = 1,
		INVALID_MASK = 2,
		COMPOSE_ERROR = 3,
		INVALID_SIZE = 4
	};
	struct ParserResponse {
		ErrorMsg error;
		std::optional <DecisionBase::DecisionBase> decision_base; 
		explicit ParserResponse(const std::vector<std::string>& game_board, const std::vector<std::string>& mask);
	};
}