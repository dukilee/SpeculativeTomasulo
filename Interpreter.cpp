#include <Interpreter.h>

using namespace std;

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

Interpreter::Interpreter(string nomearq){
	pc = 0;
	linha = 1;

	programa = fopen(nomearq.c_str(), "r");

	atom.tipo = INVAL;
	carac = novoCarac();
	
	novoAtomo();
	lCaux();
}

//FUNCTIONS
char Interpreter::novoCarac(){
	char caracLido = 0;
	fscanf(programa, "%c", &caracLido);
	if(caracLido == '\n'){
		linha++;
	}
	return caracLido;
}

/**
* Dada uma string, a classifica em um dos tipos definido no inicio do programa
*/
atomo Interpreter::classificaCadeia(string cadeia){
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
		}else if(cadeia == "lw"){
			resp.tipo = COMANDO;
			resp.atrib.atr = LOAD;
		}else if(cadeia == "addi"){
			resp.tipo = COMANDO;
			resp.atrib.atr = ADDI;
		}else if(cadeia == "beq"){
			resp.tipo = COMANDO;
			resp.atrib.atr = BEQ;
		}else if(cadeia == "ble"){
			resp.tipo = COMANDO;
			resp.atrib.atr = BLE;
		}else if(cadeia == "bne"){
			resp.tipo = COMANDO;
			resp.atrib.atr = BNE;
		}else if(cadeia == "li"){
			resp.tipo = COMANDO;
			resp.atrib.atr = LI;
		}else if(cadeia == "return"){
			resp.tipo = COMANDO;
			resp.atrib.atr = RETURN;
		}else if(cadeia == "goto"){
			resp.tipo = COMANDO;
			resp.atrib.atr = GOTO;
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
		case ';':
			resp.tipo = COMMENT;
			break;
		case '(':
			resp.tipo = ABPAR;
			break;
		case ')':
			resp.tipo = FPAR;
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
void Interpreter::novoAtomo(){
	int estado = 1;

	string cadeia;

	while(estado!=3){
		//cout<<"carac = "<<carac<<"(ASCII = "<<(int) carac<<") \testado = "<<estado<<endl;
		switch(estado){
		case 1:
			switch(carac){ 
			case ':':
			case ',':
			case '(':
			case ')':
				cadeia.push_back(carac);
				atom = classificaCadeia(cadeia);
				carac = novoCarac();
				estado = 3;
				break;
			case ';':
				estado = 5;
				carac = novoCarac();
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
		case 5:
			if(carac=='\n'){
				estado = 3;
				atom = classificaCadeia(cadeia);
				carac = novoCarac();
			}

		}
	}
}

void Interpreter::imprimeCabecalho(){
	printf("%30s %s\t %s\n", "cadeia", "tipo", "atrib");
}

string Interpreter::converteTipoPraNome(int tipo){
	string nome;
	switch(tipo){
		case ID: nome = "ID"; break;
		case COMANDO: nome = "COMANDO"; break;
		case VIRG: nome = "VIRG"; break;
		case DPTS: nome = "DPTS"; break;
		case FINAL: nome = "FINAL"; break;
		case INVAL: nome = "INVAL"; break;
		case CTE: nome = "CTE"; break;
		case COMMENT: nome = "COMMENT"; break;
		case ABPAR: nome = "ABPAR"; break;
		case FPAR: nome = "FPAR"; break;
		default: nome = "UNDEFINED"; break;
	}
	return nome;
}

string Interpreter::converteAtribPraNome(int atrib){
	string nome;
	switch(atrib){
		case 0: nome = ""; break;
		case ADD: nome = "ADD"; break;
		case SUB: nome = "SUB"; break;
		case MULT: nome = "MULT"; break;
		case DIV: nome = "DIV"; break;
		case SAVE: nome = "SW"; break;
		case LOAD: nome = "LOAD"; break;
		case ADDI: nome = "ADDI"; break;
		case BLE: nome = "BLE"; break;
		case BEQ: nome = "BEQ"; break;
		case BNE: nome = "BNE"; break;
		case LI: nome = "LI"; break;
		case RETURN: nome = "return"; break;
		case GOTO: nome = "nome"; break;
		default: nome = "UNDEFINED"; break;
	}
	return nome;
}

void Interpreter::imprimeAtomo(){
	string tipo = converteTipoPraNome(atom.tipo);
	string atr = converteAtribPraNome(atom.atrib.atr);

	printf("%30s %s\t %s\n", atom.atrib.cadeia.c_str(), tipo.c_str(), atr.c_str());

}

void Interpreter::naoEsperado(int tipo){
	string nome = converteTipoPraNome(tipo);
	printf("Nao era esperado cadeia do tipo '" RED "%s" RESET "'\n\t\t'" RED "%s" RESET "'", nome.c_str(), atom.atrib.cadeia.c_str());
	printf("\tlinha: %d\n\n", linha);
}

bool Interpreter::esperado(int tipo){
	if(atom.tipo!=tipo){
		string nomeTipo = converteTipoPraNome(tipo);
		string nomeTipoAtom = converteTipoPraNome(atom.tipo);

		printf("Era esperado um atomo do tipo '" RED "%s" RESET "' e foi encontrado do tipo '" RED "%s" RESET "'\n\t\t'" RED "%s" RESET "'", nomeTipo.c_str(), nomeTipoAtom.c_str(), atom.atrib.cadeia.c_str());
		printf("\tlinha: %d\n\n", linha);
		return true;

	}
	return false;
}


void Interpreter::comando(){
	//printf(GREEN "comando\t" YELLOW "\t%s" RESET "\n", atom.atrib.cadeia);
	//esperado(ID);
	comand c;
	c.tipo = atom.tipo;
	c.atrib = atom.atrib.atr;
	if(atom.tipo == COMANDO){
		switch(atom.atrib.atr){
			case ADD:
			case SUB:
			case MULT:
			case DIV:
			case BLE:
			case BNE:
			case BEQ:

				novoAtomo();
				if(esperado(ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;
				c.p2.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;
				c.p3.address = atom.atrib.cadeia;
				listCommands.push_back(c);

				break;
			case ADDI:
				novoAtomo();
				if(esperado(ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;
				c.p2.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(CTE)) break;
				c.p3.value = atoi(atom.atrib.cadeia.c_str());
				listCommands.push_back(c);
				break;
			case LI:
				novoAtomo();
				if(esperado(ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(CTE)) break;
				c.p2.value = atoi(atom.atrib.cadeia.c_str());
				listCommands.push_back(c);
				break;

			case LOAD:
			case SAVE:
				novoAtomo();
				if(esperado(ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;


				novoAtomo();
				if(esperado(CTE)) break;
				c.p2.value = atoi(atom.atrib.cadeia.c_str());

				novoAtomo();
				if(esperado(ABPAR)) break;

				novoAtomo();
				if(esperado(ID)) break;
				c.p3.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(FPAR)) break;
				listCommands.push_back(c);
				break;
			case RETURN:
				listCommands.push_back(c);
				break;
			case GOTO:
				novoAtomo();
				if(esperado(ID)) break;
				c.p1.address = atom.atrib.cadeia;
				listCommands.push_back(c);
				break;
		}
	}else if(atom.tipo == ID){
		string labelName = atom.atrib.cadeia;
		novoAtomo();
		if(esperado(DPTS)) return;
		labels[labelName] = listCommands.size();
	}else{
		naoEsperado(atom.tipo);
	}


}


void Interpreter::lCaux(){
	//printf(GREEN "lcaux\t" YELLOW "\t%s " RESET "\n", atom.atrib.cadeia.c_str());
	if(atom.tipo == ID || atom.tipo == COMANDO){
		listCmd();
	}else if(atom.tipo != FINAL){
		naoEsperado(atom.tipo);
	}
}

void Interpreter::listCmd(){
	//printf(GREEN "listCmd\t" YELLOW "\t%s " RESET "\n", atom.atrib.cadeia.c_str());
	comando();
	novoAtomo();
	lCaux();
}

bool Interpreter::runNextLine(){
	if(pc >= listCommands.size())
		return false;;

	comand c = listCommands[pc++];

	switch(c.atrib){
		case ADD:
			reg[c.p1.address] = reg[c.p2.address] + reg[c.p3.address];
			break;
		case ADDI:
			reg[c.p1.address] = reg[c.p2.address] + c.p3.value;
			break;
		case SUB:
			reg[c.p1.address] = reg[c.p2.address] - reg[c.p3.address];
			break;
		case MULT:
			reg[c.p1.address] = reg[c.p2.address] * reg[c.p3.address];
			break;
		case DIV:
			reg[c.p1.address] = reg[c.p2.address] / reg[c.p3.address];
			break;
		case LI:
			reg[c.p1.address] = c.p2.value;
			break;
		case SAVE:
			memory[c.p2.value][reg[c.p3.address]] = reg[c.p1.address];
			break;
		case LOAD:
			reg[c.p1.address] = memory[c.p2.value][reg[c.p3.address]];
			break;
		case BEQ:
			if(reg[c.p1.address] == reg[c.p2.address]){
				stk.push(pc);
				map<string, int>::iterator it = labels.find(c.p3.address);
				if(it == labels.end()){
					printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
				}else{
					stk.push(pc);
					pc = it->second;
				}

			}
			break;
		case BNE:
			if(reg[c.p1.address] != reg[c.p2.address]){
				map<string, int>::iterator it = labels.find(c.p3.address);
				if(it == labels.end()){
					printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
				}else{
					stk.push(pc);
					pc = it->second;
				}
			}
			break;
		case BLE:
			if(reg[c.p1.address] <= reg[c.p2.address]){
				map<string, int>::iterator it = labels.find(c.p3.address);
				if(it == labels.end()){
					printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
				}else{
					stk.push(pc);
					pc = it->second;
				}
			}
			break;
		case RETURN:
			if(!stk.empty()){
				pc = stk.top();
				stk.pop();
			}else{
				printf("'" RED "RETURN" RESET "' miss use\n");
			}
			break;
		case GOTO:
			map<string, int>::iterator it = labels.find(c.p1.address);
			if(it == labels.end()){
				printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
			}else{
				pc = it->second;
			}
			break;
			
	}

	return true;
}
