#ifndef WINDOW
#define WINDOW

#include <qapplication.h>
#include <cstdio>
#include <qpushbutton.h>
#include <QMainWindow>
#include <QListWidget>
#include <MainWidget.h>


class Window : public QMainWindow{
Q_OBJECT
public:
	Window();
	MainWidget* mainWidget;

};

#endif

