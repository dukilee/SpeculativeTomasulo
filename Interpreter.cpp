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
	clock = 0;
	emptyPos = -1;
	runnedCommands=0;
	sizeQueue = 6;

	programa = fopen(nomearq.c_str(), "r");

	atom.tipo = INVAL;
	carac = novoCarac();
	
	novoAtomo();
	lCaux();

	numLoads = 2;
	numAdds = 3;
	numMults = 2;
	timeToFinishLoad = 4;
	timeToFinishAdd = 1;
	timeToFinishMult = 3;
	timeToFinishDiv = 5;

	firstLoads = 0;
	lastLoads = numLoads - 1;
	firstAdds = lastLoads + 1;;
	lastAdds = firstAdds + numAdds - 1;
	firstMults = lastAdds + 1;
	lastMults = firstMults + numMults - 1;

	tomasuloTable.resize(numLoads + numAdds + numMults);
	for(int i = 0; i<numLoads; i++){
		tomasuloTable[i].name = "LOAD0";
		tomasuloTable[i].name[4] = i + 48;
	}
	for(int i = 0; i<numAdds; i++){
		tomasuloTable[i+numLoads].name = "ADDD0";
		tomasuloTable[i+numLoads].name[4] = i + 48;
	}
	for(int i = 0; i<numMults; i++){
		tomasuloTable[i+numLoads+numAdds].name = "MULT0";
		tomasuloTable[i+numLoads+numAdds].name[4] = i + 48;
	}

	que.resize(sizeQueue);
	contQueue = 0;
	posQueue = 0;
	for(int i = 0; i<sizeQueue; i++){
		que[i].busy = false;
	}
}

