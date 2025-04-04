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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMimeData>
#include "dialogimagecroppreview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *timer;
    QClipboard *clipboard;
    QString currentText;
    QString originalClipboardText;
    bool timerStopped;
    QSettings *settings;
    QString replaceText;
    QString withText;
    QString replaceText_default;
    QString withText_default;
    int timer_ms;
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    QStringList arguments;
    QMimeData *urlsMime;
    QImage image;
    int imageNumber;
    int imageNumberDigits;
    bool crop;
    int cropRight;
    int cropLeft;
    int cropTop;
    int cropBottom;
    QString imageFilename;
    QString imageExtension;
    QString imagePath;
    QByteArray mimeDataOrig;
    QStringList replaceTextList;
    QString replaceTextListSeparator;
    QStringList withTextList;
    QStringList undoTextList;
    int currentUndoIndex;
    QStringList macros;
    int numMacros;
    DialogImageCropPreview *imageWin;

public slots:
    void doQuit();
    void doShowLicense();
    void showAboutQt();
    void doShowRegularExpressions();
    void allUppercase();
    void allLowercase();
    void allLowerCaseWithFirstUppercase();
    void stopTimer();
    void startTimer();
    void setStartTimer();
    void allInitialsUppercase();
    void initialsUppercase(bool firstLowerCase);
    void allOnlyInitialsUppercase();
    void resetCase();
    //void setEditableTextbox(bool readOnly);
    void rimuovi1();
    void loadSettings();
    void saveSettings();
    void resetReplaceOptions();
    void swapReplaceOptions();
    void copyFoldersRecursively(QString sourceFolder, QString destFolder);
    void doCopyFolders();
    void doCopyFoldersRecursively();
    void CopyFolders(bool recursive);
    void doSaveImage(bool ask);
    void doSaveImageAsk();
    int getImageNumber();
    void doOpenSettings();
    void doOpenImageDirOnExit();
    void doCopyUrlsToClipboard();
    void doEmailWithLink();
    void doReplaceInText();
    void doCropImageAsk(bool ask);
    void doCropImage();
    void doCropImageWithPreview();
    //void doCropAfterPreview();
    void doUndo();
    void doRedo();
    void addToUndo();
    void doApplyModifications();
    void doFocusOnEditText();
    void loadMacro(QString currentMacro);
    void loadSelectedMacro();
    void keyPressEvent(QKeyEvent * event);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
