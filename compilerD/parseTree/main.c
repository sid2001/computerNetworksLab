#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
	char value;
	struct node* lchild;
	struct ndoe* rchild;
};

struct node* createNode(char value) {
	struct node* n = (struct node*)malloc(sizeof(struct node));
	n->value = value;
	return n;
}

int main() {
	char* st;

	scanf("%s",st);
	struct node* n= createNode('s');
	printf("%c",n->value);
	
	return 0;

}
