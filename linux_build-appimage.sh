#!/bin/bash
#This is just an example. Customise to your needs.
#appimagetool-x86_64.AppImage tool can be downloaded from https://github.com/AppImage/appimagetool

APP_NAME="PasteOnlyText"
APPDIR="./AppDir"

# Clean and make the AppDir structure
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/lib"
mkdir -p "$APPDIR/usr/plugins"

# Copy binary file
cp "./$APP_NAME" "$APPDIR/usr/bin/"

# Copy the Qt libraries (assuming they are in ./lib)
cp -r ./lib/* "$APPDIR/usr/lib/"

# Copy the Qt plugins (assuming they are in ./lib/plugins)
cp -r ./lib/plugins "$APPDIR/usr/"

# Create AppRun script (entrypoint)
cat > "$APPDIR/AppRun" <<EOF
#!/bin/bash
HERE=\$(dirname "\$(readlink -f "\$0")")
export LD_LIBRARY_PATH="\$HERE/usr/lib:\$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="\$HERE/usr/plugins"
exec "\$HERE/usr/bin/$APP_NAME" "\$@"
EOF
chmod +x "$APPDIR/AppRun"

# Create the desktop entry
cat > "$APPDIR/$APP_NAME.desktop" <<EOF
[Desktop Entry]
Name=$APP_NAME
Exec=$APP_NAME
Icon=$APP_NAME
Type=Application
Categories=Utility;
EOF

# (Optional) Add an icon
# cp ./icon.png "$APPDIR/$APP_NAME.png"
# or:
# cp ./icon.svg "$APPDIR/$APP_NAME.svg"

# Create the AppImage
./appimagetool-x86_64.AppImage "$APPDIR"

