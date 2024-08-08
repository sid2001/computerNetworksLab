#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

// Token types
enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, SYMBOL, UNKNOWN, WHITESPACE
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// Set of C keywords
unordered_set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
    "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

// Set of C operators
unordered_set<string> operators = {
    "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "&", "|", "^", "~",
    "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="
};

// Set of C symbols
unordered_set<char> symbols = {'(', ')', '{', '}', '[', ']', ';', ',', '.'};

// Function to check if a string is a keyword
bool isKeyword(const string& str) {
    return keywords.find(str) != keywords.end();
}

// Function to check if a string is an operator
bool isOperator(const string& str) {
    return operators.find(str) != operators.end();
}

// Function to check if a character is a symbol
bool isSymbol(char ch) {
    return symbols.find(ch) != symbols.end();
}

// Function to tokenize the input source code
vector<Token> tokenize(const string& source) {
    vector<Token> tokens;
    string buffer;
    for (size_t i = 0; i < source.length(); ++i) {
        char ch = source[i];

        // Skip whitespace
        if (isspace(ch)) {
            if (!buffer.empty()) {
                tokens.push_back({TokenType::UNKNOWN, buffer});
                buffer.clear();
            }
            tokens.push_back({TokenType::WHITESPACE, string(1, ch)});
            continue;
        }

        // Check for operators
        if (isOperator(string(1, ch))) {
            if (!buffer.empty()) {
                tokens.push_back({TokenType::UNKNOWN, buffer});
                buffer.clear();
            }
            // Handle multi-character operators
            string op(1, ch);
            if (i + 1 < source.length() && isOperator(op + source[i + 1])) {
                op += source[++i];
            }
            tokens.push_back({TokenType::OPERATOR, op});
            continue;
        }

        // Check for symbols
        if (isSymbol(ch)) {
            if (!buffer.empty()) {
                tokens.push_back({TokenType::UNKNOWN, buffer});
                buffer.clear();
            }
            tokens.push_back({TokenType::SYMBOL, string(1, ch)});
            continue;
        }

        // Check for numbers
        if (isdigit(ch)) {
            if (!buffer.empty() && !isdigit(buffer[0])) {
                tokens.push_back({TokenType::UNKNOWN, buffer});
                buffer.clear();
            }
            buffer += ch;
            continue;
        }

        // Check for identifiers or keywords
        if (isalpha(ch) || ch == '_') {
            buffer += ch;
            if (i + 1 < source.length() && (isalnum(source[i + 1]) || source[i + 1] == '_')) {
                continue;
            }
            if (isKeyword(buffer)) {
                tokens.push_back({TokenType::KEYWORD, buffer});
            } else {
                tokens.push_back({TokenType::IDENTIFIER, buffer});
            }
            buffer.clear();
            continue;
        }

        // Default case
        buffer += ch;
    }

    if (!buffer.empty()) {
        tokens.push_back({TokenType::UNKNOWN, buffer});
    }

    return tokens;
}

// Function to convert token type to string
string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::SYMBOL: return "SYMBOL";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::WHITESPACE: return "WHITESPACE";
        default: return "UNKNOWN";
    }
}

int main() {
    // Read source code from a file
    ifstream file("addition.c");
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Tokenize the source code
    vector<Token> tokens = tokenize(source);

    // Print tokens
    for (const Token& token : tokens) {
        if (token.type != TokenType::WHITESPACE) {
            cout << tokenTypeToString(token.type) << ": " << token.value << endl;
        }
    }

    return 0;
}
