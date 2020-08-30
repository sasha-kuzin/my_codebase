#include "Tests.h"
#include "parser.h"
#include "solver.h"

#include <variant>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

void TestAll();


void Demonstration(istream& in, ostream& out);
void MainAction(istream& in, ostream& out);

void OnlySolverDemonstration(istream& in, ostream& out);

int main() {
	TestAll();
#ifdef _DEBUG
	ifstream input("numbers.in");
	Demonstration(input, cout);
	OnlySolverDemonstration(input, cout);
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
				out << solution.value()[i] << endl;
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

vector<vector <int>> ReadMatrix(istream& in, size_t sz) {
	vector<vector<int>> result(sz);
	for (int i = 0; i < sz; ++i) {
		int vect_size;
		in >> vect_size;
		result[i].resize(vect_size);
		for (auto& e : result[i]) {
			in >> e;
		}
	}
	return move(result);
}

string PrintVector(size_t max_size, const vector<int>& vect) {
	stringstream result;
	for (auto e : vect) {
		result << fixed << setw(3) << e << " ";
	}
	for (auto j = 0; j < (max_size - vect.size()); ++j) {
		result << "    ";
	}
	result << ": ";
	return result.str();
}
void OnlySolverDemonstration(istream& in, ostream& out) {
	int test_count;
	in >> test_count;

	for(auto counter = 1; counter <= test_count; ++counter) {
		out << "Test #" << counter << endl;
		size_t size;
		in >> size;
		vector<vector<int>> horizontal = ReadMatrix(in, size);
		vector<vector<int>> vertical = ReadMatrix(in, size);

		auto GetMaxLength = [](const vector<vector<int>>& v) {
			size_t mx = 0;
			for (const auto& e : v) {
				mx = max(mx, e.size());
			}
			return mx;
		};
		int max_elements_count = GetMaxLength(horizontal);

		try {
			auto solution = Solver::Solver(size, horizontal, vertical).Solve().GetSolution();
			if (solution.has_value()) {
				out << "Solution:" << endl;
				for (auto i = 0; i < size; ++i){
					out << PrintVector(max_elements_count, horizontal[i]) << solution.value()[i] << endl;
				}
			}
			else {
				out << "No solution" << endl;
			}

		}
		catch (invalid_argument e) {
			out << e.what() << endl;
		}
	}
}