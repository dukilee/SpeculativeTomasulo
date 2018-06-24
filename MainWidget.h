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
	void playButtonClicked();

private:
	void integrateInstructionTable();
	void integrateRegisterTable();
	void integrateReservationTable();
	void integrateQueueTable();
	void integrateDataTable();
	void updateMemoryTable();
	void updateDataTable();
	void updateReservationTable();
	void updateRegister(int, int, int);
	void updateQueueTable();
	void paintReservationTable(QColor, int);
	void setToRunning(int, int);
	void runToEnd();
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
	QTableWidget* queueTable;
	Interpreter* interpreter;

	QGridLayout* mainLayout;
	bool isRunning;

};

#endif
