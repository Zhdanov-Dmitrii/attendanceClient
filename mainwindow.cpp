#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    socket->connectToHost("127.0.0.1", 1234);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}

void MainWindow::sockReady()
{

}

void MainWindow::on_search_lecture_clicked()
{
    QString lecture = ui->lecture_lineEdit->text();
    QString teacher = ui->teacher_lineEdit->text();
    QString team = ui->team_lineEdit->text();

    //{"type":"select lecture", "lecture":"lecture", "teacher":"teacher", "team":"team"}
    QString select = "{\"type\":\"select lecture\", \"lecture\":\""+ lecture +"\", \"teacher\":\""+ teacher +"\", \"team\":\""+team+"\"}";

    socket->write(select.toUtf8());
}
