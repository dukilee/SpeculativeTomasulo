#include <MainWidget.h>
#include <cstdio>

MainWidget::MainWidget(){
	current = 0;

	instructionTable = new QTableWidget;
	reservationTable = new QTableWidget;
	registerTable = new QTableWidget;

	playButton = new QPushButton("Play");
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));

	//for(int i = 0; i<12; i++){
		//list->addItem("primeiro");
		//list->addItem("segundo");
		//list->addItem("terceiro");
	//}

	instructionTable->insertColumn(0);
	instructionTable->insertColumn(1);
	for(int i = 0; i<10; i++){
		QString a = "prim";
		QString b = "letr";
		instructionTable->insertRow(i);

		instructionTable->setItem(i, 0, new QTableWidgetItem(a));
		instructionTable->setItem(i, 1, new QTableWidgetItem(b));
	}




	mainLayout = new QGridLayout();
	mainLayout->addWidget(instructionTable, 0, 0, 2, 1);
	mainLayout->addWidget(reservationTable, 0, 1, 1, 2);
	mainLayout->addWidget(registerTable, 1, 1, 1, 2);
	mainLayout->addWidget(playButton, 2, 2, Qt::AlignRight);
	setLayout(mainLayout);
	nextEntris();
}

void MainWidget::nextEntris(){
	QTableWidgetItem* it = instructionTable->item(current, 1);
	QBrush brush(Qt::white);
	it->setBackground(brush);
	current++;
	it = instructionTable->item(current, 1);
	QBrush brush2(Qt::cyan);
	it->setBackground(brush2);

	//list->setCurrentRow(current);
}
