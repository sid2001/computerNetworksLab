#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

using namespace std;

// Function to compute the First set
void computeFirst(const map<char, vector<string>>& productions, map<char, set<char>>& first, char nonTerminal) {
    if (first[nonTerminal].size() > 0) return;  // First set already computed

    for (const string& production : productions.at(nonTerminal)) {
        for (char symbol : production) {
            if (isupper(symbol)) {
                // Symbol is a non-terminal
                computeFirst(productions, first, symbol);
                first[nonTerminal].insert(first[symbol].begin(), first[symbol].end());
                if (first[symbol].count('#') == 0)  // '#' represents epsilon
                    break;
            } else {
                // Symbol is a terminal
                first[nonTerminal].insert(symbol);
                break;
            }
        }
    }
}

// Function to compute the Follow set
void computeFollow(const map<char, vector<string>>& productions, map<char, set<char>>& first, map<char, set<char>>& follow, char nonTerminal, char startSymbol) {
    if (follow[nonTerminal].size() > 0) return;  // Follow set already computed

    if (nonTerminal == startSymbol) {
        follow[nonTerminal].insert('$');  // '$' represents end of input
    }

    for (const auto& prod : productions) {
        char lhs = prod.first;
        for (const string& rhs : prod.second) {
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (rhs[i] == nonTerminal) {
                    if (i + 1 < rhs.size()) {
                        char nextSymbol = rhs[i + 1];
                        if (isupper(nextSymbol)) {
                            // Next symbol is a non-terminal
                            follow[nonTerminal].insert(first[nextSymbol].begin(), first[nextSymbol].end());
                            follow[nonTerminal].erase('#');
                            if (first[nextSymbol].count('#')) {
                                computeFollow(productions, first, follow, lhs, startSymbol);
                                follow[nonTerminal].insert(follow[lhs].begin(), follow[lhs].end());
                            }
                        } else {
                            // Next symbol is a terminal
                            follow[nonTerminal].insert(nextSymbol);
                        }
                    } else {
                        // It's the last symbol in the production
                        if (lhs != nonTerminal) {
                            computeFollow(productions, first, follow, lhs, startSymbol);
                            follow[nonTerminal].insert(follow[lhs].begin(), follow[lhs].end());
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int numNonTerminals, numTerminals;
    cout << "Enter the number of non-terminals: ";
    cin >> numNonTerminals;

    cout << "Enter the number of terminals: ";
    cin >> numTerminals;

    vector<char> nonTerminals(numNonTerminals);
    vector<char> terminals(numTerminals);

    cout << "Enter the non-terminals (separated by space): ";
    for (int i = 0; i < numNonTerminals; ++i) {
        cin >> nonTerminals[i];
    }

    cout << "Enter the terminals (separated by space): ";
    for (int i = 0; i < numTerminals; ++i) {
        cin >> terminals[i];
    }

    map<char, vector<string>> productions;
    cout << "Enter the production rules (format: A->BCD):\n";
    string production;
    cin.ignore();
    while (getline(cin, production) && !production.empty()) {
        char lhs = production[0];
        string rhs = production.substr(3);  // Ignore "->"
        productions[lhs].push_back(rhs);
    }

    // Compute First and Follow sets
    map<char, set<char>> first;
    map<char, set<char>> follow;

    // Compute First sets for all non-terminals
    for (char nonTerminal : nonTerminals) {
        computeFirst(productions, first, nonTerminal);
    }

    // Compute Follow sets for all non-terminals
    char startSymbol = nonTerminals[0];  // Assume the first non-terminal is the start symbol
    for (char nonTerminal : nonTerminals) {
        computeFollow(productions, first, follow, nonTerminal, startSymbol);
    }

    // Output the First and Follow sets
    for (char nonTerminal : nonTerminals) {
        cout << "Non Terminal: " << nonTerminal << "\n";
        cout << "First(" << nonTerminal << "): { ";
        for (char symbol : first[nonTerminal]) {
            cout << symbol << " ";
        }
        cout << "}\n";
        cout << "Follow(" << nonTerminal << "): { ";
        for (char symbol : follow[nonTerminal]) {
            cout << symbol << " ";
        }
        cout << "}\n";
        cout << endl;
    }

    return 0;
}
