#include <qapplication.h>
#include <qpushbutton.h>
#include "Window.h"
#include <Interpreter.h>


//int main( int argc, char **argv ){
	//QApplication a( argc, argv );
	//Window window;
//
	//window.show();
	//return a.exec();
//}


int main(){
	char* nomearq;
	nomearq = (char*) malloc(255*sizeof(char));
	strcpy(nomearq, "../entrada.txt");

	Interpreter interpreter("../entrada.txt");


	for(int i = 0; i<interpreter.listCommands.size(); i++){
		//cout<<interpreter.listCommands[i].tipo<<" "<<interpreter.listCommands[i].atrib<<" "<<interpreter.listCommands[i].p1.address<endl;
		comand c = interpreter.listCommands[i];
		cout<<c.tipo<<" "<<c.atrib<<" "<<c.p1.address<<" "<<c.p2.address<<" "<<c.p3.address<<endl<<" "<<c.p2.value<<endl;;
	}

	for(map<string, int>::iterator it = interpreter.labels.begin(); it!=interpreter.labels.end(); it++){
		cout<<it->first<<" "<<it->second<<endl;
	}

	int i = 0;
	while(	interpreter.runNextLine()){
		i++;
		cout<<i<<": "<<endl;
		for(map<string, Reg>::iterator it = interpreter.reg.begin(); it!=interpreter.reg.end(); it++){
			cout<<it->first<<" "<<it->second.value<<endl;
		}

		for(map<int, map<int, int> >::iterator it = interpreter.memory.begin(); it!=interpreter.memory.end(); it++){
			for(map<int, int>::iterator it2 = it->second.begin(); it2!=it->second.end(); it2++){
				cout<<it->first<<" "<<it2->first<<" "<<it2->second<<endl;
			}
		}
		interpreter.printTomasuloTable();
		cout<<endl;
		getchar();
	}
}


