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

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

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

//GLOBAL VARIABLES
char nomearq[255];
atomo atom;
char carac;
FILE* programa;
int linha = 1;
int coluna = 0;

//FUNCTIONS
char novoCarac(){
	char caracLido = 0;
	fscanf(programa, "%c", &caracLido);
	if(caracLido == '\n'){
		linha++;
		coluna = 0;
	}else coluna++;
	return caracLido;
}

/**
* Dada uma string, a classifica em um dos tipos definido no inicio do programa
*/
atomo classificaCadeia(string cadeia){
	atomo resp;
	resp.tipo = resp.atrib.atr = 0;
	resp.atrib.cadeia = cadeia;

	if(isalpha(cadeia[0])){
		if(cadeia == "add"){
			resp.tipo = COMANDO;
			resp.atrib.atr = ADD;
		}else if(cadeia == "sub"){
			resp.tipo = COMANDO;
			resp.atrib.atr = SUB;
		}else if(cadeia == "mult"){
			resp.tipo = COMANDO;
			resp.atrib.atr = MULT;
		}else if(cadeia == "div"){
			resp.tipo = COMANDO;
			resp.atrib.atr = DIV;
		}else if(cadeia == "sw"){
			resp.tipo = COMANDO;
			resp.atrib.atr = SAVE;
		}else if(cadeia == "load"){
			resp.tipo = COMANDO;
			resp.atrib.atr = LOAD;
		}else{
			resp.tipo = ID;
		}
	}else if(isdigit(cadeia[0])){
		resp.tipo = CTE;
	}else{
		switch(cadeia[0]){
		case ':':
			resp.tipo = DPTS;
			break;
		case ',':
			resp.tipo = VIRG;
			break;
		case 0:
			resp.tipo = FINAL;
			break;
		default:
			resp.tipo = INVAL;
		}
	}
	return resp;
}

/**
* Le caracteres da entrada e percorre o diagrama de transicao
* ateh formar um atomo. E entao classifica o atomo encontrado 
* e a funcao termina.
* Retorna o ultimo caractere lido
*/
void novoAtomo(){
	int estado = 1;

	string cadeia;

	while(estado!=3){
		//cout<<"carac = "<<carac<<"(ASCII = "<<(int) carac<<") \testado = "<<estado<<endl;
		switch(estado){
		case 1:
			switch(carac){ 
			case ':':
			case ',':
				cadeia.push_back(carac);
				atom = classificaCadeia(cadeia);
				carac = novoCarac();
				estado = 3;
				break;
			default:
				if(isalpha(carac)){
					cadeia.push_back(carac);
					carac = novoCarac();
					estado = 2;
				}else if(isdigit(carac)){
					cadeia.push_back(carac);
					carac = novoCarac();
					estado = 4;
				}else if((isspace(carac)  || iscntrl(carac)) && carac!=0){
					carac = novoCarac();
					estado = 1;
				}else if(carac == 0){
					atom = classificaCadeia(cadeia);
					estado = 3;
				}else{
					cadeia.push_back(carac);
					carac = novoCarac();
					atom = classificaCadeia(cadeia);
					estado = 3;
				}
			}
			break;
		case 2: //ID
			if(isalnum(carac)){
				cadeia.push_back(carac);
				carac = novoCarac();
				estado = 2;
			}else{
				atom = classificaCadeia(cadeia);
				estado = 3;
			}
			break;
		case 4: //CTE
			if(isdigit(carac)){
				cadeia.push_back(carac);
				carac = novoCarac();
				estado = 4;
			}else{
				atom = classificaCadeia(cadeia);
				estado = 3;
			}
			break;

		}
	}
}

void imprimeCabecalho(){
	printf("%30s %s\t %s\n", "cadeia", "tipo", "atrib");
}

