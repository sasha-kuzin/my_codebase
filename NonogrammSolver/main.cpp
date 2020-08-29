#include "Tests.h"
#include "parser.h"
#include "solver.h"

#include <variant>
#include <iostream>
#include <fstream>
using namespace std;

void TestAll();


void Demonstration(istream& in, ostream& out);
void MainAction(istream& in, ostream& out);

int main() {
	TestAll();
#ifdef _DEBUG
	ifstream input("numbers.in");
	Demonstration(input, cout);
#endif
	return 0;
}

void Demonstration(istream& in, ostream& out) {
	int test_count;
	in >> test_count;
	while (test_count--) {
		MainAction(in, out);
	}
}

vector<string> ParseBoard(istream& in, size_t size) {
	vector<string> game_board;
	game_board.reserve(size);
	for (auto i = 0; i < size; ++i) {
		string buf;
		while (buf.length() == 0) {
			getline(in, buf);
		}
		game_board.push_back(buf);
	}
	return move(game_board);
}

void MainAction(istream& in, ostream& out) {
	size_t size;
	in >> size;
	vector<string> game_board = ParseBoard(in, size);
	vector<string> mask = ParseBoard(in, size);

	Parser::ParserResponse base(game_board, mask);
	if (base.error == Parser::ErrorMsg::OK) {
		auto solution = Solver::Solver(base.decision_base.value()).Solve().GetSolution();
		if (solution.has_value()) {
			out << "Solution:" << endl;
			for (auto i = 0; i < size; ++i) {
				cout << solution.value()[i] << endl;
			}
		}
		else {
			out << "No solution" << endl;
		}
	}
	else {
		out << base.error << endl;
	}
}