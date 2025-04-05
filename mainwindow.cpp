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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QTimer>
#include <QProcess>
#include <QFile>
#include <QMessageBox>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QDir>
#include <QDate>
#include <QDesktopServices>
#include <QFileDialog>
#include <qmath.h>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString cfgFile = QCoreApplication::applicationDirPath()+"/PasteOnlyText.txt";
    settings = new QSettings(cfgFile,QSettings::IniFormat,this);
    loadSettings();
    ui->setupUi(this);
    ui->replaceEdit->setEditText(replaceText);
    ui->replaceWithEdit->setEditText(withText);
    ui->text->setUndoRedoEnabled(true);
    //for (int i = 0; i < replaceTextList.size(); i++)
    ui->replaceEdit->addItems(replaceTextList);
    ui->replaceWithEdit->addItems(withTextList);
    ui->macros->addItems(macros);
    clipboard = QApplication::clipboard();
    QStringList arguments = QCoreApplication::arguments();
    currentUndoIndex = 0;

    if (arguments.size() > 1) {
        if (arguments.at(1)=="-replace" && arguments.size() >= 4) {
            ui->replaceEdit->setEditText(arguments.at(2));
            ui->replaceWithEdit->setEditText(arguments.at(3));
            arguments.removeAt(0);
            arguments.removeAt(0);
            arguments.removeAt(0);
            arguments.removeAt(0);
            currentText = arguments.join("\n");
        } else {
            arguments.removeAt(0);
            currentText = arguments.join("\n");
        }
    } else {
        const QMimeData *mimeData = clipboard->mimeData();
        mimeDataOrig = mimeData->data("text/html");
        if (mimeData->hasUrls()) {
            QList<QUrl> urls = mimeData->urls();
            QString urlsString;
            for (int i = 0; i< urls.size();i++) {
                urlsString.append(urls.at(i).toString().replace("file:///","")+"\n");
            }
            ui->text->setPlainText(urlsString);
            ui->statusBar->showMessage(QVariant(urls.size()).toString()+" file(s) dropped!");
            currentText = urlsString;
        }  else if (mimeData->hasText()) {
            currentText = clipboard->text();
        } else if (mimeData->hasImage()) {
            currentText = imagePath+"/"+imageFilename+"."+imageExtension;
            image = qvariant_cast<QImage>(mimeData->imageData());
        } else {
            currentText = clipboard->text();
        }
        originalClipboardText = currentText;
        clipboard->setText(currentText);
    }
    ui->text->setText(currentText);
    ui->menuBar->setFocus();
    ui->text->setReadOnly(false);
    timerStopped = false;
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(stopTimer()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()),
                     this,  SLOT(doQuit()));
    connect(ui->actionLicense, SIGNAL(triggered()), this, SLOT(doShowLicense()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(showAboutQt()));
    connect(ui->actionRegular_expressions, SIGNAL(triggered()), this, SLOT(doShowRegularExpressions()));
    connect(ui->actionAllUppercase, SIGNAL(triggered()), this, SLOT(allUppercase()));
    connect(ui->actionAllLowercase, SIGNAL(triggered()), this, SLOT(allLowercase()));
    connect(ui->actionFirst_uppercase_other_lowercase, SIGNAL(triggered()), this, SLOT(allLowerCaseWithFirstUppercase()));
    connect(ui->actionAllInitialsUppercase, SIGNAL(triggered()), this, SLOT(allInitialsUppercase()));
    connect(ui->actionOnlyInitialsUppercase, SIGNAL(triggered()), this, SLOT(allOnlyInitialsUppercase()));
    connect(ui->actionStopTimer, SIGNAL(triggered()), this, SLOT(stopTimer()));
    connect(ui->actionResetCase, SIGNAL(triggered()), this, SLOT(resetCase()));
    //connect(ui->actionSetEditableTextbox, SIGNAL(triggered(bool)), this, SLOT(setEditableTextbox(bool)));
    connect(ui->actionRimuovi1, SIGNAL(triggered()), this, SLOT(rimuovi1()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(resetReplaceOptions()));
    connect(ui->swap, SIGNAL(clicked()), this, SLOT(swapReplaceOptions()));
    connect(ui->actionCopyFoldersRecursively, SIGNAL(triggered()), this, SLOT(doCopyFoldersRecursively()));
    connect(ui->actionCopyFolders, SIGNAL(triggered()), this, SLOT(doCopyFolders()));
    connect(ui->actionSaveImage, SIGNAL(triggered()), this, SLOT(doSaveImageAsk()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(doOpenSettings()));
    connect(ui->actionOpenImageDirOnExit, SIGNAL(triggered()), this, SLOT(doOpenImageDirOnExit()));
    connect(ui->actionCopyUrlsToClipboard, SIGNAL(triggered()), this, SLOT(doCopyUrlsToClipboard()));
    connect(ui->actionCreateEmailWithLink, SIGNAL(triggered()), this, SLOT(doEmailWithLink()));
    connect(ui->actionReplaceInText, SIGNAL(triggered()), this, SLOT(doReplaceInText()));
    connect(ui->replaceInText_b, SIGNAL(clicked()), this, SLOT(doReplaceInText()));
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(doUndo()));
    connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(doRedo()));
    connect(ui->actionApply_modifications, SIGNAL(triggered()), this, SLOT(doApplyModifications()));
    connect(ui->actionEditText, SIGNAL(triggered()), this, SLOT(doFocusOnEditText()));
    connect(ui->actionCropImage, SIGNAL(triggered()), this, SLOT(doCropImage()));
    connect(ui->actionCropImageWithPreview, SIGNAL(triggered()), this, SLOT(doCropImageWithPreview()));
    connect(ui->apply_b, SIGNAL(clicked()), this, SLOT(doApplyModifications()));
    connect(ui->undo_b, SIGNAL(clicked()), this, SLOT(doUndo()));
    connect(ui->runMacro_b, SIGNAL(clicked()), this, SLOT(loadSelectedMacro()));
    //connect(this->imageWin, SIGNAL(accepted()), this, SLOT(doCropAfterPreview()));
    ui->statusBar->setFocus();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doQuit()));
    if (timer_ms > 0) {
        timer->setInterval(timer_ms);
        QTimer::singleShot(80,this,SLOT(setStartTimer()));
    }
    crop = false;

    if (!QFile::exists(cfgFile)) saveSettings();
}

