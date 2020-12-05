#ifndef CREATEFOTO_H
#define CREATEFOTO_H

#include <QDialog>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QCameraImageCapture>


namespace Ui {
class createFoto;
}

class createFoto : public QDialog
{
    Q_OBJECT

public:
    explicit createFoto(QWidget *parent = nullptr);
    ~createFoto();

signals:
    void foto();

private slots:
    void on_pushButton_clicked();

private:
    QCamera *camera;
    QCameraViewfinder *cameraView;
    QCameraImageCapture *cup;
    Ui::createFoto *ui;
};

#endif // CREATEFOTO_H
