#ifndef MAIN_WIDGET
#define MAIN_WIDGET

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>
#include <QTableWidget>
#include <Interpreter.h>
#include <QLabel>

class MainWidget: public QWidget{
Q_OBJECT
public:
	MainWidget();

private slots:
	void nextEntris();
	void nextStep();

private:
	void integrateInstructionTable();
	void integrateRegisterTable();
	void integrateReservationTable();
	void integrateDataTable();
	void updateMemoryTable();
	void updateDataTable();
	void updateReservationTable();
	void updateRegister(int, int, int);
	void paintReservationTable(QColor, int);
	void setToRunning(int, int);
	int current;
	int lastRow;
	QPushButton* playButton;
	QPushButton* stepButton;
	QGridLayout* layout;
	QTableWidget* instructionTable;
	QTableWidget* reservationTable;
	QTableWidget* registerTable;
	QTableWidget* dataTable;
	QTableWidget* memoryTable;
	Interpreter* interpreter;

	QGridLayout* mainLayout;

};

#endif
