#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>

QImage blurImage(const QImage &source, int radius) {
    if (source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));

    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->open, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::applyBlurEffect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Выберите изображение"), "", tr("Image files (*.jpg *.png)"));
    if (!filePath.isEmpty()) {
        originalPixmap = QPixmap(filePath);
        if (!originalPixmap.isNull()) {
            int labelWidth = ui->picture->width();
            int labelHeight = ui->picture->height();

            originalPixmap = originalPixmap.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio);
            ui->picture->setPixmap(originalPixmap);
            applyBlurEffect(ui->horizontalSlider->value());
        }
    }
}

void MainWindow::applyBlurEffect(int radius)
{
    currentBlurRadius = radius;
    if (!originalPixmap.isNull()) {
        QImage blurredImage = blurImage(originalPixmap.toImage(), currentBlurRadius);
        ui->picture->setPixmap(QPixmap::fromImage(blurredImage));
    }
}
