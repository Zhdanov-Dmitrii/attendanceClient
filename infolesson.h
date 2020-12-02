#ifndef INFOLESSON_H
#define INFOLESSON_H

#include <QDialog>

namespace Ui {
class infoLesson;
}

class infoLesson : public QDialog
{
    Q_OBJECT

public:

    explicit infoLesson(QWidget *parent = nullptr, QStringList list = QStringList());
    ~infoLesson();

private:
    Ui::infoLesson *ui;
};

#endif // INFOLESSON_H
