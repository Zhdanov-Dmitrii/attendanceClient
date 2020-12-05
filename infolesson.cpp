#include "infolesson.h"
#include "ui_infolesson.h"

infoLesson::infoLesson(QWidget *parent, QStringList list) :
    QDialog(parent),
    ui(new Ui::infoLesson)
{
    ui->setupUi(this);

    ui->lesson->setText(list[0]);
    ui->date->setText(list[1]);
    ui->time->setText(list[2]);
    ui->lecturer->setText(list[3]);
    ui->audit->setText(list[4]);
    ui->groups->setText(list[5]);
    ui->attendance->setText(list[6]);
    ui->noAttendance->setText(list[7]);

}

infoLesson::~infoLesson()
{
    delete ui;
}
