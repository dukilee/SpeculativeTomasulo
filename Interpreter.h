
#ifndef INTERPRETER
#define INTERPRETER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <map>
#include <stack>

using namespace std;

//DEFINES
enum ENUM_TIPOS{BEGIN=1, PROGRAM, DPTS, VIRG, ID, CTE,
INVAL, COMANDO, ABPAR, FPAR, COMMENT, FINAL};

enum ENUM_ATRIB{ADD=1, SUB, MULT, DIV, SAVE, LOAD, ADDI, BEQ, BLE, BNE, LI, RETURN, GOTO};

//STRUCTS
struct atribatomo{
	string cadeia;
	int valor;
	int atr;
	char caractere;
};
struct atomo{
	int tipo;
	atribatomo atrib;
};

struct param{
	int value;
	string address;
};

struct comand{
	int tipo, atrib;
	param p1, p2, p3;
};

class Interpreter{
public:
	char nomearq[255];
	atomo atom;
	char carac;
	FILE* programa;
	int linha;
	Interpreter(string nomearq);
	int pc;

	char novoCarac();
	atomo classificaCadeia(string cadeia);
	void novoAtomo();
	void imprimeCabecalho();
	string converteTipoPraNome(int tipo);
	string converteAtribPraNome(int atrib);
	void imprimeAtomo();
	void naoEsperado(int tipo);
	bool esperado(int tipo);
	void comando();
	void lCaux();
	void listCmd();
	bool runNextLine();

	vector<comand> listCommands;
	map<string, int> labels;
	map<string, int> reg;
	map<int, map<int, int> > memory;
	stack<int> stk;
};

#endif
