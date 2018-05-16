#ifndef MAIN_WIDGET
#define MAIN_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

class MainWidget: public QWidget{
Q_OBJECT
public:
	int current;
	QPushButton* playButton;
	QGridLayout* layout;
	QListWidget* list;
	MainWidget();
public slots:
	void nextEntris();

};

#endif
