#include <Window.h>

Window::Window(){
	mainWidget = new MainWidget();
	setCentralWidget(mainWidget);
}
