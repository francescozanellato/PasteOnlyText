/*
 * Author: Francesco Paolo Luca Zanellato
 * Copyright (C) 2015-2025 Francesco Paolo Luca Zanellato
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

#ifndef DIALOGIMAGECROPPREVIEW_H
#define DIALOGIMAGECROPPREVIEW_H

#include <QDialog>

namespace Ui {
class DialogImageCropPreview;
}

class DialogImageCropPreview : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageCropPreview(QWidget *parent = 0);
    ~DialogImageCropPreview();
    void setImage(QImage, int cropTop, int cropBottom, int cropLeft, int cropRight);
    QPixmap *pix;
    int getCropBottom();
    int getCropTop();
    int getCropLeft();
    int getCropRight();
    int cropTopOrig;
    int cropBottomOrig;
    int cropLeftOrig;
    int cropRightOrig;

//protected:
//    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void doCropBottom(int);
    void doCropTop(int);
    void doCropLeft(int);
    void doCropRight(int);
    void doReset(bool checked);
    void doStepChange(int step);

private:
    Ui::DialogImageCropPreview *ui;
};

#endif // DIALOGIMAGECROPPREVIEW_H
