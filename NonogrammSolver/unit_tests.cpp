#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;

void TestDecisionBuilder();
void TestParser();

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestDecisionBuilder, "Test Decison Builder");
	tr.RunTest(TestParser, "Test Parser");
}

void BuildDecisionBaseAndTest(
	size_t size,
	DecisionBase::Matrix<bool> game_board,
	DecisionBase::Matrix<bool> mask,
	DecisionBase::Matrix<int> combined_board,
	vector<string> printed_board
) {
	DecisionBase::DecisionBase base =
		DecisionBase::DecisionBaseBuilder().
		SetSize(size).
		SetGameBoard(game_board).
		SetMask(mask).
		Build();

	{
		auto result = base.GetMaskedBoard();
		ASSERT_EQUAL(result, combined_board);
	}
	{
		auto result = base.PrintMaskedBoard();
		ASSERT_EQUAL(result, printed_board);
	}
}

void TestUsualDecision() {
	size_t size = 3;
	DecisionBase::Matrix<bool> game_board = {
		{true, false, true},
		{false, true, false},
		{true, false, true},
	};

	DecisionBase::Matrix<bool> mask = {
		{true, true, true},
		{true, true, true},
		{true, true, true},
	};

	DecisionBase::Matrix<int> combined_position = {
			{1,0,1},
			{0,1,0},
			{1,0,1}
	};

	vector<string> printed_position = {
			"*.*",
			".*.",
			"*.*"
	};

	BuildDecisionBaseAndTest(
		size,
		game_board,
		mask,
		combined_position,
		printed_position
	);
}

void TestMaskedDecision() {
	size_t size = 3;
	DecisionBase::Matrix<bool> game_board = {
		{true, false, true},
		{false, true, false},
		{true, false, true},
	};

	DecisionBase::Matrix<bool> mask = {
		{false, true, false},
		{false, true, false},
		{false, true, false},
	};

	DecisionBase::Matrix<int> combined_position = {
			{-1,0,-1},
			{-1,1,-1},
			{-1,0,-1}
	};

	vector<string> printed_position = {
			"?.?",
			"?*?",
			"?.?"
	};

	BuildDecisionBaseAndTest(
		size,
		game_board,
		mask,
		combined_position,
		printed_position
	);
}

void TestDecisionBuilder() {
	TestUsualDecision();
	TestMaskedDecision();
}

void RunParserAndTest(
	const vector<string>& game_board,
	const vector <string>& mask,
	variant<vector<string>, Parser::ErrorMsg> checker
);

void TestParserCorrect();
void TestParserIncorrect();

void TestParser() {
	TestParserCorrect();
	TestParserIncorrect();
}

void TestParserCorrect() {
	vector <string> game_board = {
		"***",
		"***",
		"***"
	};
	vector <string> mask = game_board;
	vector <string> checker = game_board;
	RunParserAndTest(game_board, mask, checker);

	mask = {
		"...",
		"...",
		"..."
	};
	checker = {
		"???",
		"???",
		"???"
	};
	RunParserAndTest(game_board, mask, checker);

	game_board = {
		"*.*",
		"***",
		".*."
	};
	mask = {
		"***",
		"...",
		"*.*"
	};
	checker = {
		"*.*",
		"???",
		".?."
	};
	RunParserAndTest(game_board, mask, checker);
}

void TestInvalidGameboard();
void TestInvalidMask();
void TestInvalidSize();
void TestComposeError();

void TestParserIncorrect() {
	TestInvalidGameboard();
	TestInvalidMask();
	TestInvalidSize();
	TestComposeError();
}

void RunParserAndTest(
	const vector<string>& game_board,
	const vector <string>& mask,
	variant<vector<string>, Parser::ErrorMsg> checker
){
	Parser::ParserResponse response(game_board, mask);
	if (holds_alternative<vector<string>>(checker)) {
		ASSERT(response.decision_base.has_value());
		ASSERT_EQUAL(response.decision_base.value().PrintMaskedBoard(), get<vector<string>>(checker));
	}
	else {
		ASSERT(!(response.decision_base.has_value()));
		ASSERT_EQUAL(response.error, get<Parser::ErrorMsg>(checker));
	}
}

void TestInvalidGameboard() {
	vector <string> game_board = {
		"***"
	};
	vector <string> mask = game_board;
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_GAMEBOARD);

	game_board = {
		"***",
		"**",
		"***"
	};
	mask = game_board;
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_GAMEBOARD);

	game_board = {
		"***",
		"*a*",
		"***"
	};
	mask = game_board;
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_GAMEBOARD);
}

void TestInvalidMask() {
	vector <string> game_board = {
		"***",
		"***",
		"***"
	};
	vector <string> mask = {
		"***",
		"***"
	};
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_MASK);

	game_board = {
		"***",
		"***",
		"***"
	};
	mask = {
		"***",
		"**",
		"***"
	};
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_MASK);

	game_board = {
		"***",
		"***",
		"***"
	};
	mask = {
		"***",
		"*a*",
		"***"
	};
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_MASK);
}

void TestInvalidSize() {
	const int sz = 100;
	vector <string> game_board(sz);
	for (auto& e : game_board) {
		e = string(sz, '*');
	}
	vector <string> mask = game_board;
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::INVALID_SIZE);
}
void TestComposeError() {
	vector <string> game_board = {
		"***",
		"***",
		"***"
	};
	vector <string> mask = {
		"****",
		"****",
		"****",
		"****"
	};
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::COMPOSE_ERROR);

	game_board = {
		"****",
		"****",
		"****",
		"****"
	};
	mask = {
		"***",
		"***",
		"***"
	};
	RunParserAndTest(game_board, mask, Parser::ErrorMsg::COMPOSE_ERROR);
}