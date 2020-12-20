#include "authentication.h"
#include "ui_authentication.h"

authentication::authentication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authentication)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

authentication::~authentication()
{
    delete ui;
}

void authentication::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();

    emit enter(login, password);
}
