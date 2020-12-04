#ifndef CREATEFOTO_H
#define CREATEFOTO_H

#include <QDialog>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>


namespace Ui {
class createFoto;
}

class createFoto : public QDialog
{
    Q_OBJECT

public:
    explicit createFoto(QWidget *parent = nullptr);
    ~createFoto();

private:
    QCamera *camera;
    QCameraViewfinder *cameraView;
    Ui::createFoto *ui;
};

#endif // CREATEFOTO_H
