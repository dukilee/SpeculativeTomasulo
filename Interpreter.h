
#ifndef INTERPRETER
#define INTERPRETER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

using namespace std;

//DEFINES
enum ENUM_TIPOS{BEGIN=1, PROGRAM, DPTS, VIRG, ID, CTE,
INVAL, COMANDO, FINAL};

enum ENUM_ATRIB{ADD=1, SUB, MULT, DIV, SAVE, LOAD};

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


class Interpreter{
public:
	char nomearq[255];
	atomo atom;
	char carac;
	FILE* programa;
	int linha;
	Interpreter(string nomearq);

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
};

#endif
