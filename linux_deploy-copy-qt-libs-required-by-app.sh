#!/bin/bash

APP_PATH="./PasteOnlyText"
APP_NAME=$(basename "$APP_PATH")
LIB_DIR="./lib"
PLUGINS_DIR="$LIB_DIR/plugins"

# Clena the directory
rm -rf "$LIB_DIR"
mkdir -p "$LIB_DIR"

# Start the app in background
"$APP_PATH" &
APP_PID=$!
echo "🚀 Avviata $APP_NAME (PID=$APP_PID)"
sleep 5  # Wait that the all the libraries are loaded

# Check that it is still running
if ! ps -p "$APP_PID" > /dev/null; then
    echo "❌ Process stopped too early. Try to increase the sleep time or run the app manually."
    exit 1
fi

# Collect the .so open libraries
echo "🔍 Extraction of the used shared libraries..."
LIBS=$(lsof -p "$APP_PID" 2>/dev/null | awk '{print $9}' | grep '\.so' | sort | uniq)

# Stop the app
kill "$APP_PID" 2>/dev/null

# Function to copy maintaining the folder structure
copy_lib() {
    local SRC="$1"
    if [[ -f "$SRC" ]]; then
        REL_PATH="${SRC#/}"  # remove the first slash
        DEST="$LIB_DIR/$REL_PATH"
        mkdir -p "$(dirname "$DEST")"
        cp -u "$SRC" "$DEST"
        echo "✅ Copied $SRC → $DEST"
    fi
}

# 1. Copy the dynamic libraries used at runtime
for LIB in $LIBS; do
    copy_lib "$LIB"
done

# 2. Fin the Qt plugin directory (search libqxcb.so)
echo "🔍 Searching Qt plugins..."
PLUGIN_XCB_PATH=$(find / -name libqxcb.so 2>/dev/null | head -n 1)

if [[ -z "$PLUGIN_XCB_PATH" ]]; then
    echo "⚠️  Qt Plugin not found (libqxcb.so). Please copy them manually in ./lib/plugins/"
else
    QT_PLUGINS_ROOT=$(dirname "$(dirname "$PLUGIN_XCB_PATH")")  # remove /platforms/libqxcb.so
    echo "📦 Qt plugins found in: $QT_PLUGINS_ROOT"

    # 3. Copy all the Qt plugins subdirectories
    for DIR in "$QT_PLUGINS_ROOT"/*; do
        if [[ -d "$DIR" ]]; then
            DEST_DIR="$PLUGINS_DIR/$(basename "$DIR")"
            mkdir -p "$DEST_DIR"
            cp -ru "$DIR/"* "$DEST_DIR/"
            echo "📁 Copied plugins: $DIR → $DEST_DIR"
        fi
    done
fi

echo ""
echo "✅ Completed! All libraries and Qt plugins copied in: $LIB_DIR"
echo "👉 To run the app:"
echo ""
echo "   export LD_LIBRARY_PATH=\$(pwd)/lib"
echo "   export QT_PLUGIN_PATH=\$(pwd)/lib/plugins"
echo "   ./laTuaApp"

