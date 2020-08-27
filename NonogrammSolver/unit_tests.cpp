#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;

void TestDecisionBuilder();

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestDecisionBuilder, "Test Decison Builder");
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

