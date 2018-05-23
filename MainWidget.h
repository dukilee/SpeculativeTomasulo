#ifndef MAIN_WIDGET
#define MAIN_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>
#include <QTableWidget>
#include <Interpreter.h>

class MainWidget: public QWidget{
Q_OBJECT
public:
	MainWidget();

private slots:
	void nextEntris();

private:
	void integrateInstructionTable();
	int current;
	QPushButton* playButton;
	QGridLayout* layout;
	QTableWidget* instructionTable;
	QTableWidget* reservationTable;
	QTableWidget* registerTable;
	Interpreter* interpreter;

	QGridLayout* mainLayout;

};

#endif
