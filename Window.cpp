#include <Window.h>

Window::Window(){
	QPushButton* hello;
	mainWidget = new MainWidget();
	setCentralWidget(mainWidget);
}
