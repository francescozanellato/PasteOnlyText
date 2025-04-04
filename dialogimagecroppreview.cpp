/*
 * Author: Francesco Paolo Luca Zanellato
 * Copyright (C) 2015-2025 Francesco Zanellato
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "dialogimagecroppreview.h"
#include "ui_dialogimagecroppreview.h"
//#include <QDesktopWidget>
//#include <QDesktopServices>
#include <QScreen>

DialogImageCropPreview::DialogImageCropPreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageCropPreview)
{
    ui->setupUi(this);
    ui->step->setValue(10);
    ui->ImageLabel->setScaledContents( true );
    ui->ImageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    //connect(ui->cropBottom->textChanged, SIGNAL(void ()), this, SLOT(doCropBottom(const QString)));
    connect(ui->cropBottom, SIGNAL(valueChanged(int)), this, SLOT(doCropBottom(int)));
    connect(ui->cropTop, SIGNAL(valueChanged(int)), this, SLOT(doCropTop(int)));
    connect(ui->cropLeft, SIGNAL(valueChanged(int)), this, SLOT(doCropLeft(int)));
    connect(ui->cropRight, SIGNAL(valueChanged(int)), this, SLOT(doCropRight(int)));
    connect(ui->reset, SIGNAL(clicked(bool)), this, SLOT(doReset(bool)));
    connect(ui->step, SIGNAL(valueChanged(int)), this, SLOT(doStepChange(int)));
}

DialogImageCropPreview::~DialogImageCropPreview()
{
    delete ui;
}

//void DialogImageCropPreview::resizeEvent(QResizeEvent *event)
//{
    /*if (width() > image.width() || height() > image.height()) {
        ui->ImageLabel->setPixmap(pix->scaled(ui->ImageLabel->width(),ui->ImageLabel->height(),Qt::KeepAspectRatio));
        update();
    } else {
    }*/
//    QWidget::resizeEvent(event);
//}

void DialogImageCropPreview::setImage(QImage image, int cropTop, int cropBottom, int cropLeft, int cropRight)
{
    //QImage *img = new QImage(image);
    pix = new QPixmap();
    pix->convertFromImage(image);
    ui->cropTop->setValue(cropTop);
    ui->cropTop->setSingleStep(ui->step->value());
    ui->cropBottom->setValue(cropBottom);
    ui->cropBottom->setSingleStep(ui->step->value());
    ui->cropLeft->setValue(cropLeft);
    ui->cropLeft->setSingleStep(ui->step->value());
    ui->cropRight->setValue(cropRight);
    ui->cropRight->setSingleStep(ui->step->value());
    cropTopOrig = cropTop;
    cropBottomOrig = cropBottom;
    cropLeftOrig = cropLeft;
    cropRightOrig = cropRight;
    //pix->scaled(400,400,Qt::KeepAspectRatio);
    //QPixmap QPixmap::scaled(int width, int height, Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio, Qt::TransformationMode transformMode = Qt::FastTransformation) const
    if (pix->isNull()) return;
    //QDesktopWidget desktop;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    //int desktopHeight=desktop.geometry().height()-200;
    int desktopHeight=screenGeometry.height()-200;
    int desktopWidth=screenGeometry.width()-400;
    /*QRect rec = qApp->desktop()->screenGeometry();
     int height = rec.height()-30;
     int width = rec.width()-30;*/

    ui->sizeLabel->setText(QVariant(pix->width()).toString()+"x"+QVariant(pix->height()).toString());
    //ui->ImageLabel->setPixmap(pix->scaled(800,800,Qt::KeepAspectRatio));
    //ui->ImageLabel->setPixmap(pix->scaled(width(),height(),Qt::KeepAspectRatio));
    //ui->ImageLabel->setPixmap(pix->copy());
    resize(qMin(desktopWidth,pix->width()),qMin(desktopHeight,pix->height()));
    QRect rect(ui->cropLeft->value(), ui->cropTop->value(), pix->width()-ui->cropLeft->value()-ui->cropRight->value(),
               pix->height()-ui->cropTop->value()-ui->cropBottom->value());
     ui->ImageLabel->setPixmap(pix->copy(rect).scaledToWidth(width()));
    //ui->sizeLabel->setText("ciao");
}

