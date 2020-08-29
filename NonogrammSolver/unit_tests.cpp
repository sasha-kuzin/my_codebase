#pragma once
#include "Tests.h"
#include "parser.h"
using namespace std;

void TestDecisionBuilder();
void TestParser();
void TestCryptedPositions();
void TestSolverBuild();
void TestSolverRun();

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestDecisionBuilder, "Test Decison Builder");
	tr.RunTest(TestParser, "Test Parser");
	tr.RunTest(TestCryptedPositions, "Test Crypted Position");
	tr.RunTest(TestSolverBuild, "Test Solver Build");
	tr.RunTest(TestSolverRun, "Test Solver Run");
}