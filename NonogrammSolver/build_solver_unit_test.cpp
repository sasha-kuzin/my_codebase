#pragma once
#include "Tests.h"
#include "solver.h"
#include "parser.h"

void TestFullBuild();
void TestEmptyBuild();
void TestUsualBuild();

void TestSolverBuild() {
	TestFullBuild();
	TestEmptyBuild();
	TestUsualBuild();
}

void TestFullBuild() {
	vector <string> game_board = {
		"****",
		"****",
		"****",
		"****"
	};
	vector <string> mask = game_board;
	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	vector <set<int>> checker = {
		{15}, {15}, {15}, {15}
	};
	for (auto i = 0; i < checker.size(); ++i) {
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::horizontal, i, checker[i]));
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::vertical, i, checker[i]));
	}
}

void TestEmptyBuild() {
	vector <string> game_board = {
		"....",
		"....",
		"....",
		"...."
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	vector <set<int>> checker = {
		{0}, {0}, {0}, {0}
	};
	for (auto i = 0; i < checker.size(); ++i) {
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::horizontal, i, checker[i]));
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::vertical, i, checker[i]));
	}
}

void TestUsualBuild() {
	vector <string> game_board = {
		"*.*.",
		".*.*",
		"**.*",
		".***"
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	vector <set<int>> horizontal_checker = {
		{5,9,10}, {5,9,10}, {11}, {7,14}
	};
	vector <set<int>> vertical_checker = {
		{5,9,10}, {7,14}, {5,9,10}, {7,14}
	};
	for (auto i = 0; i < horizontal_checker.size(); ++i) {
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::horizontal, i, horizontal_checker[i]));
		ASSERT(solver.CompareSolutionsSet(Solver::Dimension::vertical, i, vertical_checker[i]));
	}
}