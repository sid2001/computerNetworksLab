#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./parser/expressionParser.cpp"
#include <vector>

struct node {
	char value;
	struct node* lchild;
	struct node* rchild;
};

struct node* createNode(char value) {
	struct node* n = (struct node*)malloc(sizeof(struct node));
	n->value = value;
	return n;
}



int main() {
	string st; 
	cin >> st;
	struct expression* ex = NewExp(st);
	int c = 0;
	read(ex);
	while(ex->ch != '\0') {
		printf("%c",ex->ch);
		read(ex);
		c++;
	}
	return 0;
}
