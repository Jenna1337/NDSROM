@echo off

set targ="%~1"
set targ=%targ:/=\%

echo %targ%
echo %~n1

set mdir="converted_NDS_data--%~n1"
if not exist %mdir% mkdir %mdir%





REM ##Icon
echo Extracting icon...
cd %mdir%
rem   places the icon in the working directory
set icongetterdir=546345642's NDS Icon Ripper
start /wait "%ProgramFiles%\Python27\python.exe" "%~dp0%icongetterdir%\originalcolours.py" "%1"
ren %~n1.png icon.png
cd ".."
echo Done extracting icon.





REM ##Models
echo Extracting models...
set tmpdir="%TEMP%\convertNDS--%~n1"
set apiculadir=apicula\apicula
set apicula="%~dp0%apiculadir%"
%apicula% extract %targ% --output %tmpdir%
%apicula% convert --more-textures %tmpdir% --output "%~dp0%mdir:"=%\models"
if exist %tmpdir% rmdir /S /Q %tmpdir%
echo Done extracting models.





REM ##Audio
echo Extracting audio...
set gcode=
set gtitle=
rem     Need this space     v
set fppart=ndsext_v04\ndssndext.exe
set fpcmd="%~dp0%fppart%" %targ%
echo %fpcmd%
for /F "tokens=1,2* delims=: " %%k in ('%~dp0%fppart% %targ%') do @(
	if "%%k"=="TITLE" (
		set gtitle=%%~l
	)
	if "%%k"=="CODE" (
		set gcode=%%~l
	)
)
echo "Title: %gtitle%"
echo "Code:  %gcode%"
set paud="%~dp0%mdir:"=%\audio"
mkdir %paud%
robocopy "%~dp1%gtitle%_%gcode%" %paud% /E /MOVE
echo Done extracting audio.





REM ##Images





REM ##Video





