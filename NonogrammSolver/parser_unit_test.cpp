#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;


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
) {
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