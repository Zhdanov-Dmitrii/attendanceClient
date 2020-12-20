#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>

namespace Ui {
class authentication;
}

class authentication : public QDialog
{
    Q_OBJECT

public:
    explicit authentication(QWidget *parent = nullptr);
    ~authentication();

signals:
    void enter(QString& login, QString& password);

private slots:
    void on_pushButton_clicked();

private:
    Ui::authentication *ui;
};

#endif // AUTHENTICATION_H
