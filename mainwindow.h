#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>


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

private slots:
    void on_search_lecture_clicked();

private:
    QTcpSocket *socket;
    QByteArray data;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
