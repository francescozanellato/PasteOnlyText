Dim pENV, WshShell
Set WshShell = CreateObject("WScript.Shell") 
Set pENV = WshShell.Environment("PROCESS")
pENV("PATH") = ".\;.\bin\;C:\msys64\mingw64\bin\"
pENV("QT_PLUGIN_PATH") = ".\share\qt6\plugins;C:\msys64\mingw64\share\qt6\plugins\"
WshShell.Run chr(34) & Replace(WScript.ScriptName, ".vbs" ,".exe") & Chr(34), 1
Set pENV = Nothing
Set WshShell = Nothing
