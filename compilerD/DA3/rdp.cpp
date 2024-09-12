#include <iostream>
#include <string>

using namespace std;

class P {
public:
    P(const string& s) : s(s), i(0) {
        c = s[i];
    }

    void parse() {
        e();
        if (i < s.length()) {
            cout << "Error: unexpected '" << c << "'" << endl;
            return;
        }
        cout << "Parse successful!" << endl;
    }

private:
    string s; // Input string
    size_t i; // Current position in input
    char c;   // Current character

    void next() {
        i++;
        if (i < s.length()) {
            c = s[i];
        } else {
            c = '\0';
        }
    }

    void e() {
        t();
        e1();
    }

    void e1() {
        if (c == '+') {
            next();
            t();
            e1();
        }
    }

    void t() {
        f();
        t1();
    }

    void t1() {
        if (c == '*') {
            next();
            f();
            t1();
        }
    }

    void f() {
        if (c == '(') {
            next();
            e();
            if (c == ')') {
                next();
            } else {
                cout << "Error: expected ')'" << endl;
                return;
            }
        } else if (isalpha(c)) {
            next();
        } else {
            cout << "Error: expected id or '('" << endl;
            return;
        }
    }
};

int main() {
    string input;
    cout << "Enter an expression: ";
    getline(cin, input);
    P p(input);
    p.parse();
    return 0;
}
