#!/bin/bash
export LD_LIBRARY_PATH=.
# # warning: QT_QPA_PLATFORM_PLUGIN_PATH has priority on QT_PLUGIN_PATH for the platform plugin path
# export QT_QPA_PLATFORM_PLUGIN_PATH=./plugins/platforms
# export QT_DEBUG_PLUGINS=1
export QT_PLUGIN_PATH=./plugins
./PasteOnlyText

