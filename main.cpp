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
}

