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
    data = socket->readAll();
    qDebug() << data;
    doc = QJsonDocument::fromJson(data, &docError);

    if(docError.error == QJsonParseError::NoError)
    {
        //{"type":"result select lecture", "results":[...]}
        if(doc.object().value("type").toString() == "result select lecture")
        {
            QJsonArray docArr = doc.object().value("results").toArray();

            ui->lecture_tableWidget->setRowCount(docArr.count());
            ui->lecture_tableWidget->clearContents();
            for(int i = 0; i < docArr.count();i++)
            {
                qDebug() << docArr[i].toObject().value("lecture").toString();
                ui->lecture_tableWidget->setItem(i,0, new QTableWidgetItem(docArr[i].toObject().value("lecture").toString()));
                ui->lecture_tableWidget->setItem(i,1, new QTableWidgetItem(docArr[i].toObject().value("teacher").toString()));
                ui->lecture_tableWidget->setItem(i,2, new QTableWidgetItem(docArr[i].toObject().value("team").toString()));
            }
            ui->lecture_tableWidget->resizeColumnsToContents();
            QApplication::beep();
        }
        else if(doc.object().value("type").toString() == "result select attendance") //{\"type\":\"result select attendance\", \"results\":[...]}
        {
            QJsonArray docArr = doc.object().value("results").toArray();

            QString t = docArr[0].toObject().value("date").toString();

            QStringList name;
            for(int i = 0; t  == docArr[i].toObject().value("date").toString(); i++)
                name << docArr[i].toObject().value("fio").toString();

            QStringList date;
            for(int i = 0; i < docArr.count(); i += name.size())
                date << docArr[i].toObject().value("date").toString();


            ui->tableWidget->setRowCount(name.size());
            ui->tableWidget->setColumnCount(date.size());
            ui->tableWidget->setHorizontalHeaderLabels(date);
            ui->tableWidget->setVerticalHeaderLabels(name);

            int k = 0;
            for(int i = 0; i < name.size();i++)
            {
                for(int j = 0; j < date.size();j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(docArr[k].toObject().value("+-").toString());
                    if(docArr[k].toObject().value("+-").toString() == "+")
                        item->setBackgroundColor(QColor(0,255,0));
                    else
                        item->setBackgroundColor(QColor(255,0,0));

                    ui->tableWidget->setItem(i,j, item);
                    k++;
                }
            }


        }
    }
    else
    {
        qDebug() << docError.errorString();
    }

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

void MainWindow::on_pushButton_3_clicked()
{
    QProcess *process = new QProcess(this);
    QString file = "C:\\Users\\Дмитрий\\AppData\\Local\\GitHubDesktop\\GitHubDesktop.exe";
    process->start(file);

}

void MainWindow::on_lecture_tableWidget_cellDoubleClicked(int row, int column)
{
    QString lessonName = ui->lecture_tableWidget->item(row,0)->text();
    QString lessonLecturer = ui->lecture_tableWidget->item(row,1)->text();
    QString groupName = ui->lecture_tableWidget->item(row,2)->text();

    //{"type":"select attendance", "lessonName":"", "lessonLecturer":"", "groupName":""}
    QString select ="{\"type\":\"select attendance\", \"lessonName\":\"" + lessonName + "\", \"lessonLecturer\":\""+lessonLecturer+"\", \"groupName\":\""+groupName+"\"}";
    socket->write(select.toUtf8());
}