MainWindow::~MainWindow()
{
    if (ui->saveStrings->isChecked())
        saveSettings();
    QDir(QCoreApplication::applicationDirPath()+"/TMP").removeRecursively();
    delete ui;
}

void MainWindow::doUndo()
{
    if (undoTextList.isEmpty()) {
        ui->text->undo();
        return;
    } else {
        ui->text->setText(undoTextList.at(currentUndoIndex));
        undoTextList.removeLast();
        currentUndoIndex--;
    }
}

void MainWindow::doRedo()
{
    /*if (undoTextList.isEmpty()) return;
    ui->text->setText(undoTextList.at(currentUndoIndex));
    undoTextList.removeLast();
    currentUndoIndex--;*/
    ui->text->redo();
}

void MainWindow::doCropImageWithPreview()
{
    imageWin = new DialogImageCropPreview();
    imageWin->setImage(image, cropTop, cropBottom, cropLeft, cropRight);
    int result = imageWin->exec();
    //crop = true;
    //doCropImageAsk(false);
    //doQuit();

    if(result == QDialog::Accepted) { // YesButton clicked
        cropLeft = imageWin->getCropLeft();
        cropRight = imageWin->getCropRight();
        cropTop = imageWin->getCropTop();
        cropBottom = imageWin->getCropBottom();
    } else return;
    //   if(result == QDialog::Rejected) { // NoButton clicked
    //   }
    //crop = true;
    doCropImage();
}

void MainWindow::doCropImage()
{
    crop = true;
    doCropImageAsk(false);
    doQuit();
}

