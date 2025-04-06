Dim pENV, WshShell
Set WshShell = CreateObject("WScript.Shell") 
Set pENV = WshShell.Environment("PROCESS")
pENV("PATH") = ".\;.\bin\;C:\msys64\mingw64\bin\"
pENV("QT_PLUGIN_PATH") = ".\share\qt6\plugins;C:\msys64\mingw64\share\qt6\plugins\"
' WshShell.Run chr(34) & Replace"TemplateAppV2_QWT-UUID-SHA256-QSQL-QT3D-AES256.exe" & Chr(34), 1
WshShell.Run chr(34) & Replace(WScript.ScriptName, ".vbs" ,".exe") & Chr(34), 1
' hidden run: 0, show application run: 1
' sPath_cancella = "C:\QT\msys64\mingw64\bin\;.\;" & WshShell.ExpandEnvironmentStrings("%PATH%")
Set pENV = Nothing
Set WshShell = Nothing
