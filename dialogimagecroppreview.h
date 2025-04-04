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
