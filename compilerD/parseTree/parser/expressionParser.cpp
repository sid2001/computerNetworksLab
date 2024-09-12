#include <stdlib.h>

#define EXPRESSION_SIZE 100
#define ES EXPRESSION_SIZE

class Expression {
private:
	int		position;	// holds position for the current character under cursor
	char		ch;		//current character
	//int	next_position;	//next position of the character to be scanned
	int		nextPosition;
	std::string	exp;		//expression string
public:
	Expression(string st) : exp(st) {
		this->position = 0;
		this->char = st[position];
		this->nextPosition = 1;
	}

	void readId() {
		int position = this->position;
		while(isNotOperator(this->ch)) {
			this->position++;
			this->ch = this->exp[this->position];
		}


	}

	static int isNotOperator(char ch) {
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
	}
	
};

struct expression* NewExp(string exp) {
	struct expression* e = (struct expression*)malloc(sizeof(struct expression));
	e->exp = exp
	sprintf(e->exp,"%s",exp);
	e->position = 0;

	return e;
}

void read(struct expression* e) {
	e->ch = e->exp[e->position];
	e->position++;

	return;
}
