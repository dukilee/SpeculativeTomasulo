#include <MainWidget.h>

using namespace std;

MainWidget::MainWidget(){
	lastRow = -1;
	interpreter = new Interpreter("../fully.txt");

	instructionTable = new QTableWidget;
	reservationTable = new QTableWidget;
	registerTable = new QTableWidget;
	dataTable = new QTableWidget;
	memoryTable = new QTableWidget;

	playButton = new QPushButton(tr("&Play"));
	stepButton = new QPushButton(tr("&Step"));
	connect(playButton, SIGNAL(clicked()), this, SLOT(nextEntris()));
	connect(stepButton, SIGNAL(clicked()), this, SLOT(nextStep()));

	integrateInstructionTable();
	integrateRegisterTable();
	integrateReservationTable();
	integrateDataTable();

	QStringList sList;
	sList<<"Memory";
	memoryTable->insertRow(0);
	memoryTable->setVerticalHeaderLabels(sList);

	mainLayout = new QGridLayout();
	mainLayout->addWidget(instructionTable, 0, 0, 3, 2);
	mainLayout->addWidget(dataTable, 3, 0, 2, 1);
	mainLayout->addWidget(reservationTable, 0, 1, 1, 1);
	mainLayout->addWidget(registerTable, 2, 1, 1, 1);
	mainLayout->addWidget(memoryTable, 3, 1, 1, 1);
	//mainLayout->addWidget(playButton, 2, 0, Qt::AlignRight);
	mainLayout->addWidget(stepButton, 5, 1, Qt::AlignRight);
	setLayout(mainLayout);
}

void MainWidget::updateMemoryTable(){
	QStringList sList;
	int cont = 0;
	
	for(map<int, map<int, int> >::iterator it = interpreter->memory.begin(); it!=interpreter->memory.end();it++){

		for(map<int, int>::iterator it2 = it->second.begin(); it2!=it->second.end(); it2++){

			sList << tr("%1(").arg(it->first)+tr("%1)").arg(it2->first);
			QTableWidgetItem* item = memoryTable->item(0, cont);


			if(item == NULL) {
				item = new QTableWidgetItem(tr("%1").arg(it2->second));
				memoryTable->insertColumn(cont);
				memoryTable->setItem(0, cont, item);

			}else{
				item->setText(tr("%1").arg(it2->second));
			}
			cont++;
		}
	}
	if(cont>0)
		memoryTable->setHorizontalHeaderLabels(sList);
}

void MainWidget::updateReservationTable(){
	int row = -1;
	for(vector<TomasuloTable>::iterator it = interpreter->tomasuloTable.begin(); it!=interpreter->tomasuloTable.end(); it++){
		row++;

		reservationTable->item(row, 0)->setText(tr("%1").arg(it->busy));
		if(it->busy)
			reservationTable->item(row, 1)->setText(QString::fromStdString(interpreter->converteAtribPraNome(interpreter->listCommands[it->op].atrib)));

		if(it->qj<0 && it->busy)
			reservationTable->item(row, 3)->setText(tr("%1").arg(it->vj));
		else if(it->busy)
			reservationTable->item(row, 3)->setText("");
		if(it->qk<0 && it->busy)
			reservationTable->item(row, 4)->setText(tr("%1").arg(it->vk));
		else if(it->busy)
			reservationTable->item(row, 4)->setText("");

		if(it->busy)
			reservationTable->item(row, 5)->setText(tr("%1").arg(it->qj+1));

		if(it->busy)
			reservationTable->item(row, 6)->setText(tr("%1").arg(it->qk+1));

		if(it->busy)
			reservationTable->item(row, 7)->setText(QString::fromStdString(interpreter->listCommands[it->op].d));



		if(!it->busy && reservationTable->item(row, 2)->text()!="")
			reservationTable->item(row, 2)->setText("Recorded");
		if(it->busy){
			reservationTable->item(row, 2)->setText("Emited");
		}
	}
	setToRunning(interpreter->firstLoads, interpreter->lastLoads);
	setToRunning(interpreter->firstAdds, interpreter->lastAdds);
	setToRunning(interpreter->firstMults, interpreter->lastMults);
}

void MainWidget::setToRunning(int start, int finish){
	int min, id;
	min = 9999999;
	id = -1;
	for(int i = start; i<=finish; i++){
		if(!interpreter->tomasuloTable[i].busy || interpreter->tomasuloTable[i].qk>=0 || interpreter->tomasuloTable[i].qj>=0) continue;
		if(interpreter->tomasuloTable[i].clockToFinish<min){
			min = interpreter->tomasuloTable[i].clockToFinish;
			id = i;
		}
	}
	if(id>=0){
		reservationTable->item(id, 2)->setText("Running");
	}

}

void MainWidget::integrateReservationTable(){
	QStringList sListHor, sListVer;
	sListHor<<"busy"<<"op"<<"state"<<"vj"<<"vk"<<"qj"<<"qk"<<"a";
	for(int i = 0; i<8; i++){
		reservationTable->insertColumn(i);
	}
	reservationTable->setHorizontalHeaderLabels(sListHor);
	for(vector<TomasuloTable>::iterator it = interpreter->tomasuloTable.begin(); it!=interpreter->tomasuloTable.end(); it++){
		int row = reservationTable->rowCount();
		reservationTable->insertRow(row);
		sListVer<<QString::fromStdString(it->name);

		for(int i = 0; i<8; i++){
			reservationTable->setItem(row, i, new QTableWidgetItem(""));

		}
	}
	reservationTable->setVerticalHeaderLabels(sListVer);
}

void MainWidget::integrateDataTable(){
	QStringList sList;
	sList<<"Clock"<<"CLI"<<"PC"<<"Instructions";
	dataTable->insertColumn(0);
	for(int i = 0; i<4; i++){
		dataTable->insertRow(i);
		dataTable->setItem(i, 0, new QTableWidgetItem("0"));
	}
	dataTable->setVerticalHeaderLabels(sList);
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
	if(!interpreter->runNextLine()){}


		//return;

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


	updateDataTable();
	updateMemoryTable();


	update();
}

void MainWidget::updateDataTable(){
	dataTable->item(0, 0)->setText(tr("%1").arg(interpreter->clock));
	dataTable->item(1, 0)->setText(tr("%1").arg(interpreter->runnedCommands/1.0/interpreter->clock));
	dataTable->item(2, 0)->setText(tr("%1").arg(current+1));
	dataTable->item(3, 0)->setText(tr("%1").arg(interpreter->runnedCommands));
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
