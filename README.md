# PasteOnlyText
PasteOnlyText lets you perform many actions on the text/content/imges copied in your clipboard, like conversion to "All UPPERCASE", "all lower case" and fast image cropping, conversion and saving.

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

# Features

Explore the text of your clipboard:

![immagine](https://github.com/user-attachments/assets/987feecd-0385-4f5f-8cb5-12d0448bfb54)

Perform many actions on text (like all UPPERCASE or all lowercase) and images:

![Senza titolo2](https://github.com/user-attachments/assets/0c7c6287-ed51-4361-93d2-dc7027b2aa59)

Crop an image in memory and save it with a single click:

![immagine](https://github.com/user-attachments/assets/627b76cb-9c35-43b3-b364-9c3c51c69fde)

Duplicate the folders structures (but without the files inside) with the "Copy folders recursively" command:

![immagine](https://github.com/user-attachments/assets/8d3483dc-6c77-4d2c-ac23-fa8ea1e05a8c)



The PasteOnlyText project is a tool designed to perform various actions on the text or content that is copied to your clipboard. Here are some key features and functionalities:

* Clipboard Management: It allows you to manage and manipulate the text content within your clipboard, making it easier to work with copied data.
* Fast operations on texts: select any text in any application, then press CTRL+C (CMD+C on MacOS) to copy the text in the clipboard, then open PasteOnlyText and click CTRL+SHIFT+A, then press ESC to exit the program. Now you can paste the text (only plain text, without formatting) back into your application: you can see that the text is now ALL UPPERCASE. Other functions are possible in addition to "ALL UPPERCASE": "all lowercase", "All Initials Uppercase", etc. Please refer to the "Tool" menu for more details.
* Remove the formatting of the text: select any text in any application, then press CTRL+C (CMD+C on MacOS) to copy the text in the clipboard, then open PasteOnlyText, then press ESC to exit the program. The text is now in the clipboard without any text formatting, ready to be pasted with CTRL+C (Windows) or CMD+C (MacOS) in any text application.
* Press the "STAMP" key on your keyboard, then open PasteOnlyText, then press "O" in PasteOnlyText: a folder will open showing you the new image file automatically created.
* Press the "STAMP" key on your keyboard, then open PasteOnlyText, then press "W" in PasteOnlyText: you will preview the image in memory and you can crop the image before automatically saving it in the "Images" folder.
* Press CTRL+C on some selected files in you "Explorer" (Windows) or "finder" (MacOS) application, to copy them in the clipboard, then open PasteOnlyText and you can rename them in batch: use the lower part of the PasteOnlyText window (replace "" with "")
* You can use and save custom macros to speed you the text/file replacement actions.

* Text Processing: You can perform several text processing actions such as trimming, formatting, and converting text to different cases.

* Automation: The project can automate repetitive tasks involving text content, saving time and effort.

* Integration: It integrates seamlessly with other applications and tools, enabling efficient text handling across different platforms.

The project is primarily written in C++ and utilizes QMake for building and project management. Its user interface and functionalities are designed to enhance productivity when dealing with clipboard text content.

The main.cpp file for the PasteOnlyText project includes a command-line option to run the application in a non-interactive mode for progressive pasting to the clipboard. Here are the command-line options:

    -progressive: Enables non-interactive mode for progressive pasting to the clipboard.
        When this option is used, the application will read the ProgressiveText setting from the PasteOnlyText.txt configuration file.
        It will paste the next item from the ProgressiveText list to the clipboard.
        If the ProgressiveText setting is empty, a default list (Example1;;Example2;;Example3) will be created in the configuration file.
        The imageNumber setting in the configuration file is used to keep track of the current index for progressive pasting.

Example Usage (from commandline/bash environment):

    ./PasteOnlyText -progressive

This option allows the PasteOnlyText application to automate the process of pasting predefined text snippets from a list to the clipboard in a sequential manner.

By modifying the PasteOnlyText.txt file (located in the same folder of the executable file), you can set up your own text snippets and configure other options to enhance your clipboard management and automation experience.

# Works with AutoHotKey v2
You can install AutoHotKey v2 (https://www.autohotkey.com/) and configure a keyboard shortcut to open PasteOnlyText, using the following script in AutoHotKey (shortcut WIN+CTRL+ALT+C):

```
 ^#!c::
{
if WinExist("PasteOnlyText")
{
    WinActivate()
    Return
}
else
{
    Run("C:\PortableApps\PasteOnlyText\PasteOnlyText.exe")
    WinWait("PasteOnlyText")
    WinActivate()
    Return
}
```
You can automatically insert some favourite texts in the clipboard with the following AutoHotKey script (bound to shortcut WIN+CTRL+C):

```
^#c::
{
Run "C:\PortableApps\\PasteOnlyText\PasteOnlyText.exe -progressive"
Return
}
```

The favourite texts can be configured in the configuration file "C:\PortableApps\\PasteOnlyText\PasteOnlyText.txt", changing the following variable:

```
ProgressiveText="First text;;Second text"
```

The texts are separated, in the above example, with ";;" and each time you call "PasteOnlyText.exe -progressive", the next text will be put in the clipboard (no user interaction is needed).

Automatically paste your favourite text with the following AutoHotKey script (bound to shortcut WIN+CTRL+V):

```
#^v::
{
RunWait("C:\PortableApps\fraz\FrazannaSoft\PasteOnlyText.exe -progressive")
SendInput("^v")
Return
```

# Configuration

PasteOnlyText allows you to configure various options through the `PasteOnlyText.txt` configuration file. This file lets you customize different settings to tailor the application's behavior to your needs.

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

Ensure that you have access to C:\ and that you can write directories in the C:\ location.
Also ensure that no files are present in "C:\PortableApps\PasteOnlyText\" (or that this folder does not exist), otherwise, make a backup of these files, since the below commands will overwrite the contents.

Clone the Repository, build the project, run and deploy the application:

    git clone https://github.com/francescozanellato/PasteOnlyText.git
    cd PasteOnlyText
    qmake6
    mingw32-make.exe
	export MY_PROJECT_DEPLOYMENT_PATH="/c/PortableApps/PasteOnlyText"
	export PATH=./bin:./:$PATH
    mkdir -p $MY_PROJECT_DEPLOYMENT_PATH
    cp ./release/PasteOnlyText.exe $MY_PROJECT_DEPLOYMENT_PATH
    cp ./release/PasteOnlyText.vbs $MY_PROJECT_DEPLOYMENT_PATH
    cd $MY_PROJECT_DEPLOYMENT_PATH
    windeployqt6 --plugindir ./share/qt6/plugins $MY_PROJECT_DEPLOYMENT_PATH/PasteOnlyText.exe
    find $MY_PROJECT_DEPLOYMENT_PATH/share/ -type f -executable | xargs ldd | grep /mingw64 | awk '{print $3}' | xargs -i cp {} $MY_PROJECT_DEPLOYMENT_PATH/
    find $MY_PROJECT_DEPLOYMENT_PATH/ -type f -executable | xargs ldd | grep /mingw64 | awk '{print $3}' | xargs -i cp {} $MY_PROJECT_DEPLOYMENT_PATH/
    cp /mingw64/bin/libgif-7.dll ./
    rm -f D3Dcompiler_47.dll
    explorer.exe /select,"C:\\PortableApps\\PasteOnlyText\\PasteOnlyText.exe"
    $MY_PROJECT_DEPLOYMENT_PATH/PasteOnlyText.exe &

In Windows, create the program shortcut on the Desktop, pointing to the following path:

    "C:\PortableApps\PasteOnlyText\PasteOnlyText.exe"

Double click the link to open the program.

Warning: if you need to move the program folder in a different location and you obtain the Message "Error saving image!", please remember to change the variable "imagePath" in the configuration file "PasteOnlyText.txt" so that it points to the correct path (the path indicated in this variable is not created, so it must be existing).

Note: after compiling, the folder "C:\msys64\home\fraz3\PasteOnlyText" can be removed. You can leave the folder "C:\msys64" for compiling other programs.


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