int DialogImageCropPreview::getCropLeft()
{
  return ui->cropLeft->value();
}

int DialogImageCropPreview::getCropRight()
{
  return ui->cropRight->value();
}

int DialogImageCropPreview::getCropTop()
{
  return ui->cropTop->value();
}

int DialogImageCropPreview::getCropBottom()
{
  return ui->cropBottom->value();
}

void DialogImageCropPreview::doStepChange(int step)
{
    ui->cropBottom->setSingleStep(step);
    ui->cropTop->setSingleStep(step);
    ui->cropLeft->setSingleStep(step);
    ui->cropRight->setSingleStep(step);
}

void DialogImageCropPreview::doReset(bool checked)
{
 if (checked) {
     ui->cropBottom->setValue(0);
     ui->cropTop->setValue(0);
     ui->cropLeft->setValue(0);
     ui->cropRight->setValue(0);
 } else {
     ui->cropBottom->setValue(cropBottomOrig);
     ui->cropTop->setValue(cropTopOrig);
     ui->cropLeft->setValue(cropLeftOrig);
     ui->cropRight->setValue(cropRightOrig);
 }
}

void DialogImageCropPreview::doCropBottom(int)
{
    //int cropX = cropLeft;
    //int cropY = cropTop;
    //int cropWidth = image.width() - cropRight - cropLeft;
    //int cropHeight = image.height() - cropBottom - cropTop;
    //QRect rect(cropX, cropY, cropWidth, cropHeight);
    //pix = ui->ImageLabel->pixmap();
    if (pix->isNull()) return;
    QRect rect(ui->cropLeft->value(), ui->cropTop->value(), pix->width()-ui->cropLeft->value()-ui->cropRight->value(),
               pix->height()-ui->cropTop->value()-ui->cropBottom->value());
     ui->ImageLabel->setPixmap(pix->copy(rect).scaledToWidth(ui->ImageLabel->width()));
}

void DialogImageCropPreview::doCropTop(int)
{
    //int cropX = cropLeft;
    //int cropY = cropTop;
    //int cropWidth = image.width() - cropRight - cropLeft;
    //int cropHeight = image.height() - cropBottom - cropTop;
    //QRect rect(cropX, cropY, cropWidth, cropHeight);
    //pix = ui->ImageLabel->pixmap();
    if (pix->isNull()) return;
    QRect rect(ui->cropLeft->value(), ui->cropTop->value(), pix->width()-ui->cropLeft->value()-ui->cropRight->value(),
               pix->height()-ui->cropTop->value()-ui->cropBottom->value());
     ui->ImageLabel->setPixmap(pix->copy(rect).scaledToWidth(ui->ImageLabel->width()));
}

void DialogImageCropPreview::doCropLeft(int)
{
    //int cropX = cropLeft;
    //int cropY = cropTop;
    //int cropWidth = image.width() - cropRight - cropLeft;
    //int cropHeight = image.height() - cropBottom - cropTop;
    //QRect rect(cropX, cropY, cropWidth, cropHeight);
    //pix = ui->ImageLabel->pixmap();
    if (pix->isNull()) return;
    QRect rect(ui->cropLeft->value(), ui->cropTop->value(), pix->width()-ui->cropLeft->value()-ui->cropRight->value(),
               pix->height()-ui->cropTop->value()-ui->cropBottom->value());
     ui->ImageLabel->setPixmap(pix->copy(rect).scaledToWidth(ui->ImageLabel->width()));
}

void DialogImageCropPreview::doCropRight(int)
{
    //int cropX = cropLeft;
    //int cropY = cropTop;
    //int cropWidth = image.width() - cropRight - cropLeft;
    //int cropHeight = image.height() - cropBottom - cropTop;
    //QRect rect(cropX, cropY, cropWidth, cropHeight);
    //pix = ui->ImageLabel->pixmap();
    if (pix->isNull()) return;
    QRect rect(ui->cropLeft->value(), ui->cropTop->value(), pix->width()-ui->cropLeft->value()-ui->cropRight->value(),
               pix->height()-ui->cropTop->value()-ui->cropBottom->value());
     ui->ImageLabel->setPixmap(pix->copy(rect).scaledToWidth(ui->ImageLabel->width()));
}
