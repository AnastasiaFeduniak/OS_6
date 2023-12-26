#include "enterchar.h"
#include "ui_enterchar.h"
int n = -1;
EnterChar::EnterChar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterChar)
{
    ui->setupUi(this);
}

EnterChar::~EnterChar()
{
    delete ui;
}

void EnterChar::on_pushButton_clicked()
{
    emit returnChar(ui->lineEdit->text()[0].toLatin1(), n);
    ui->lineEdit->clear();
    close();
}


void EnterChar::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    close();
}

void EnterChar::getN(int N){
    n = N;
}
