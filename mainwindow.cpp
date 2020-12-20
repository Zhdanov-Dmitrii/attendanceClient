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

    ui->lecture_tableWidget->sortByColumn(0,Qt::AscendingOrder);

    ss = false;
    changeTable = true;
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

            QStringList name;
            name << docArr[0].toObject().value("fio").toString();;
            for(int i = 1; i < docArr.count() && docArr[i].toObject().value("fio").toString() != name[0]; i++)
                name << docArr[i].toObject().value("fio").toString();


            QStringList date;
            for(int i = 0; i < docArr.count(); i += name.size())
                date << docArr[i].toObject().value("date").toString();


            ui->tableWidget->setRowCount(name.size());
            ui->tableWidget->setColumnCount(date.size());
            ui->tableWidget->setHorizontalHeaderLabels(date);
            ui->tableWidget->setVerticalHeaderLabels(name);

            int k = 0;
            changeTable = false;
            for(int i = 0; i < date.size();i++)
            {
                for(int j = 0; j < name.size();j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(docArr[k].toObject().value("+-").toString());
                    if(docArr[k].toObject().value("+-").toString() == "+")
                        item->setBackgroundColor(QColor(0,255,0));
                    else
                        item->setBackgroundColor(QColor(255,0,0));

                    ui->tableWidget->setItem(j, i, item);
                    k++;
                }
            }
            ui->tableWidget->resizeColumnsToContents();
            changeTable = true;

        }
        else if(doc.object().value("type").toString() == "result face recognition")
        {
            QJsonArray docArr = doc.object().value("results").toArray();
            QStringList data;
            for(int i = 0; i < docArr.count();i++)
            {
                data << docArr[i].toObject().value("name").toString();
                data << docArr[i].toObject().value("group").toString();
                data << docArr[i].toObject().value("status").toString();
            }
            cf->writeTable(data);
        }
        else if(doc.object().value("type").toString() == "result select lesson info")
        {
            QString audit = doc.object().value("audit").toString();
            QString foto = doc.object().value("foto").toString();
            qDebug() << foto;
            info->setFoto(audit, foto);
        }
        else if(doc.object().value("type").toString() == "result login")
        {
            QString res = doc.object().value("result").toString();
            if(res == "1")
            {
                QMessageBox::information(this,"Авторизация", "Вы успешно вошли");
                auth->close();
                ui->pushButton_4->setText("Выйти из системы");
                ss = true;
            }
            else
            {
                QMessageBox::critical(this, "Ошибка", "Неправильный логин или пароль");
                ss = false;
            }
        }
    }
    else
    {
        QMessageBox::information(this,"Оповещение", "Нет данных");
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
    if(!ss)
    {
        QMessageBox::critical(this, "Ошибка доступа", "Вы не авторизованы");
        return;
    }

    cf = new createFoto(this);
    cf->show();

    connect(cf,SIGNAL(foto(bool)), this, SLOT(queryFoto(bool)));




    //QString pyCommand("python D:\\project\\attendance\\main.py \n"); //try with out " \n" also...

    //QProcess *qprocess = new QProcess(this);
    //qprocess->start("cmd");
    //qprocess->write(pyCommand.toLatin1().data());
}

