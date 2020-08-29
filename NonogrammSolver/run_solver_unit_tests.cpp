#pragma once
#include "Tests.h"
#include "solver.h"
#include "parser.h"

void TestFullRun();
void TestEmptyRun();
void TestUsualRun();
void TestNoSolutionRun();

void TestSolverRun() {
	TestFullRun();
	TestEmptyRun();
	TestUsualRun();
	TestNoSolutionRun();
}

void TestFullRun() {
	vector <string> game_board = {
		"****",
		"****",
		"****",
		"****"
	};
	vector <string> mask = game_board;
	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	Solver::Status result = solver.Solve().GetSolutionStatus();
	ASSERT(result == Solver::Status::SOLVED);
}

void TestEmptyRun() {
	vector <string> game_board = {
		"....",
		"....",
		"....",
		"...."
	};
	vector <string> mask = game_board;

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	Solver::Status result = solver.Solve().GetSolutionStatus();
	ASSERT(result == Solver::Status::SOLVED);
}

void TestUsualRun() {
	vector <string> game_board = {
		"..******..",
		".********.",
		"**....****",
		"**...*****",
		"**..***.**",
		"**.***..**",
		"*****...**",
		"****....**",
		".********.",
		"..******.."
	};
	vector <string> mask = {
		"..........",
		"..........",
		"**........",
		"..........",
		"..........",
		"..........",
		"..........",
		".........*",
		"..........",
		".........."
	};

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	Solver::Status result = solver.Solve().GetSolutionStatus();
	ASSERT(result == Solver::Status::SOLVED);
}

void TestNoSolutionRun() {
	vector <string> game_board = {
		"..******..",
		".********.",
		"**....****",
		"**...*****",
		"**..***.**",
		"**.***..**",
		"*****...**",
		"****....**",
		".********.",
		"..******.."
	};
	vector <string> mask = {
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
		"..........",
		".........."
	};

	DecisionBase::DecisionBase base = Parser::ParserResponse(game_board, mask).decision_base.value();
	Solver::Solver solver(base);

	Solver::Status result = solver.Solve().GetSolutionStatus();
	ASSERT(result == Solver::Status::NO_SOLUTION);
}