pair<int, int> Interpreter::findInterval(int x){
	if(x>=firstLoads  && x<=lastLoads){
		return pair<int, int>(firstLoads, lastLoads);
	}
	if(x>=firstAdds  && x<=lastAdds){
		return pair<int, int>(firstAdds, lastAdds);
	}
	if(x>=firstMults  && x<=lastMults){
		return pair<int, int>(firstMults, lastMults);
	}
	return pair<int, int>(1, 0);
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

string Interpreter::toLower(string s){
	for(int i = 0; i<s.length(); i++){
		if(s[i]>=65 && s[i]<=90) s[i]+=32;
	}
	return s;
}

/**
* Dada uma string, a classifica em um dos tipos definido no inicio do programa
*/
atomo Interpreter::classificaCadeia(string cadeia){
	atomo resp;
	resp.tipo = resp.atrib.atr = 0;
	cadeia = toLower(cadeia);
	resp.atrib.cadeia = cadeia;

	if(isalpha(cadeia[0])){
		if(cadeia == "add"){
			resp.tipo = COMANDO;
			resp.atrib.atr = ADD;
		}else if(cadeia == "sub"){
			resp.tipo = COMANDO;
			resp.atrib.atr = SUB;
		}else if(cadeia == "mul"){
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
		//}else if(cadeia == "return"){
			//resp.tipo = COMANDO;
			//resp.atrib.atr = RETURN;
		}else if(cadeia == "goto"){
			resp.tipo = COMANDO;
			resp.atrib.atr = GOTO;
		}else{
			bool isMem = (cadeia[0]=='r' || cadeia[0]=='R');
			for(int i = 1; i<cadeia.length() && isMem; i++){
				if(!isdigit(cadeia[i])){
					isMem = false;
				}
			}
			if(isMem){
				resp.tipo = REG_ID;
			}else{
				resp.tipo = ID;
			}
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
			}else carac = novoCarac();
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
		case REG_ID: nome = "REG_ID"; break;
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
		case MULT: nome = "MUL"; break;
		case DIV: nome = "DIV"; break;
		case SAVE: nome = "SW"; break;
		case LOAD: nome = "LOAD"; break;
		case ADDI: nome = "ADDI"; break;
		case BLE: nome = "BLE"; break;
		case BEQ: nome = "BEQ"; break;
		case BNE: nome = "BNE"; break;
		case LI: nome = "LI"; break;
		case RETURN: nome = "RETURN"; break;
		case GOTO: nome = "GOTO"; break;
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
	c.line = (int) listCommands.size();
	c.predictive = -1;
	string axu;
	int la;
	char ch;
	if(atom.tipo == COMANDO){
		switch(atom.atrib.atr){
			case ADD:
			case SUB:
			case MULT:
			case DIV:
				c.nParams = 3;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p2.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p3.address = atom.atrib.cadeia;
				listCommands.push_back(c);

				break;
			case BLE:
			case BNE:
			case BEQ:
				c.predictive = 0;
				c.nParams = 3;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p2.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(ID)) break;
				c.p3.address = atom.atrib.cadeia;
				listCommands.push_back(c);

				break;
			case ADDI:
				c.nParams = 3;
				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p2.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;

				novoAtomo();
				if(esperado(CTE)) break;
				c.p3.value = atoi(atom.atrib.cadeia.c_str());
				listCommands.push_back(c);
				break;
			case LI:
				c.nParams = 2;
				novoAtomo();
				if(esperado(REG_ID)) break;
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
				c.nParams = 2;
				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p1.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(VIRG)) break;


				novoAtomo();
				if(esperado(CTE)) break;
				c.p2.value = atoi(atom.atrib.cadeia.c_str());

				novoAtomo();
				if(esperado(ABPAR)) break;

				novoAtomo();
				if(esperado(REG_ID)) break;
				c.p3.address = atom.atrib.cadeia;

				novoAtomo();
				if(esperado(FPAR)) break;
				la = c.p2.value;
				axu = "";
				do{
					ch = la + 48;
					axu += ch;
					la /= 10;
				}while(la>0);

				c.d = axu + "(" + c.p3.address + ")";
				
				listCommands.push_back(c);
				break;
			case RETURN:
				c.nParams = 0;
				listCommands.push_back(c);
				break;
			case GOTO:
				c.nParams = 1;
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

		c.nParams = 0;
		c.p1.address = labelName+":";
		listCommands.push_back(c);
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

void Interpreter::printaCommand(comand c){
	cout<<converteTipoPraNome(c.tipo)<<" "<<converteAtribPraNome(c.atrib)<<" "<<c.p1.address<<" "<<c.p2.address<<" "<<c.p3.address<<endl;
}

int Interpreter::runCommand(comand c, int vj, int vk){
	runnedCommands++;
	//cout<<"RUN: ";
	//printaCommand(c);
	switch(c.atrib){
		case ADD:
		case ADDI:
			return vj + vk;
		case SUB:
			return vj - vk;
		case MULT:
			return vj * vk;
		case DIV:
			return vj / vk;
		case LI:
			return vj;
		case SAVE:
			return vj;
		case LOAD:
			return memory[vj][vk].value;
		case BEQ:
		case BNE:
		case BLE:
			bool condition;
			vj = reg[c.p1.address].value;
			vk = reg[c.p2.address].value;
			switch(c.atrib){
				case BEQ: condition = (vj == vk); break;
				case BNE: condition = (vj != vk); break;
				case BLE: condition = (vj <= vk); break;
			}


			if(condition != c.predictive){
				contQueue = 0;
				for(int i = 0; i<sizeQueue; i++){
					comand t = que[i].c;
					if(c.atrib == SAVE){
						int a1 = c.p2.value;
						int a2 = c.p3.value;
						if(memory[a1][a2].dependency == i){
							memory[a1][a2].dataDependency = false;
						}
					}else{
						if(reg[t.p1.address].dataDependency && reg[t.p1.address].dependency == i){
							reg[t.p1.address].dataDependency = false;
						}
					}
					que[i].busy = false;

				}

				if(condition){
					map<string, int>::iterator it = labels.find(c.p3.address);
					if(it == labels.end()){
						printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
					}else{
						//stk.push(pc);
						pc = it->second;
					}

				}else{
					pc = c.line+1;
				}
				listCommands[c.line].predictive = condition;
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
	return -19;
}

void Interpreter::tryToGetValue(int id, char ch, string address){
	if(reg[address].dataDependency){
		int pos = reg[address].dependency;
		if(que[pos].state<=1)
			if(ch=='j') tomasuloTable[id].qj = reg[address].dependency;
			else tomasuloTable[id].qk = reg[address].dependency;
		else{
			if(ch=='j') tomasuloTable[id].vj = que[pos].val;
			else  tomasuloTable[id].vk = que[pos].val;
		}
	}else{
		if(ch=='j') tomasuloTable[id].vj = reg[address].value;
		else tomasuloTable[id].vk = reg[address].value;
		
	}
}

int Interpreter::getNextEmpty(int first, int last){
	int id = first;
	while(id<=last && tomasuloTable[id].busy) id++;

	if(id>last) return -1;

	tomasuloTable[id].busy = true;
	tomasuloTable[id].op = pc-1;
	tomasuloTable[id].qj = -1;
	tomasuloTable[id].qk = -1;
	emptyPos = id;
	return id;
}

bool Interpreter::hasEnded(){
	if(pc<listCommands.size()) return false;
	for(int i = 0; i<tomasuloTable.size(); i++){
		if(tomasuloTable[i].busy) return false;
	}
	return contQueue==0;
}

void Interpreter::continueCommand(int id){
	comand c = listCommands[tomasuloTable[id].op];
	int pos = tomasuloTable[id].posQueue;
	que[pos].state = 2;
	//cout<<"CONTINUE: ";
	//printaCommand(c);
	int val = runCommand(c, tomasuloTable[id].vj, tomasuloTable[id].vk);
	que[pos].val = val;
	if(c.atrib == SAVE)
		que[pos].c.p3.value = tomasuloTable[id].vk;
	

	bool changed = false;
	for(int i = 0; i<tomasuloTable.size(); i++){
		if(!tomasuloTable[i].busy) continue;
		changed = false;
		if(tomasuloTable[i].qj == pos){
			tomasuloTable[i].vj = val;
			tomasuloTable[i].qj = -1;
			changed = true;
		}
		if(tomasuloTable[i].qk == pos){
			tomasuloTable[i].vk = val;
			tomasuloTable[i].qk = -1;
			changed = true;
		}

		if(changed && tomasuloTable[i].qk == -1 && tomasuloTable[i].qj == -1){
			tomasuloTable[i].clockToFinish += timeToFree(i);
		}
	}
	tomasuloTable[id].busy = false;
	
}

int Interpreter::timeToFree(int x){
	pair<int, int> interval = findInterval(x);
	int minimum = clock;
	for(int j = interval.first; j<=interval.second; j++){
		if(j==x) continue;
		if(!tomasuloTable[j].busy) continue;
		minimum = max(minimum, tomasuloTable[j].clockToFinish);
	}

	return minimum;

}

bool Interpreter::runNextLine(){
	if(hasEnded()) return false;
	clock++;
	int id = -1;

	for(int i = 0; i<tomasuloTable.size(); i++){
		
		if(tomasuloTable[i].busy && tomasuloTable[i].qk==-1 && tomasuloTable[i].qj == -1 && tomasuloTable[i].clockToFinish<=clock){
			continueCommand(i);
			break;
		}
	}
	emptyPos = -1;

	if(contQueue>0){
		int curr = (posQueue + sizeQueue - contQueue)%sizeQueue;
		comand c = que[curr].c;
		if(que[curr].state==2){
			que[curr].state = 3;
			if(c.atrib == SAVE){
				int a1 = c.p2.value;
				int a2 = c.p3.value;
				memory[a1][a2].value = que[curr].val;
				if(memory[a1][a2].dependency == curr){
					memory[a1][a2].dataDependency = false;
				}
			}else{
				reg[c.p1.address].value = que[curr].val;
				if(reg[c.p1.address].dataDependency && reg[c.p1.address].dependency == curr){
					reg[c.p1.address].dataDependency = false;
				}
			}
			que[curr].busy = false;
			if(contQueue>0)
				contQueue--;
		}else if(que[curr].state == 1 && (c.atrib==BLE || c.atrib==BEQ || c.atrib==BNE) ){
			runCommand(c, 0, 0);
			que[curr].state = 3;
			que[curr].busy = false;
			if(contQueue>0)
				contQueue--;
		}
	}

	if(pc >= listCommands.size())
		return hasEnded();
	
	if(contQueue>=sizeQueue) return hasEnded();

	comand c = listCommands[pc++];
	while(c.tipo != COMANDO) c = listCommands[pc++];
	id = -1;

	switch(c.atrib){
		case ADD:
		case SUB:
		case MULT:
		case DIV:
			if(c.atrib==MULT || c.atrib==DIV)
				id = getNextEmpty(firstMults, lastMults);
			else
				id = getNextEmpty(firstAdds, lastAdds);

			if(id==-1){
				pc--;
				break;
			}
			tryToGetValue(id, 'j', c.p2.address);
			tryToGetValue(id, 'k', c.p3.address);
			int timeToFinish;
			switch(c.atrib){
				case SUB:
				case ADD:
					timeToFinish = timeToFinishAdd;
					break;
				case MULT:
					timeToFinish = timeToFinishMult;
					break;
				case DIV:
					timeToFinish = timeToFinishDiv;
					break;
					
			}
			tomasuloTable[id].clockToFinish = timeToFinish;
			if(tomasuloTable[id].qj==-1 && tomasuloTable[id].qk==-1)
				tomasuloTable[id].clockToFinish += timeToFree(id);
			break;
		case ADDI:
			id = getNextEmpty(firstAdds, lastAdds);
			if(id==-1){
				pc--;
				break;
			}
			tryToGetValue(id, 'j', c.p2.address);
			tomasuloTable[id].vk = c.p3.value;
			tomasuloTable[id].clockToFinish = timeToFinishAdd;
			if(tomasuloTable[id].qj==-1 && tomasuloTable[id].qk==-1)
				tomasuloTable[id].clockToFinish += timeToFree(id);
			break;
		case LI:
			id = getNextEmpty(firstLoads, lastLoads);
			if(id==-1){
				pc--;
				break;
			}
			tomasuloTable[id].vj = c.p2.value;
			tomasuloTable[id].clockToFinish = timeToFinishLoad;
			if(tomasuloTable[id].qj==-1 && tomasuloTable[id].qk==-1)
				tomasuloTable[id].clockToFinish += timeToFree(id);
			break;
		case LOAD:
			if(reg[c.p3.address].dataDependency){
				pc--;
				break;
			}
			id = getNextEmpty(firstLoads, lastLoads);
			if(id==-1){
				pc--;
				break;
			}


			tomasuloTable[id].vj = c.p2.value;
			tryToGetValue(id, 'k', c.p3.address);
			tomasuloTable[id].clockToFinish = timeToFinishLoad;
			if(tomasuloTable[id].qj==-1 && tomasuloTable[id].qk==-1)
				tomasuloTable[id].clockToFinish += timeToFree(id);
			break;
		case SAVE:
			if(reg[c.p3.address].dataDependency){
				pc--;
				break;
			}
			id = getNextEmpty(firstLoads, lastLoads);
			if(id==-1){
				pc--;
				break;
			}

			tryToGetValue(id, 'j', c.p1.address);
			tryToGetValue(id, 'k', c.p3.address);
			tomasuloTable[id].clockToFinish = timeToFinishLoad;
			if(tomasuloTable[id].qj==-1 && tomasuloTable[id].qk==-1)
				tomasuloTable[id].clockToFinish += timeToFree(id);
			break;
		case BEQ:
		case BNE:
		case BLE:
			id = -2;
			if(c.predictive){
				map<string, int>::iterator it = labels.find(c.p3.address);
				if(it == labels.end()){
					printf("The label '" RED "%s" RESET "' wasn't defined.\n", c.p3.address.c_str());
				}else{
					//stk.push(pc);
					pc = it->second;
				}

			}
			break;
		case RETURN:
		case GOTO:
			runCommand(c, 0, 0);
			break;
			
			

	}
	if(id!=-1){

		if(id>=0)
		switch(c.atrib){
			case SAVE:
				memory[c.p2.value][reg[c.p3.address].value].dataDependency = true;
				memory[c.p2.value][reg[c.p3.address].value].dependency = posQueue;
			break;
			default:
				reg[c.p1.address].dataDependency = true;
				reg[c.p1.address].dependency = posQueue;
		}

		if(id>=0)
			tomasuloTable[id].posQueue = posQueue;

		que[posQueue].busy = 1;
		que[posQueue].c = c;
		que[posQueue].state = 1;
		posQueue = (posQueue+1)%sizeQueue;
		contQueue++;
	}
	if(pc<listCommands.size()){
		c = listCommands[pc];
		while(c.tipo != COMANDO && pc+1<listCommands.size()) c = listCommands[++pc];
	}

	return hasEnded();
}

void Interpreter::printTomasuloTable(){
	cout<<"clock: "<<clock<<endl;
	printf("name  b op  vj  vk qj qk  d ctf\n");
	for(vector<TomasuloTable>::iterator it = tomasuloTable.begin(); it!=tomasuloTable.end(); it++){
		cout<<it->name<<" "<<it->busy<<" ";
		printf("%2d %3d %3d %2d %2d %2d %3d\n", it->op, it->vj, it->vk, it->qj, it->qk, it->d, it->clockToFinish);
	}
	cout<<endl;
}
