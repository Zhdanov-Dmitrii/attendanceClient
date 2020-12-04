#include "createfoto.h"
#include "ui_createfoto.h"

createFoto::createFoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createFoto)
{
    ui->setupUi(this);
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    camera = new QCamera(cameras[0],this);
    cameraView = new QCameraViewfinder(this);

    cameraView->setMinimumSize(128,128 );


    ui->verticalLayout->addWidget(cameraView,0, Qt::AlignLeft | Qt::AlignTop);
    cameraView->setMaximumHeight(400);
    cameraView->show();
    camera->setViewfinder(cameraView);

    camera->start();



}

createFoto::~createFoto()
{
    delete ui;
}
