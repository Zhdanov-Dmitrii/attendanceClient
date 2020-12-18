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

    if(ui->pushButton->text() == "Сделать фото")
        emit foto(true);
    else
        emit foto(false);

    ui->pushButton->setText("Переснять");

    qDebug() << QCoreApplication::applicationDirPath()+"/foto.jpg";
}

void createFoto::writeTable(QStringList data)
{
    ui->table->setRowCount(data.size()/3);
    ui->table->setColumnCount(3);
    qDebug() << ui->table->rowCount() << ui->table->columnCount();
    int k = 0;
    for(int i = 0; i < data.size()/3; ++i)
    {
        QColor clr = QColor(250,0,0);
        if(data[i+2]=="+")
            clr = QColor(0,255,0);

        for(int j = 0; j < 3; j++)
        {
            qDebug() << data[k];
            QTableWidgetItem *item = new QTableWidgetItem(data[k]);
            item->setBackgroundColor(clr);
            ui->table->setItem(i, j, item);
            k++;
        }
    }
}
