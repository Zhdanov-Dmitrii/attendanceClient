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
    cup = new QCameraImageCapture(camera);

    cameraView->setMinimumSize(128,128 );

    ui->verticalLayout->addWidget(cameraView,0, Qt::AlignLeft | Qt::AlignTop);
    cameraView->setMaximumHeight(400);
    cameraView->show();
    camera->setViewfinder(cameraView);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
}

createFoto::~createFoto()
{
    delete ui;
}

void createFoto::on_pushButton_clicked()
{
    camera->searchAndLock();
    cup->capture(QCoreApplication::applicationDirPath()+"/foto.jpg");
    camera->unlock();
    emit foto()ж

    qDebug() << QCoreApplication::applicationDirPath()+"/foto.jpg";
}
