#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;

void TestDecisionBuilder();
void TestParser();
void TestCryptedPositions();
void TestSolverBuild();

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestDecisionBuilder, "Test Decison Builder");
	tr.RunTest(TestParser, "Test Parser");
	tr.RunTest(TestCryptedPositions, "Test Crypted Position");
	tr.RunTest(TestSolverBuild, "Test Solver Build");
}