#include <MainWidget.h>

using namespace std;

MainWidget::MainWidget(){
	lastRow = -1;
	interpreter = new Interpreter("../fully.txt");

	instructionTable = new QTableWidget;
	reservationTable = new QTableWidget;
	registerTable = new QTableWidget;

	playButton = new QPushButton(tr("&Play"));
	stepButton = new QPushButton(tr("&Step"));
	clockLbl = new QLabel("0");
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));
	connect(stepButton, SIGNAL(clicked()), this, SLOT(nextStep()));

	integrateInstructionTable();
	integrateRegisterTable();
	integrateReservationTable();

	mainLayout = new QGridLayout();
	mainLayout->addWidget(instructionTable, 0, 0, 2, 1);
	mainLayout->addWidget(reservationTable, 0, 1, 1, 1);
	mainLayout->addWidget(registerTable, 1, 1, 2, 1);
	//mainLayout->addWidget(playButton, 2, 0, Qt::AlignRight);
	mainLayout->addWidget(clockLbl, 2, 0, Qt::AlignLeft);
	mainLayout->addWidget(stepButton, 2, 1, Qt::AlignRight);
	setLayout(mainLayout);
}

void MainWidget::updateReservationTable(){
	int row = -1;
	for(vector<TomasuloTable>::iterator it = interpreter->tomasuloTable.begin(); it!=interpreter->tomasuloTable.end(); it++){
		row++;

		reservationTable->item(row, 0)->setText(tr("%1").arg(it->busy));
		if(it->busy)
			reservationTable->item(row, 1)->setText(tr("%1").arg(it->op));
		else
			reservationTable->item(row, 1)->setText("");

		if(it->qj<0 && it->busy)
			reservationTable->item(row, 2)->setText(tr("%1").arg(it->vj));
		else
			reservationTable->item(row, 2)->setText("");
		if(it->qk<0 && it->busy)
			reservationTable->item(row, 3)->setText(tr("%1").arg(it->vk));
		else
			reservationTable->item(row, 3)->setText("");
		if(it->qj>=0 && it->busy)
			reservationTable->item(row, 4)->setText(tr("%1").arg(it->qj));
		else
			reservationTable->item(row, 4)->setText("");

		if(it->qk>=0 && it->busy)
			reservationTable->item(row, 5)->setText(tr("%1").arg(it->qk));
		else
			reservationTable->item(row, 5)->setText("");

		if(it->busy)
			reservationTable->item(row, 6)->setText(tr("%1").arg(it->d));
		else
			reservationTable->item(row, 6)->setText("");
	}
}

void MainWidget::integrateReservationTable(){
	QStringList sListHor, sListVer;
	sListHor<<"busy"<<"op"<<"vj"<<"vk"<<"qj"<<"qk"<<"d";
	for(int i = 0; i<7; i++){
		reservationTable->insertColumn(i);
	}
	reservationTable->setHorizontalHeaderLabels(sListHor);
	for(vector<TomasuloTable>::iterator it = interpreter->tomasuloTable.begin(); it!=interpreter->tomasuloTable.end(); it++){
		int row = reservationTable->rowCount();
		reservationTable->insertRow(row);
		sListVer<<QString::fromStdString(it->name);

		for(int i = 0; i<7; i++){
			reservationTable->setItem(row, i, new QTableWidgetItem(""));

		}
	}
	reservationTable->setVerticalHeaderLabels(sListVer);
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
		}
	}
	updateReservationTable();
	paintReservationTable(Qt::yellow, interpreter->emptyPos);


	clockLbl->setText(tr("%1").arg(interpreter->clock));
	update();
}

void MainWidget::integrateInstructionTable(){
	for(int i = 0; i<4; i++){
		instructionTable->insertColumn(i);
	}
	for(int i = 0; i<interpreter->listCommands.size(); i++){
		instructionTable->insertRow(i);
		comand c = interpreter->listCommands[i];
		QString atrib = QString::fromStdString(interpreter->converteAtribPraNome(c.atrib));
		if(atrib=="")
			atrib = QString::fromStdString(c.p1.address);
		instructionTable->setItem(i, 0, new QTableWidgetItem(atrib));
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
	for(int i = 0; i<1; i++){
		registerTable->insertRow(i);
	}
	for(int i = 0; i<11; i++){
		registerTable->insertColumn(i);
		sList << tr("R%1").arg(i);
	}
	registerTable->setHorizontalHeaderLabels(sList);

}

void MainWidget::paintReservationTable(QColor color, int row){
	QBrush brushWhite(Qt::white);
	QBrush brushColor(color);
	QTableWidgetItem* it;
	int pc = interpreter->pc;
	for(int i = 0; i<reservationTable->columnCount(); i++){
		it = reservationTable->item(lastRow, i);
		if(it!=NULL)
			it->setBackground(brushWhite);
		it = reservationTable->item(row, i);
		if(it!=NULL)
			it->setBackground(brushColor);
	}
	lastRow = row;
}

void MainWidget::nextEntris(){
	QBrush brushWhite(Qt::white);
	QBrush brushColor(Qt::cyan);
	QTableWidgetItem* it;
	int pc = interpreter->pc;
	for(int i = 0; i<instructionTable->columnCount(); i++){
		it = instructionTable->item(current, i);
		if(it!=NULL)
			it->setBackground(brushWhite);
		it = instructionTable->item(pc, i);
		if(it!=NULL)
			it->setBackground(brushColor);
	}
	current=pc;
}
