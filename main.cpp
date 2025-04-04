/*
 * Author: Francesco Zanellato
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

#include "mainwindow.h"
#include <QApplication>


#include <QSettings>
#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QClipboard>
#include <QTimer>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QSharedMemory sharedMemory;
    sharedMemory.setKey("PasteOnlyTextID");
    if(sharedMemory.attach()) {
        // m_isRunning = true;
        return 2;
    }

    if (!sharedMemory.create(1)) {
        return 3; // Exit already a process running
    }
    //QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    QApplication ab(argc, argv);
    QStringList arguments = ab.arguments();
    if (arguments.size() > 1 && arguments.at(1)=="-progressive")
    { //Non-interactive mode. Progressive paste to clipboard
        //qDebug("Non-interactive mode. Progressive paste to clipboard...");
        QSettings *settings = new QSettings(ab.applicationDirPath()+"/PasteOnlyText.cfg",QSettings::IniFormat,0);

        QString replaceTextListSeparator = settings->value("ReplaceTextListSeparator",+";;").toString();
        QString progressiveText = settings->value("ProgressiveText","").toString();

        int currentIndex = settings->value("imageNumber",+"1").toInt();

        if (progressiveText.isEmpty()) {
            settings->setValue("ProgressiveText","Example1;;Example2;;Example3");
            settings->sync();
            //qDebug("ProgressiveText is empty. Example1;;Example2 created in settings. Exiting.");
            return 1;
        }
        QStringList progressiveTextList = progressiveText.split(replaceTextListSeparator);

        if (currentIndex>progressiveTextList.size())
            currentIndex = 1;

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(progressiveTextList.at(currentIndex-1));
        //clipboard->setText("ciao");
        currentIndex++;

        settings->setValue("imageNumber",currentIndex);

        //QObject::connect(settings, SIGNAL(destroyed()), &ab, SLOT(quit()));

        delete settings;
        //delete clipboard;
        QTimer::singleShot(500, &ab, SLOT(quit()));
        return ab.exec();

        //return 0;
    }

    else
    {
        //QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return ab.exec();
    }
}