string converteTipoPraNome(int tipo){
	string nome;
	cout<<"tipo = "<<tipo<<endl;
	switch(tipo){
		case ID: nome = "ID"; break;
		case COMANDO: nome = "COMANDO"; break;
		case VIRG: nome = "VIRG"; break;
		case DPTS: nome = "DPTS"; break;
		case FINAL: nome = "FINAL"; break;
		case INVAL: nome = "INVAL"; break;
		default: nome = "UNDEFINED"; break;
	}
	return nome;
}

string converteAtribPraNome(int atrib){
	string nome;
	switch(atrib){
		case 0: nome = ""; break;
		case ADD: nome = "ADD"; break;
		case SUB: nome = "SUB"; break;
		case MULT: nome = "MULT"; break;
		case DIV: nome = "DIV"; break;
		case SAVE: nome = "SW"; break;
		case LOAD: nome = "LOAD"; break;
		default: nome = "UNDEFINED"; break;
	}
	return nome;
}

void imprimeAtomo(){
	string tipo = converteTipoPraNome(atom.tipo);
	string atr = converteAtribPraNome(atom.atrib.atr);

	printf("%30s %s\t %s\n", atom.atrib.cadeia.c_str(), tipo.c_str(), atr.c_str());

}

void naoEsperado(int tipo){
	string nome = converteTipoPraNome(tipo);
	printf("Nao era esperado cadeia do tipo '" RED "%s" RESET "'\n\t\t'" RED "%s" RESET "'\n", nome.c_str(), atom.atrib.cadeia.c_str());
	printf("\t\tlinha: %d\tcoluna: %d\n\n", linha, (int) (coluna - atom.atrib.cadeia.length()));
}

bool esperado(int tipo){
	if(atom.tipo!=tipo){
		string nomeTipo = converteTipoPraNome(tipo);
		string nomeTipoAtom = converteTipoPraNome(atom.tipo);

		printf("Era esperado um atomo do tipo '" RED "%s" RESET "' e foi encontrado do tipo '" RED "%s" RESET "'\n\t\t'" RED "%s" RESET "'\n", nomeTipo.c_str(), nomeTipoAtom.c_str(), atom.atrib.cadeia.c_str());
		printf("\t\tlinha: %d\tcoluna: %d\n\n", linha, (int) (coluna - atom.atrib.cadeia.length()));
		return true;

	}
	return false;
}


void comando(){
	//printf(GREEN "comando\t" YELLOW "\t%s" RESET "\n", atom.atrib.cadeia);
	//esperado(ID);
	if(atom.tipo == COMANDO){
		switch(atom.atrib.atr){
			case ADD:
			case SUB:
			case MULT:
			case DIV:
				novoAtomo();
				if(esperado(ID)) break;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;
				break;
			case SAVE:
				novoAtomo();
				if(esperado(ID)) break;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(CTE)) break;
		}
	}else if(atom.tipo == ID){
		novoAtomo();
		esperado(DPTS);

	}else{
		naoEsperado(atom.tipo);
	}


}

void listCmd();

void lCaux(){
	printf(GREEN "lcaux\t" YELLOW "\t%s " RESET "\n", atom.atrib.cadeia.c_str());
	if(atom.tipo == ID || atom.tipo == COMANDO){
		listCmd();
	}else if(atom.tipo != FINAL){
		naoEsperado(atom.tipo);
	}
}

void listCmd(){
	printf(GREEN "listCmd\t" YELLOW "\t%s " RESET "\n", atom.atrib.cadeia.c_str());
	comando();
	novoAtomo();
	lCaux();
}


int main(){
	printf("A N A L I S E   L E X I C A\n\n");
	printf("Nome do arquivo: ");
	setbuf(stdin, NULL);

	//scanf("%s", nomearq);

	strcpy(nomearq, "../entrada.txt");
	printf("\n");


	programa = fopen(nomearq, "r");
	imprimeCabecalho();

	atom.tipo = INVAL;
	carac = novoCarac();
	
	novoAtomo();
	lCaux();

	printf("\nAnalise do arquivo '%s' encerrada", nomearq);

	printf("\n\nVer atomos no arquivo 'atomosmp'\n");
	//system("pause");

	return 0;
}
