#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;


void TestFullCrypted();
void TestEmptyCrypted();
void TestUsualCrypted();

void TestCryptedPositions() {
	TestFullCrypted();
	TestEmptyCrypted();
	TestUsualCrypted();
}

void TestFullCrypted() {
	vector <string> game_board = {
		"****",
		"****",
		"****",
		"****"
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	DecisionBase::CryptedPosition config = base.BuildCryptedPosition();
	const vector <vector<int>> checker = { {4},{4},{4},{4} };
	ASSERT_EQUAL(config.size, static_cast<size_t>(4));
	ASSERT_EQUAL(config.rows, checker);
	ASSERT_EQUAL(config.columns, checker);
}

void TestEmptyCrypted() {
	vector <string> game_board = {
		"....",
		"....",
		"....",
		"...."
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	DecisionBase::CryptedPosition config = base.BuildCryptedPosition();
	const vector <vector<int>> checker = { {0},{0},{0},{0} };
	ASSERT_EQUAL(config.size, static_cast<size_t>(4));
	ASSERT_EQUAL(config.rows, checker);
	ASSERT_EQUAL(config.columns, checker);
}

void TestUsualCrypted() {
	vector <string> game_board = {
		"*.*.",
		".*.*",
		"**.*",
		".***"
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	DecisionBase::CryptedPosition config = base.BuildCryptedPosition();
	const vector <vector<int>> checker = { {1,1},{1,1},{2,1},{3} };
	const vector<vector<int>> vertical_checker = { {1,1}, {3}, {1,1}, {3} };
	ASSERT_EQUAL(config.size, static_cast<size_t>(4));
	ASSERT_EQUAL(config.rows, checker);
	ASSERT_EQUAL(config.columns, vertical_checker);
}