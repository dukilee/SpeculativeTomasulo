#include <MainWidget.h>
#include <cstdio>

MainWidget::MainWidget(){
	current = 0;
	list = new QListWidget;
	playButton = new QPushButton("Play");
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));

	for(int i = 0; i<12; i++){
		list->addItem("primeiro");
		list->addItem("segundo");
		list->addItem("terceiro");
	}

	layout = new QGridLayout();
	layout->addWidget(list, 0, 0);
	layout->addWidget(playButton, 1, 1);
	setLayout(layout);
	nextEntris();
}

void MainWidget::nextEntris(){
	current++;
	list->setCurrentRow(current);
}
