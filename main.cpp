#include <qapplication.h>
#include <qpushbutton.h>
#include "Window.h"
#include <Interpreter.h>


int main( int argc, char **argv ){
	QApplication a( argc, argv );
	Window window;

	window.show();
	return a.exec();
}
