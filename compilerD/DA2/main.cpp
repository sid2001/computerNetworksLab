#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct Node {
    char symbol;
    Node *left, *right;
    set<int> firstpos, lastpos;
    bool nullable;

    Node(char sym) : symbol(sym), left(nullptr), right(nullptr), nullable(false) {}
};

Node* buildSyntaxTree(string re, vector<char>& leaves) {
    vector<Node*> stack;
    int pos = 1;
    for (char c : re) {
        if (c == '|' || c == '.' || c == '*') {
            Node* node = new Node(c);
            if (c == '*') {
                node->left = stack.back();
                stack.pop_back();
                node->nullable = true;
                node->firstpos = node->left->firstpos;
                node->lastpos = node->left->lastpos;
            } else {
                node->right = stack.back();
                stack.pop_back();
                node->left = stack.back();
                stack.pop_back();
                if (c == '|') {
                    node->nullable = node->left->nullable || node->right->nullable;
                    node->firstpos.insert(node->left->firstpos.begin(), node->left->firstpos.end());
                    node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
                    node->lastpos.insert(node->left->lastpos.begin(), node->left->lastpos.end());
                    node->lastpos.insert(node->right->lastpos.begin(), node->right->lastpos.end());
                } else if (c == '.') {
                    node->nullable = node->left->nullable && node->right->nullable;
                    if (node->left->nullable) {
                        node->firstpos.insert(node->left->firstpos.begin(), node->left->firstpos.end());
                        node->firstpos.insert(node->right->firstpos.begin(), node->right->firstpos.end());
                    } else {
                        node->firstpos = node->left->firstpos;
                    }
                    if (node->right->nullable) {
                        node->lastpos.insert(node->left->lastpos.begin(), node->left->lastpos.end());
                        node->lastpos.insert(node->right->lastpos.begin(), node->right->lastpos.end());
                    } else {
                        node->lastpos = node->right->lastpos;
                    }
                }
            }
            stack.push_back(node);
        } else {
            Node* node = new Node(c);
            node->firstpos.insert(pos);
            node->lastpos.insert(pos);
            leaves.push_back(c);
            stack.push_back(node);
            pos++;
        }
    }
    return stack.back();
}

void computeFollowpos(Node* root, map<int, set<int>>& followpos) {
    if (!root) return;
    computeFollowpos(root->left, followpos);
    computeFollowpos(root->right, followpos);
    if (root->symbol == '.') {
        for (int i : root->left->lastpos) {
            followpos[i].insert(root->right->firstpos.begin(), root->right->firstpos.end());
        }
    }
    if (root->symbol == '*') {
        for (int i : root->lastpos) {
            followpos[i].insert(root->firstpos.begin(), root->firstpos.end());
        }
    }
}

void printSet(const set<int>& s) {
    cout << "{";
    for (auto it = s.begin(); it != s.end(); ++it) {
        cout << *it;
        if (next(it) != s.end()) cout << ", ";
    }
    cout << "}";
}

int main() {
    string re;
    cout << "Enter Regular Expression: ";
    cin >> re;

    vector<char> leaves;
    Node* syntaxTree = buildSyntaxTree(re, leaves);
    map<int, set<int>> followpos;
    computeFollowpos(syntaxTree, followpos);

    cout << "Firstpos(n): ";
    printSet(syntaxTree->firstpos);
    cout << endl;

    cout << "Lastpos(n): ";
    printSet(syntaxTree->lastpos);
    cout << endl;

    cout << "Followpos(i):" << endl;
    for (size_t i = 0; i < leaves.size(); ++i) {
        cout << "followpos(" << (i + 1) << "): ";
        printSet(followpos[i + 1]);
        cout << endl;
    }

    return 0;
}