void MainWindow::doCropImageAsk(bool ask=false)
{
    QString imageFilenameNow;
    if (ask)
        imageFilenameNow = QFileDialog::getSaveFileName(this, "Save Image...",ui->text->toPlainText());
    else {
        imageFilenameNow = ui->text->toPlainText();
        imageFilenameNow.replace("\\","/");
        //imageFilenameNow = imageFilenameNow+"_cropped."+imageExtension;
    }
    if (imageFilenameNow.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(imageFilenameNow);
    imagePath = fileInfo.absolutePath();
    imageFilename = fileInfo.completeBaseName();
    imageExtension = fileInfo.suffix();
    imageNumber = getImageNumber() + 1;
    QString imageStringNumberTrailingZeros =
        QString("%1").arg(QVariant(imageNumber).toString().replace(".",""),imageNumberDigits,QLatin1Char('0'));
    imageFilenameNow = imageFilenameNow.replace("."+imageExtension,
                                                imageStringNumberTrailingZeros+"."+imageExtension);
    int cropX = cropLeft;
    int cropY = cropTop;
    int cropWidth = image.width() - cropRight - cropLeft;
    int cropHeight = image.height() - cropBottom - cropTop;
    QRect rect(cropX, cropY, cropWidth, cropHeight);

    //QImage cropped = image.copy(rect);

    if (image.copy(rect).save(imageFilenameNow)) {
        saveSettings();
    }
    else  QMessageBox::warning(this,"Error","Error saving image\n"+imageFilenameNow);
}


void MainWindow::doApplyModifications()
{
    doUndo();
    ui->preview->setChecked(false);
    rimuovi1();
}

void MainWindow::doFocusOnEditText()
{
    ui->text->setFocus();
}

void MainWindow::addToUndo()
{
    undoTextList.append(ui->text->toPlainText());
    currentUndoIndex = undoTextList.size()-1;
}

void MainWindow::doReplaceInText()
{
    addToUndo();
    QString currentReplaceWithEdit =  ui->replaceWithEdit->currentText();
    if (ui->useNewline->isChecked())
        currentReplaceWithEdit
            .replace("\\n","\n")
            .replace("\\t","\t");
    if (!ui->useRegexp->isChecked())
        ui->text->setText(ui->text->toPlainText().replace(ui->replaceEdit->currentText(),
                                                          currentReplaceWithEdit));
    else {
        QRegularExpression myRegExp(ui->replaceEdit->currentText());
        //myRegExp.setMinimal(true);
        if (!ui->useTags->isChecked()) {
            ui->text->setText(ui->text->toPlainText().replace(myRegExp,
                                                              currentReplaceWithEdit));
        }
        else {
            QStringList originalText = ui->text->toPlainText().split("\n");
            QStringList newText;
            QString currentLine;
            QString numberTrailingZeros;
            for (int i = 0; i < originalText.size(); i++) {
                currentLine = originalText.at(i);
                numberTrailingZeros =
                    QString("%1").arg(QVariant(i+1).toString().replace(".",""),imageNumberDigits,QLatin1Char('0'));
                currentLine.replace(myRegExp,currentReplaceWithEdit.
                                              replace("##NUMBER#",numberTrailingZeros).
                                              replace("##DATE#",QDate::currentDate().toString("yyyy-MM-dd")));
                newText.append(currentLine);
                ui->text->setText(newText.join("\n"));
            }
        }
    }
}

void MainWindow::doEmailWithLink()
{
    /*QFile file(QCoreApplication::applicationDirPath()+"/PasteOnlyText_model.eml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;*/
    QString urlComplete =
        "Subject: \n"
        "Date: Mon, 3 Feb 2014 13:34:39 +0100\n"
        "MIME-Version: 1.0\n"
        "Content-Type: multipart/alternative;\n"
        "	boundary=\"----=_NextPart_000_015C_01CF20E4.B8A495A0\"\n"
        "X-Priority: 3\n"
        "X-MSMail-Priority: Normal\n"
        "Importance: Normal\n"
        "X-Unsent: 1\n"
        "X-MimeOLE: \n"
        "\n"
        "Messaggio multiparte in formato MIME.\n"
        "\n"
        "------=_NextPart_000_015C_01CF20E4.B8A495A0\n"
        "Content-Type: text/html;\n"
        "	charset=\"iso-8859-1\"\n"
        "Content-Transfer-Encoding: quoted-printable\n"
        "\n"
        "<HTML><HEAD></HEAD><BODY dir=3Dltr><DIV dir=3Dltr>";


    //urlsMime = new QMimeData();

    QStringList stringUrls = ui->text->toPlainText().replace("/","\\")
                                 .split("\n",Qt::SkipEmptyParts);
    //QString urlComplete = "<HTML><BODY><p><A href=\"";
    //QString urlComplete = "<HTML><BODY><p><A href=\"";
    //QString urlComplete = "<HTML><HEAD></HEAD><BODY dir=3Dltr><DIV dir=3Dltr>";
    for (int i=0;i<stringUrls.size();i++) {
        urlComplete.append("<DIV><A href=3D\""+stringUrls.at(i)+"\">"+stringUrls.at(i));
        //urlComplete.append();
        //urlComplete.append("</A></p><p><A href=\"");
        urlComplete.append("</A></DIV>");
    }
    //urlComplete.append(stringUrls.last()+"\">"+stringUrls.last());
    urlComplete.append("<DIV><FONT size=3D2=20 face=3DArial></FONT></DIV></DIV></BODY></HTML>");
    //urlComplete.append("</A></p></BODY></HTML>");
    urlComplete.append("\n\n------=_NextPart_000_015C_01CF20E4.B8A495A0--");

    /*while (!file.atEnd()) {
            QByteArray line = file.readLine();
            process_line(line);
        }*/
    //QTextStream in(&file);
    //QString allData = in.readAll();
    //QByteArray allData = file.readAll();
    //allData.replace("___LINK_BOOKMARK___",urlComplete);

    QFile fileOut(QCoreApplication::applicationDirPath()+"/PasteOnlyText.eml");
    if (!fileOut.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return;

    QTextStream out(&fileOut);
    out << urlComplete;
    //fileOut.write();
    fileOut.close();
    QDesktopServices::openUrl(
        QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/PasteOnlyText.eml"));
    //urlsMime->setHtml(urlComplete);
    //clipboard->setMimeData(urlsMime);
    //clipboard->setMimeData(urlsMime);
    QCoreApplication::quit();
}

void MainWindow::doCopyUrlsToClipboard()
{
    //qDebug(mimeDataOrig);
    urlsMime = new QMimeData();
    //urlsMime->setData("application/x-qt-windows-mime;value=\"msSourceUrl\"",
    //                  mimeDataOrig->data("application/x-qt-windows-mime;value=\"msSourceUrl\""));
    //urlsMime->setData("text/html",mimeDataOrig);
    //QByteArray mimeDataOrig = clipboard->mimeData()->data("text/html");
    //qDebug(mimeDataOrig->formats().join("\n").toUtf8());
    //urlsMime->setData("text/plain","prova123");
    //urlsMime->setUrls(QList<QUrl>()<<QUrl::fromLocalFile("C:\\Users"));
    //provare: urlsMime->setHtml();
    //clipboard->setMimeData(urlsMime);

    //urlsMime->setText("C:\\Users");
    //urlsMime->setUrls(QList<QUrl>()<<QUrl("file://C:\\Users"));
    QStringList stringUrls = ui->text->toPlainText().replace("/","\\")
                                 .split("\n",Qt::SkipEmptyParts);
    //QString urlComplete = "<HTML><BODY><p><A href=\"";
    QString urlComplete = "<HTML><BODY><p><A href=\"";
    for (int i=0;i<stringUrls.size()-1;i++) {
        urlComplete.append(stringUrls.at(i)+"\">"+stringUrls.at(i));
        urlComplete.append("</A></p><p><A href=\"");
    }
    urlComplete.append(stringUrls.last()+"\">"+stringUrls.last());
    urlComplete.append("</A></p></BODY></HTML>");

    //QString urlStart = "<HTML><BODY><p><A href=\"";
    //QString url1 = "C:\\Users\">C:\\Users";
    //QString urlSeparator = "</A></p><p><A href=\"";
    //QString url2 = "C:\\Windows\">C:\\Windows";
    //QString urlEnd = "</A></p></BODY></HTML>";
    //QString urlComplete("<HTML><BODY><p><A href=\"C:\\Users\">C:\\Users</A></p><p><A href=\"C:\\Windows\">C:\\Windows</A></p></BODY></HTML>");
    //QString urlComplete = urlStart+url1+urlSeparator+url2+urlEnd;

    urlsMime->setHtml(urlComplete);
    //clipboard->setMimeData(urlsMime);
    clipboard->setMimeData(urlsMime);
    QCoreApplication::quit();

}

void MainWindow::doSaveImageAsk()
{
    doSaveImage(true);
}

void MainWindow::doSaveImage(bool ask=true)
{
    QString imageFilenameNow;
    if (ask)
        imageFilenameNow = QFileDialog::getSaveFileName(this, "Save Image...",ui->text->toPlainText());
    else {
        imageFilenameNow = ui->text->toPlainText();
        imageFilenameNow.replace("\\","/");
    }
    if (imageFilenameNow.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(imageFilenameNow);
    imagePath = fileInfo.absolutePath();
    imageFilename = fileInfo.completeBaseName();
    imageExtension = fileInfo.suffix();
    imageNumber = getImageNumber() + 1;
    QString imageStringNumberTrailingZeros =
        QString("%1").arg(QVariant(imageNumber).toString().replace(".",""),imageNumberDigits,QLatin1Char('0'));
    imageFilenameNow = imageFilenameNow.replace("."+imageExtension,
                                                imageStringNumberTrailingZeros+"."+imageExtension);

    if (image.save(imageFilenameNow)) {
        saveSettings();
    }
    else  QMessageBox::warning(this,"Error","Error saving image!");
}

int MainWindow::getImageNumber()
{
    QDir directory(imagePath);
    QStringList allImages = directory.entryList(QStringList(imageFilename+"*."+imageExtension),QDir::Files,QDir::Name);
    int max = 0;
    int now = 0;
    for (int i = 0; i< allImages.size();i++) {
        QString digits = QVariant(imageNumberDigits).toString();
        QRegularExpression regExp(imageFilename+"([0-9]{"+
                                  digits+","+digits+"})"+"."+imageExtension);
        //regExp.setMinimal(true);
        regExp.setPatternOptions(QRegularExpression::InvertedGreedinessOption);

        //int pos = regExp.indexIn(allImages.at(i));
        QRegularExpressionMatch match = regExp.match(allImages.at(i));
        //int pos = regExp.match(allImages.at(i));
        //if (pos > -1) {
        if (match.hasMatch()) {
            //QString nowString = regExp.cap(1);
            QString nowString = match.captured(1);
            now = QVariant(nowString).toInt();
            if (now > max )
                max = now;
        }
    }

    return max;
}

void MainWindow::doOpenSettings()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/PasteOnlyText.txt"));
}

void MainWindow::doOpenImageDirOnExit()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath));
    doQuit();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString urlsString;
    for (int i = 0; i< urls.size();i++) {
        urlsString.append(urls.at(i).toString().replace("file:///","")+"\n");
    }
    ui->text->setPlainText(urlsString);
    ui->statusBar->showMessage(QVariant(urls.size()).toString()+" file(s) dropped!");
    event->acceptProposedAction();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::doQuit()
{
    if (image.isNull())
        clipboard->setText(ui->text->toPlainText());
    else {
        if (!crop) doSaveImage(false);
        clipboard->clear();
        crop = false;
    }
    qApp->processEvents();
    QCoreApplication::quit();
}

void MainWindow::loadSettings()
{
    replaceText = settings->value("ReplaceText"," (1)").toString();
    withText = settings->value("WithText","").toString();
    replaceText_default = settings->value("ReplaceText_default"," (1)").toString();
    withText_default = settings->value("WithText_default","").toString();
    timer_ms = settings->value("EnableTimer_ms",0).toInt();
    imageNumber = settings->value("imageNumber",1).toInt();
    imageNumberDigits = settings->value("imageNumberDigits",3).toInt();
    imagePath = settings->value("imagePath",QCoreApplication::applicationDirPath()+"/Images").toString();
    QDir tempImageDir = QDir(imagePath);
    if (!tempImageDir.exists()) tempImageDir.mkdir(imagePath);
    imageFilename = settings->value("imageFilename","image").toString();
    imageExtension = settings->value("imageExtension",+"png").toString();
    cropRight = settings->value("cropRight",10).toInt();
    cropLeft = settings->value("cropLeft",10).toInt();
    cropTop = settings->value("cropTop",10).toInt();
    cropBottom = settings->value("cropBottom",10).toInt();
    replaceTextListSeparator = settings->value("ReplaceTextListSeparator",+";;").toString();
    replaceTextList = settings->value("ReplaceTextList",+" (1)"+
                                                             replaceTextListSeparator+" (2)").toString().split(replaceTextListSeparator);
    withTextList = settings->value("WithTextList",+""+
                                                       replaceTextListSeparator+" (1)").toString().split(replaceTextListSeparator);
    numMacros = settings->value("numMacros",10).toInt();
    macros.clear();
    macros.append(settings->value("Macro1",
                                  "Ctrl+1"+replaceTextListSeparator+
                                      "Macro01"+replaceTextListSeparator+
                                      "From"+replaceTextListSeparator+
                                      "To"+replaceTextListSeparator+
                                      "UseRegExp"+replaceTextListSeparator+
                                      "UseNewLines"+replaceTextListSeparator+
                                      "UseTags"+replaceTextListSeparator+
                                      "AutoRun"+replaceTextListSeparator+
                                      "RenameFiles"
                                  ).toString());
    for (int i =1; i< numMacros; i++)
        macros.append(settings->value("Macro"+QVariant(i+1).toString(),"<empty>"
                                      ).toString());
}

void MainWindow::saveSettings()
{
    settings->setValue("ReplaceText", ui->replaceEdit->currentText());
    settings->setValue("WithText", ui->replaceWithEdit->currentText());
    settings->setValue("ReplaceText_default", replaceText_default);
    settings->setValue("WithText_default", withText_default);
    settings->setValue("EnableTimer_ms", timer_ms);
    settings->setValue("imageFilename",imageFilename);
    settings->setValue("imageNumber",imageNumber);
    settings->setValue("imageNumberDigits",imageNumberDigits);
    settings->setValue("imageExtension",imageExtension);
    settings->setValue("imagePath",imagePath);
    settings->setValue("cropLeft",cropLeft);
    settings->setValue("cropRight",cropRight);
    settings->setValue("cropTop",cropTop);
    settings->setValue("cropBottom",cropBottom);
    settings->setValue("ReplaceTextListSeparator",replaceTextListSeparator);
    settings->setValue("ReplaceTextList",replaceTextList.join(replaceTextListSeparator));
    settings->setValue("WithTextList",withTextList.join(replaceTextListSeparator));

    QString progressiveText = settings->value("ProgressiveText","").toString();
    //qDebug("progressiveText: "+progressiveText.toUtf8());
    if (progressiveText.isEmpty()) {
        //qDebug("progressiveText is empty");
        settings->setValue("ProgressiveText","Example1;;Example2;;Example3");
        settings->sync();
        //qDebug("ProgressiveText is empty. Example1;;Example2;;Example3 created in settings. Exiting.");
        //return 1;
    }


    //settings->setValue("Macro001",macros.first());
    settings->setValue("NumMacros",macros.size());
    for (int i =0; i< macros.size(); i++)
        settings->setValue("Macro"+QVariant(i+1).toString(),macros.at(i));


}

void MainWindow::resetReplaceOptions()
{
    ui->replaceEdit->setEditText(replaceText_default);
    ui->replaceWithEdit->setEditText(withText_default);
    saveSettings();
}

void MainWindow::swapReplaceOptions()
{
    QString swapString = ui->replaceEdit->currentText();
    ui->replaceEdit->setEditText(ui->replaceWithEdit->currentText());
    ui->replaceWithEdit->setEditText(swapString);
}

void MainWindow::loadSelectedMacro()
{
    loadMacro(ui->macros->currentText().split(replaceTextListSeparator).at(0));
}

void MainWindow::loadMacro(QString currentMacro)
{
    for (int i=0;i<macros.size();i++) {
        if (macros.at(i).contains(currentMacro+replaceTextListSeparator,Qt::CaseInsensitive)) {
            QStringList currentMacroTexts = macros.at(i).split(replaceTextListSeparator);
            if (currentMacroTexts.size()==9) {
                ui->macros->setCurrentIndex(i);
                ui->replaceEdit->setEditText(currentMacroTexts.at(2).toUtf8());
                ui->replaceWithEdit->setEditText(currentMacroTexts.at(3));
                if (currentMacroTexts.at(4).toLower()=="useregexp")
                    ui->useRegexp->setChecked(true);
                else
                    ui->useRegexp->setChecked(false);
                if (currentMacroTexts.at(5).toLower()=="usenewlines")
                    ui->useNewline->setChecked(true);
                else
                    ui->useNewline->setChecked(false);
                if (currentMacroTexts.at(6).toLower()=="usetags")
                    ui->useTags->setChecked(true);
                else
                    ui->useTags->setChecked(false);
                if (currentMacroTexts.at(7).toLower()=="autorun") {
                    if (currentMacroTexts.at(8).toLower()=="renamefiles")
                        rimuovi1();
                    else
                        doReplaceInText();
                }
                //qDebug(currentMacroTexts.join("\n").toUtf8());
            }
            break;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{

    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        int keyInt = keyEvent->key();
        //Qt::Key key = static_cast<Qt::Key>(keyInt);
        /*if(key == Qt::Key_unknown){
            qDebug("Unknown key from a macro probably");
            return;
        }
        // the user have clicked just and only the special keys Ctrl, Shift, Alt, Meta.
        if(key == Qt::Key_Control ||
                key == Qt::Key_Shift ||
                key == Qt::Key_Alt ||
                key == Qt::Key_Meta)
        {
            qDebug("Single click of special key: Ctrl, Shift, Alt or Meta");
            qDebug("New KeySequence:");
            qDebug(QKeySequence(keyInt).toString(QKeySequence::NativeText).toUtf8());
            return;
        }*/

        // check for a combination of user clicks
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        //QString keyText = keyEvent->text();
        // if the keyText is empty than it's a special key like F1, F5, ...
        //qDebug("Pressed Key: ");
        //qDebug(keyText.toUtf8());

        //QList<Qt::Key> modifiersList;
        if(modifiers & Qt::ShiftModifier)
            keyInt += Qt::SHIFT;
        if(modifiers & Qt::ControlModifier)
            keyInt += Qt::CTRL;
        if(modifiers & Qt::AltModifier)
            keyInt += Qt::ALT;
        if(modifiers & Qt::MetaModifier)
            keyInt += Qt::META;

        //qDebug("New KeySequence:");
        //qDebug(QKeySequence(keyInt).toString(QKeySequence::NativeText).toUtf8());
        loadMacro(QKeySequence(keyInt).toString(QKeySequence::NativeText));
    }
}

void MainWindow::rimuovi1()
{
    if (ui->replaceEdit->currentText() == "") {
        QMessageBox::warning(this,"Error","The field \"From\" must not be empty");
        return;
    }
    addToUndo();
    QString stringReplace, newString;
    stringReplace = ui->text->toPlainText();
    stringReplace.replace("file:///","");
    QStringList files = stringReplace.split("\n",Qt::SkipEmptyParts);
    int errors = 0;
    QFileInfo fileInfo;
    QString absolutePath;

    for (int i = 0; i < files.size();i++) {
        stringReplace = files.at(i);
        fileInfo.setFile(stringReplace);
        //stringReplace.replace("\\","/");
        stringReplace = fileInfo.fileName();
        newString = stringReplace;
        absolutePath = fileInfo.absolutePath();
        //newString.replace(ui->replaceEdit->text(),ui->replaceWithEdit->text());

        //NEW
        if (!ui->useRegexp->isChecked())
            //ui->text->setText(ui->text->toPlainText().replace(ui->replaceEdit->text(),
            //                                                  ui->replaceWithEdit->text()));
            newString.replace(ui->replaceEdit->currentText(),ui->replaceWithEdit->currentText());
        else {
            QRegularExpression myRegExp(ui->replaceEdit->currentText());
            //myRegExp.setMinimal(true);
            if (!ui->useTags->isChecked()) {
                //ui->text->setText(ui->text->toPlainText().replace(myRegExp,
                //                                                  ui->replaceWithEdit->text()));
                newString.replace(myRegExp,ui->replaceWithEdit->currentText());
            }
            else {

                QString NumberTrailingZeros =
                    QString("%1").arg(QVariant(i+1).toString().replace(".",""),imageNumberDigits,QLatin1Char('0'));
                newString.replace(myRegExp,ui->replaceWithEdit->currentText().
                                            replace("##NUMBER#",NumberTrailingZeros).
                                            replace("##DATE#",QDate::currentDate().toString("yyyy-MM-dd")));
            }
        }
        //END NEW
        if (!ui->preview->isChecked())
        {
            if (!QFile::rename(absolutePath+"/"+stringReplace,absolutePath+"/"+newString))
                errors++;
            else
                files.replace(i,absolutePath+"/"+newString);
        } else {
            files.replace(i,absolutePath+"/"+newString);
        }
    }
    if (errors > 0) {
        ui->statusBar->showMessage("Error renaming "+QVariant(errors).toString()+" file(s)!");
        QMessageBox::warning(this,"Error","Error renaming "+QVariant(errors).toString()+" file(s)!");
    }
    else
    {
        if (!ui->preview->isChecked())
            ui->statusBar->showMessage(QVariant(files.size()).toString()+"File(s) renamed correctly");
        else
            ui->statusBar->showMessage(QVariant(files.size()).toString()+"File(s) (NOT ACTUALLY) renamed correctly");
    }
    ui->text->setText(files.join("\n"));
}

void MainWindow::doShowLicense()
{
    //qDebug(qCompress("ZANELLATO FRANCESCO PAOLO LUCA").toHex());
    //qDebug(qUncompress(QByteArray::fromHex("0000001e789c8b72f473f5f1710cf157700b72f473760d76f6570870f4f7f157f0097576040081fb083f")));
    QMessageBox::information(this,"License",
                             "Copyright (C) 2015-2025 Francesco Paolo Luca Zanellato\n"
                             "\n"
                             "This program is free software: you can redistribute it and/or modify\n"
                             "it under the terms of the GNU General Public License as published by\n"
                             "the Free Software Foundation, either version 3 of the License, or\n"
                             "(at your option) any later version.\n"
                             "\n"
                             "This program is distributed in the hope that it will be useful,\n"
                             "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                             "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
                             "GNU General Public License for more details.\n"
                             "\n"
                             "You should have received a copy of the GNU General Public License\n"
                             "along with this program. If not, see <https://www.gnu.org/licenses/>."
                             );
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this,"About Qt");
}

void MainWindow::doShowRegularExpressions()
{
    QDesktopServices::openUrl(QUrl("https://pcre.org/original/doc/html/pcrepattern.html"));
}

void MainWindow::allUppercase()
{
    addToUndo();
    timer->stop();
    if (ui->text->isReadOnly()) {
        currentText = clipboard->text().toUpper();
        ui->text->setText(currentText);
    }
    else {
        QTextCursor myCursor = ui->text->textCursor();
        if (myCursor.selectedText().isEmpty()) {
            currentText = ui->text->toPlainText().toUpper();
            ui->text->setText(currentText);
        }
        else {
            int selStart = myCursor.selectionStart();
            int selEnd = myCursor.selectionEnd();
            ui->text->textCursor().insertText(ui->text->textCursor().selectedText().toUpper());
            myCursor.setPosition(selStart);
            myCursor.setPosition(selEnd, QTextCursor::KeepAnchor);
            ui->text->setTextCursor(myCursor);
            ui->text->setFocus();
            currentText = ui->text->toPlainText();
        }
    }
    //ui->text->setText(currentText);
    clipboard->setText(currentText);
    if (!timerStopped)
        QTimer::singleShot(50,this,SLOT(startTimer()));
}

void MainWindow::allLowercase()
{
    addToUndo();
    timer->stop();
    if (ui->text->isReadOnly()) {
        currentText = clipboard->text().toLower();
        ui->text->setText(currentText);
    }
    else {
        QTextCursor myCursor = ui->text->textCursor();
        if (myCursor.selectedText().isEmpty()) {
            currentText = ui->text->toPlainText().toLower();
            ui->text->setText(currentText);
        }
        else {
            int selStart = myCursor.selectionStart();
            int selEnd = myCursor.selectionEnd();
            ui->text->textCursor().insertText(ui->text->textCursor().selectedText().toLower());
            myCursor.setPosition(selStart);
            myCursor.setPosition(selEnd, QTextCursor::KeepAnchor);
            ui->text->setTextCursor(myCursor);
            ui->text->setFocus();
            currentText = ui->text->toPlainText();
        }
    }
    //ui->text->setText(currentText);
    clipboard->setText(currentText);
    if (!timerStopped)
        QTimer::singleShot(50,this,SLOT(startTimer()));
}

void MainWindow::allLowerCaseWithFirstUppercase()
{
    addToUndo();
    timer->stop();
    if (ui->text->isReadOnly())
        currentText = clipboard->text().toLower();
    else
        currentText = ui->text->toPlainText().toLower();
    QStringList stringList;
    stringList<<currentText.split("\n",Qt::KeepEmptyParts);
    for (int i=0;i<stringList.size();i++) {
        if (stringList[i].size()>0)
            stringList[i].replace(0,1,stringList.at(i).at(0).toUpper());
    }
    currentText=stringList.join("\n");
    ui->text->setText(currentText);
    clipboard->setText(currentText);
    if (!timerStopped)
        QTimer::singleShot(50,this,SLOT(startTimer()));
}

void MainWindow::resetCase()
{
    addToUndo();
    timer->stop();
    currentText = originalClipboardText;
    ui->text->setText(originalClipboardText);
    clipboard->setText(originalClipboardText);
    if (!timerStopped)
        QTimer::singleShot(50,this,SLOT(startTimer()));
}

void MainWindow::allOnlyInitialsUppercase()
{
    initialsUppercase(false);
}

void MainWindow::allInitialsUppercase()
{
    initialsUppercase(true);
}

void MainWindow::initialsUppercase(bool firstLowerCase = true)
{
    addToUndo();
    timer->stop();
    if (ui->text->isReadOnly())
        currentText = clipboard->text();
    else
        currentText = ui->text->toPlainText();
    if (firstLowerCase) currentText = currentText.toLower();
    currentText.replace(0,1,currentText.at(0).toUpper());
    QChar currChar;
    for (int i = 1; i < currentText.size()-1;i++) {
        currChar = currentText.at(i);
        if (
            currChar.isSymbol() ||
            currChar.isPunct() ||
            currChar.isSpace()
            ) {
            currentText.replace(i,2,QString(currentText.at(i))
                                    .append(currentText.at(i+1).toUpper()));
        }
    }

    ui->text->setText(currentText);
    clipboard->setText(currentText);
    if (!timerStopped)
        QTimer::singleShot(50,this,SLOT(startTimer()));
}

void MainWindow::stopTimer()
{
    if (this->isActiveWindow()) {
        timer->stop();
        timerStopped = true;
    }
    if (!ui->text->isActiveWindow())
        ui->text->setReadOnly(true);
    else
        ui->text->setReadOnly(false);
}

/*void MainWindow::setEditableTextbox(bool readOnly)
{
    if (!readOnly) {
        currentText = ui->text->toPlainText();
        clipboard->setText(currentText);
    }
    timer->stop();
    ui->text->setFocus();
    ui->text->setReadOnly(!readOnly);
}*/

void MainWindow::startTimer()
{
    timer->start();
}

void MainWindow::setStartTimer()
{
    timerStopped = false;
    timer->start();
}

void MainWindow::doCopyFolders()
{
    CopyFolders(false);
}

void MainWindow::doCopyFoldersRecursively()
{
    CopyFolders(true);
}

void MainWindow::CopyFolders(bool recursive)
{
    timer->stop();
    QString DestFolder = QCoreApplication::applicationDirPath()+"/TMP";
    if (!QFile::exists(DestFolder))
        QDir().mkdir(DestFolder);
    QString stringReplace;
    stringReplace = ui->text->toPlainText();
    stringReplace.replace("file:///","");
    QStringList files = stringReplace.split("\n",Qt::SkipEmptyParts);
    bool warning = false;
    for (int i=0; i< files.size(); i++) {
        if (recursive) {
            copyFoldersRecursively(files.at(i), DestFolder+"/"+QDir(files.at(i)).dirName());
        }
        else
            QDir().mkdir(DestFolder+"/"+QDir(files.at(i)).dirName());
    }
    if (warning)
        QMessageBox::warning(this,"Warning","Some folders have path length > 250 characters.\n"
                                              "Please check the copied folders");
    QDesktopServices::openUrl(QUrl::fromLocalFile(DestFolder));
}

void MainWindow::copyFoldersRecursively(QString sourceFolder, QString destFolder)
{
    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
        return;
    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        destDir.mkdir(destFolder);
    }

    /*//to also copy files:
    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        QFile::copy(srcName, destName);
    }
    files.clear();*/
    QStringList files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        copyFoldersRecursively(srcName, destName);
    }
}
