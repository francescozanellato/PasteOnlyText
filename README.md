# PasteOnlyText
PasteOnlyText lets you perform many actions on the text/content copied in your clipboard.

    Author: Francesco Paolo Luca Zanellato
    Copyright (C) 2015-2025 Francesco Paolo Luca Zanellato

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.


The PasteOnlyText project is a tool designed to perform various actions on the text or content that is copied to your clipboard. Here are some key features and functionalities:

    Clipboard Management: It allows you to manage and manipulate the text content within your clipboard, making it easier to work with copied data.

    Text Processing: You can perform several text processing actions such as trimming, formatting, and converting text to different cases.

    Automation: The project can automate repetitive tasks involving text content, saving time and effort.

    Integration: It integrates seamlessly with other applications and tools, enabling efficient text handling across different platforms.

The project is primarily written in C++ and utilizes QMake for building and project management. Its user interface and functionalities are designed to enhance productivity when dealing with clipboard text content.

The main.cpp file for the PasteOnlyText project includes a command-line option to run the application in a non-interactive mode for progressive pasting to the clipboard. Here are the command-line options:

    -progressive: Enables non-interactive mode for progressive pasting to the clipboard.
        When this option is used, the application will read the ProgressiveText setting from the PasteOnlyText.cfg configuration file.
        It will paste the next item from the ProgressiveText list to the clipboard.
        If the ProgressiveText setting is empty, a default list (Example1;;Example2;;Example3) will be created in the configuration file.
        The imageNumber setting in the configuration file is used to keep track of the current index for progressive pasting.

Example Usage (from commandline/bash environment):

    ./PasteOnlyText -progressive

This option allows the PasteOnlyText application to automate the process of pasting predefined text snippets from a list to the clipboard in a sequential manner.

By modifying the PasteOnlyText.cfg file (located in the same folder of the executable file), you can set up your own text snippets and configure other options to enhance your clipboard management and automation experience.

# Configuration

PasteOnlyText allows you to configure various options through the `PasteOnlyText.cfg` configuration file. This file lets you customize different settings to tailor the application's behavior to your needs.

Example Configuration

```ini
[ProgressiveText]
# List of text snippets for progressive pasting
textList = Example1;;Example2;;Example3

[General]
# Index for progressive pasting
imageNumber = 0
```


# Compiling (recommended method on Windows):

Install MSYS2, following the instructions at the following link:

https://www.msys2.org/wiki/MSYS2-installation/

Run MSYS2 (Mingw64 environment):

    C:\msys64\mingw64.exe

Install Qt package and all the development tools with the following commands inside MSYS2 (Mingw64 environment):

    pacman -S mingw-w64-x86_64-qt6
    pacman -S mingw-w64-x86_64-make
    pacman -S git
    pacman -S mingw-w64-x86_64-gcc

Clone the Repository:

    git clone https://github.com/francescozanellato/PasteOnlyText.git
    cd PasteOnlyText

Generate Makefile using QMake:

    qmake6

Compile the Project:

    mingw32-make.exe

Run the Application:

    ./release/PasteOnlyText

copy the executable in the "bin" folder (along with the required dll to run the software):

    cp ./release/PasteOnlyText.exe /mingw64/bin/

    exit


In Windows, create the program shortcut on the Desktop, pointing to the following path:

    "C:\msys64\mingw64\bin\PasteOnlyText.exe"

Double click the link to open the program.



# Compiling (alternative method for all operating systems):


# Compiling

Install Qt and QMake:

    Ensure that you have Qt and QMake installed on your system. You can download and install Qt from the Qt official website.

Clone the Repository:

    git clone https://github.com/francescozanellato/PasteOnlyText.git
    cd PasteOnlyText

Generate Makefile using QMake:

    qmake

Compile the Project:

    make

Run the Application:

    ./PasteOnlyText

