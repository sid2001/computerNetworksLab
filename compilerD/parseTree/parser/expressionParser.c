#include <stdlib.h>

#define EXPRESSION_SIZE 100
#define ES EXPRESSION_SIZE

struct expression {
	int	position;	// holds position for the current character under cursor
	char	ch;		//current character
	//int	next_position;	//next position of the character to be scanned
	char*	exp;

};

struct expression* NewExp(char* exp) {
	struct expression* e = (struct expression*)malloc(sizeof(struct expression));
	e->exp = (char*)malloc(ES*sizeof(char));
	sprintf(e->exp,"%s",exp);
	e->position = 0;

	return e;
}

void read(struct expression* e) {
	e->ch = e->exp[e->position];
	e->position++;

	return;
}
