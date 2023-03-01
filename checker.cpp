#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define fin freopen("ast.txt", "r", stdin)

using namespace std;

// Structs

struct Line {
    int program_line_number{}; // number of line in the initial program
    int number{}; // <number> that specify the order of execution
    string ass_var_id; // <command> ::= <var> := <expression> -> this is <var>
    vector <string> other_var_ids; // <var>s in expression
};

// Helper Functions

vector <string> split(const string& line) {
    vector <string> res;
    string word;
    for (auto c : line) {
        if (c == '-' || c == ' ') {
            if (!word.empty())
                res.push_back(word);

            word = "";
            continue;
        }

        word += c;
    }

    if (!word.empty())
        res.push_back(word);

    return res;
}

vector <Line> parse_into_lines(vector <vector<string>> input) {
    vector <Line> res;

    auto line = Line();
    int line_counter = 1;
    for (int i = 1; i < input.size(); ++i) {
        auto tokens = input[i];
        if (tokens[0] == "LINE" && i == 1) {
            line.number = stoi(tokens[1]);
            line.program_line_number = line_counter++;
            continue;
        } else if (tokens[0] == "LINE") {
            res.push_back(line);

            line = Line();
            line.number = stoi(tokens[1]);
            line.program_line_number = line_counter++;

            continue;
        }

        if (tokens[0] == "IDENTIFIER" && input[i - 1][0] == "ASSIGNMENT") {
            line.ass_var_id = tokens[1];
            continue;
        }

        if (tokens[0] == "IDENTIFIER") {
            line.other_var_ids.push_back(tokens[1]);
            continue;
        }
    }

    res.push_back(line);
    return res;
}

vector <vector<string>> read_file() {
    vector <vector<string>> res;

    string line;
    while (getline(cin, line)) {
        res.push_back(split(line));
    }

    return res;
}

// Checkers

bool check_lines_numbers_uniqueness(const vector <Line>& lines) {
    unordered_map<int, vector<int>> mp;
    for (const auto& line : lines)
        mp[line.number].push_back(line.program_line_number);

    bool ok = true;
    for (auto i : mp) {
        if (i.second.size() == 1)
            continue;

        cout << "ERR: Lines ";
        for (auto j : i.second)
            cout << j << " ";
        cout << "have the same number\n";
        ok = false;
    }

    return ok;
}

bool lines_comp(const Line& a, const Line& b) {
    return a.number < b.number;
}

bool check_var_assignment_correctness(vector <Line> lines) {
    sort(lines.begin(), lines.end(), lines_comp);

    bool ok = true;

    // Recursive assignments
    for (const auto& line : lines) {
        bool temp_ok = true;
        for (const auto& v : line.other_var_ids) {
            if (v == line.ass_var_id) {
                temp_ok = false;
                break;
            }
        }

        if (!temp_ok) {
            cout << "ERR: Recursive assignment of " << line.ass_var_id << " in line " << line.program_line_number << "\n";
        }

        ok = ok && temp_ok;
    }

    // Variables are assigned before they are used
    unordered_map <string, bool> assigned;
    unordered_map <string, vector <int>> errors;

    for (const auto& line : lines) {
        assigned[line.ass_var_id] = true;

        for (const auto& v : line.other_var_ids) {
            if (!assigned[v])
                errors[v].push_back(line.program_line_number);
        }
    }

    ok = ok && errors.empty();
    for (auto i : errors) {
        cout << "ERR: Usage before the assignment of " << i.first << " in lines ";
        for (auto j : i.second)
            cout << j << " ";
        cout << "\n";
    }

    return ok;
}

// main

int main() {
    fin; // enable file read
    auto res = read_file();
    auto lines = parse_into_lines(res);
    
    // Tests
    auto ok = check_lines_numbers_uniqueness(lines);
    if (!ok)
        return 0;

    ok = check_var_assignment_correctness(lines);
    if (!ok)
        return 0;

    cout << "Completed static analysis:\n\tNo errors";
    return 0;
}