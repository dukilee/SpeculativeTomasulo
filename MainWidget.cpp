#include <MainWidget.h>
#include <cstdio>

MainWidget::MainWidget(){
	current = 0;
	//list = new QListWidget;
	table = new QTableWidget;
	playButton = new QPushButton("Play");
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));

	//for(int i = 0; i<12; i++){
		//list->addItem("primeiro");
		//list->addItem("segundo");
		//list->addItem("terceiro");
	//}

	table->insertColumn(0);
	table->insertColumn(1);
	for(int i = 0; i<10; i++){
		QString a = "prim";
		QString b = "letr";
		table->insertRow(i);

		table->setItem(i, 0, new QTableWidgetItem(a));
		table->setItem(i, 1, new QTableWidgetItem(b));
	}

	QTableWidgetItem* it = table->item(1, 1);
	QBrush brush(Qt::cyan);
	it->setBackground(brush);

	


	layout = new QGridLayout();
	layout->addWidget(table, 0, 0);
	layout->addWidget(playButton, 1, 1);
	setLayout(layout);
	nextEntris();
}

void MainWidget::nextEntris(){
	//current++;
	//list->setCurrentRow(current);
}