void MainWindow::on_lecture_tableWidget_cellDoubleClicked(int row, int column)
{
    QString lessonName = ui->lecture_tableWidget->item(row,0)->text();
    QString lessonLecturer = ui->lecture_tableWidget->item(row,1)->text();
    QString groupName = ui->lecture_tableWidget->item(row,2)->text();

    ui->infoLesson->setText(lessonName);
    ui->infoLecturer->setText(lessonLecturer);
    ui->infoGroup->setText(groupName);

    //{"type":"select attendance", "lessonName":"", "lessonLecturer":"", "groupName":""}
    QString select ="{\"type\":\"select attendance\", \"lessonName\":\"" + lessonName + "\", \"lessonLecturer\":\""+lessonLecturer+"\", \"groupName\":\""+groupName+"\"}";
    socket->write(select.toUtf8());
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->lecture_tableWidget->sortByColumn(index,Qt::AscendingOrder);
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //{"type":"select lesson info","date":"","lessonName":"",,"lecturer":"", "time":""}
    QString fio, lessonName, lessonTime, date, lecturer;
    lecturer = ui->infoLecturer->text();
    fio = ui->tableWidget->verticalHeaderItem(row)->text();
    lessonName = ui->infoLesson->text();
    date = ui->tableWidget->horizontalHeaderItem(column)->text();
    lessonTime = date;
    lessonTime.remove(0, lessonTime.indexOf('(')+1);
    lessonTime.remove(')');
    date.remove(date.lastIndexOf(' '), date.size()-date.lastIndexOf(' '));

    QString select = "{\"type\":\"select lesson info\",\"date\":\"" +
                        date +"\",\"lessonName\":\""+
                        lessonName +"\",\"lecturer\":\""
                       +lecturer+"\", \"time\":\""+
                        lessonTime+"\"}";
    socket->write(select.toUtf8());

    QStringList list;
    list << ui->infoLesson->text();
    list << ui->tableWidget->horizontalHeaderItem(column)->text().split(' ');

    list[2].remove('(');
    list[2].remove(')');

    list << ui->infoLecturer->text();
    list << "тут может быть ваша реклама";
    list << ui->infoGroup->text();



    int count = 0;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        if(ui->tableWidget->item(i, column)->text() == "+")
            count++;

    list << QString::number(count) << QString::number(ui->tableWidget->rowCount()-count);


    info = new infoLesson(this, list);
    info->show();


}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if(!changeTable)
        return;

    if(!ss)
    {
        QMessageBox::critical(this, "Ошибка доступа", "Вы не авторизованы");
        return;
    }

    QString fio, lessonName, lessonTime, date;
    fio = ui->tableWidget->verticalHeaderItem(row)->text();
    lessonName = ui->infoLesson->text();

    date = ui->tableWidget->horizontalHeaderItem(column)->text();
    lessonTime = date;
    lessonTime.remove(0, lessonTime.indexOf('(')+1);
    lessonTime.remove(')');
    date.remove(date.lastIndexOf(' '), date.size()-date.lastIndexOf(' '));

    qDebug() << fio << lessonName << lessonTime << date;

    QString select = "{\"type\":\"update student status\", ";
    select += "\"fio\":\""+fio+"\", \"lessonName\":\""+lessonName+"\", \"lessonTime\":\""+lessonTime+"\", \"date\":\""+date+"\", \"status\":\""+
            ui->tableWidget->item(row, column)->text()+"\"}";
    socket->write(select.toUtf8());

    changeTable = false;
    if(ui->tableWidget->item(row, column)->text() == "-")
        ui->tableWidget->item(row, column)->setBackgroundColor(QColor(255,0,0));
    else
        ui->tableWidget->item(row, column)->setBackgroundColor(QColor(0,255,0));
    changeTable = true;
}

void MainWindow::queryFoto(bool status)
{
    QFile foto(QCoreApplication::applicationDirPath()+"/foto.jpg");
    if(!foto.open(QFile::ReadOnly))
        return;
    //название пары, время, день недели, аудитория
    QByteArray query = "{\"type\":\"face recognition\", \"lesson name\":\"";
    query += ui->currentLesson->text();
    query += "\",\"time\":\"";
    query += ui->currentTime->text();
    query += "\", \"audit\":\"";
    query += ui->currentAudit->text();
    query += "\", \"status\":\"";
    if(status)
        query += "insert";
    else
        query += "update";
    query += "\"}";
    query += foto.readAll();
    socket->write(query);
    foto.close();
    foto.remove();
}

void MainWindow::on_searchStudent_clicked()
{
    QString fio = ui->lineEditFio->text();
    QString group = ui->lineEditGroupStudent->text();

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->showRow(i);

        QString tFio = ui->tableWidget->verticalHeaderItem(i)->text();
        QString tGroup = tFio;

        tGroup.remove(0,tFio.indexOf('(')+1);
        tGroup.remove(')');

        tFio.remove(tFio.lastIndexOf(' '), tFio.size()-tFio.lastIndexOf(' '));

        if(!(tFio.contains(fio,Qt::CaseInsensitive) && tGroup.contains(group,Qt::CaseInsensitive)))
            ui->tableWidget->hideRow(i);

    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ss)
    {
        ss = false;
        ui->pushButton_4->setText("Войти как преподаватель");
        return;
    }

    auth = new authentication(this);
    connect(auth, SIGNAL(enter(QString&, QString&)), this, SLOT(login(QString&, QString&)));
    auth->show();
}

void MainWindow::login(QString& login, QString& password)
{
    socket->write("{\"type\":\"login\",\"login\":\""+login.toUtf8()+"\",\"password\":\""+password.toUtf8()+"\"}");
}
