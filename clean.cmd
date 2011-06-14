@echo off
echo Cleaning, a moment please...

attrib *.suo -s -h -r

del /f /s /q *.suo
del /f /s /q *.user
del /f /s /q *.ncb

rd /s /q temp

del /f /s /q output\*.res
del /f /s /q output\*.dep
del /f /s /q output\*.htm
del /f /s /q output\*.ilk
del /f /s /q output\*.manifest
del /f /s /q output\*.obj
del /f /s /q output\*.pdb
del /f /s /q output\*.idb

rd /s /q output

echo Cleaning done!
echo. & pause
