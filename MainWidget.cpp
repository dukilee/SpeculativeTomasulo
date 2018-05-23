#include <MainWidget.h>

using namespace std;

MainWidget::MainWidget(){
	current = 0;
	interpreter = new Interpreter("../fully.txt");

	instructionTable = new QTableWidget;
	reservationTable = new QTableWidget;
	registerTable = new QTableWidget;

	playButton = new QPushButton(tr("&Play"));
	stepButton = new QPushButton(tr("&Step"));
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));
	connect(stepButton, SIGNAL(clicked()), this, SLOT(nextStep()));

	integrateInstructionTable();
	integrateRegisterTable();


	mainLayout = new QGridLayout();
	mainLayout->addWidget(instructionTable, 0, 0, 2, 1);
	mainLayout->addWidget(reservationTable, 0, 1, 1, 1);
	mainLayout->addWidget(registerTable, 1, 1, 2, 1);
	mainLayout->addWidget(playButton, 2, 0, Qt::AlignRight);
	mainLayout->addWidget(stepButton, 2, 1, Qt::AlignRight);
	setLayout(mainLayout);
}

void MainWidget::updateRegister(int id, int value, int dataDependency){
	QTableWidgetItem* item = registerTable->item(0, id);
	QString msg = tr("%1").arg(value);

	if(dataDependency)
		msg = "#" + msg;

	if(item == NULL) {
		item = new QTableWidgetItem(msg);
		registerTable->setItem(0, id, item);
	}else{
		item->setText(msg);
	}
}

void MainWidget::nextStep(){
	nextEntris();
	if(!interpreter->runNextLine())
		return;

	int id;
	for(map<string, Reg>::iterator it = interpreter->reg.begin(); it!=interpreter->reg.end(); it++){
		id = atoi(it->first.substr(1).c_str());
		updateRegister(id, it->second.value, it->second.dataDependency);
	}




	for(map<int, map<int, int> >::iterator it = interpreter->memory.begin(); it!=interpreter->memory.end(); it++){
		for(map<int, int>::iterator it2 = it->second.begin(); it2!=it->second.end(); it2++){
			cout<<"memo: "<<it->first<<" "<<it2->first<<" "<<it2->second<<endl;
		}
	}
	interpreter->printTomasuloTable();
	cout<<endl;
}
/*
	for(map<string, Reg>::iterator it = interpreter.reg.begin(); it!=interpreter.reg.end(); it++){
		cout<<it->first<<" "<<it->second.value<<" "<<it->second.dataDependency<<endl;
	}
	for(map<int, map<int, int> >::iterator it = interpreter.memory.begin(); it!=interpreter.memory.end(); it++){
		for(map<int, int>::iterator it2 = it->second.begin(); it2!=it->second.end(); it2++){
			cout<<"memo: "<<it->first<<" "<<it2->first<<" "<<it2->second<<endl;
		}
	}
}

*/


void MainWidget::integrateInstructionTable(){
	for(int i = 0; i<4; i++){
		instructionTable->insertColumn(i);
	}
	for(int i = 0; i<interpreter->listCommands.size(); i++){
		instructionTable->insertRow(i);
		comand c = interpreter->listCommands[i];
		instructionTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(interpreter->converteAtribPraNome(c.atrib))));
		if(c.nParams>=1)
		if(c.p1.address == ""){
			instructionTable->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(c.p1.value)));
		}else{
			instructionTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(c.p1.address)));
		}

		if(c.nParams>=2)
		if(c.p2.address == ""){
			instructionTable->setItem(i, 2, new QTableWidgetItem(tr("%1").arg(c.p2.value)));
		}else{
			instructionTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(c.p2.address)));
		}

		if(c.nParams>=3)
		if(c.p3.address == ""){
			instructionTable->setItem(i, 3, new QTableWidgetItem(tr("%1").arg(c.p3.value)));
		}else{
			instructionTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(c.p3.address)));
		}

	}
}

void MainWidget::integrateRegisterTable(){
	QStringList sList;
	for(int i = 0; i<2; i++){
		registerTable->insertRow(i);
	}
	for(int i = 0; i<11; i++){
		registerTable->insertColumn(i);
		sList << tr("R%1").arg(i);
	}
	registerTable->setHorizontalHeaderLabels(sList);

}

void MainWidget::nextEntris(){
	QBrush brushWhite(Qt::white);
	QBrush brushColor(Qt::cyan);
	QTableWidgetItem* it;
	for(int i = 0; i<instructionTable->columnCount(); i++){
		it = instructionTable->item(current-1, i);
		if(it!=NULL)
			it->setBackground(brushWhite);
		it = instructionTable->item(current, i);
		if(it!=NULL)
			it->setBackground(brushColor);
	}
	current++;
}
