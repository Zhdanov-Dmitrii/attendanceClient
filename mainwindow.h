#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QFile>
#include <QMessageBox>

#include "infolesson.h"
#include "createfoto.h"
#include "authentication.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void sockReady();
    void sockDisc();
    void login(QString &login, QString &password);

private slots:
    void on_search_lecture_clicked();

    void on_pushButton_3_clicked();

    void on_lecture_tableWidget_cellDoubleClicked(int row, int column);

    void on_comboBox_currentIndexChanged(int index);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tableWidget_cellChanged(int row, int column);

    void queryFoto(bool status);

    void on_searchStudent_clicked();

    void on_pushButton_4_clicked();



signals:
    void listStudent(QStringList &data);

private:
    bool ss;
    bool changeTable;
    createFoto *cf;
    infoLesson *info;
    QTcpSocket *socket;
    authentication *auth;
    QByteArray data;
    QJsonDocument doc;
    QJsonParseError docError;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
