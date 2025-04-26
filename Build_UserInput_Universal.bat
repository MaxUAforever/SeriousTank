@echo off

set BuildConfiguration=1
set CompressionLevel="Fast"
set IsClient=0
set IsDistribution=
set Iterative=0
set IsIterative=
set IsIterativeBuild=
set PLATFORM=Win64
set ANDROID_ADDITIONALS=
set INPUTCookFlavor=
set COOKFLAVOR=Multi
set both=0
set UbtArgs=

echo ---- PLATFORM OPTIONS ----
echo 1. Win64
::echo 2. PS4
::echo 3. PS5
::echo 4. XSX
::echo 5. WinGDK
::echo 5. Android
::echo 6. Switch

echo ============================

set INPUT=
SET /P INPUT=Select Platform #:
echo.
echo.

::IF "%INPUT%"=="1" (
	set PLATFORM=Win64
::)
::IF "%INPUT%"=="2" (
::	set PLATFORM=PS4
::)
::IF "%INPUT%"=="3" (
::	set PLATFORM=PS5
::)
::IF "%INPUT%"=="4" (
::	set PLATFORM=XSX
::	set UbtArgs=-iostore
::)
::IF "%INPUT%"=="5" (
::	set PLATFORM=WinGDK
::	set UbtArgs=-iostore -ubtargs=-EnableGDK
::)
::IF "%INPUT%"=="5" (
::	set PLATFORM=Android
::	set UbtArgs=-ubtargs=-BypassCustomVQS
::)
::IF "%INPUT%"=="6" (
::	set PLATFORM=Switch
::	set UbtArgs=-iostore
::)

::ECHO ---- COOK FLAVOUR (Android only) ----
::ECHO 1 = Multi
::ECHO 2 = DXT
::ECHO 3 = ETC2
::ECHO 4 = ASTC
::echo ============================
::SET /P INPUTCookFlavor=Select cook flavor #:
::echo.
::echo.
::
::IF %INPUTCookFlavor% EQU 1 (
::	set COOKFLAVOR=Multi
::)
::IF %INPUTCookFlavor% EQU 2 (
::	set COOKFLAVOR=DTX
::)
::IF %INPUTCookFlavor% EQU 3 (
::	set COOKFLAVOR=ETC2
::)
::IF %INPUTCookFlavor% EQU 4 (
::	set COOKFLAVOR=ASTC
::)
::IF "%PLATFORM%"=="Android" (
::	set ANDROID_ADDITIONALS=-cookflavor=%COOKFLAVOR% -nodebuginfo -compile 
::)

ECHO ---- BUILD CONFIGURATION ----
ECHO 0 = DebugGame
ECHO 1 = Development
ECHO 2 = Shipping
ECHO 3 = Shipping Distribution
ECHO 4 = Test
echo ============================

SET /P BuildConfiguration= Please enter an BuildConfiguration: 
IF "%BuildConfiguration%" == "" (
	ECHO BuildConfiguration=1
	set BuildConfiguration=1
) 

ECHO 0 = Clear Cooking
ECHO 1 = Iterative
echo ============================
SET /P Iterative= Clear or iterative cooking?: 
IF %Iterative% EQU 1  (
	ECHO Iterative build
	set IsIterative=-iterativecooking
) 

ECHO 0 = Clear Build
ECHO 1 = Iterative
echo ============================
SET /P IterativeBuild= Clear or iterative build?: 
IF %IterativeBuild% EQU 0  (
	ECHO Clear build
	set IsIterativeBuild=-clean
) 

For /f "tokens=2-4 delims=/." %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
For /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a-%%b)
echo %mydate%_%mytime%

echo --- BUILD STARTED AT %date% %mytime% ---


IF %BuildConfiguration% EQU 0 (
	
	echo BUILDING DEBUG GAME

	set BuildConfiguration=DebugGame
	set CompressionLevel=Fast
) ELSE (

IF %BuildConfiguration% EQU 1 (

	echo BUILDING DEVELOPMENT
	
	set BuildConfiguration=Development
	set CompressionLevel=Fast
) ELSE (

IF %BuildConfiguration% EQU 2 (

	echo BUILDING SHIPPING
	
	set BuildConfiguration=Shipping
	set CompressionLevel=Optimal1
) ELSE (
IF %BuildConfiguration% EQU 3 (

	echo BUILDING SHIPPING
	
	set BuildConfiguration=Shipping
	set IsDistribution=-distribution
	set CompressionLevel=Optimal3
) ELSE (
IF %BuildConfiguration% EQU 4 (

	echo BUILDING Test
	
	set BuildConfiguration=Test
	set CompressionLevel=Optimal1
) ELSE (
	echo ERROR INPUT - BUILDING DEVELOPMENT
	
	set BuildConfiguration=Development
)))))

echo --- BUILD STARTED AT %date% %mytime% ---

CALL  "%~dp0..\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%~dp0SeriousTank.uproject" BuildCookRun -project="%~dp0SeriousTank.uproject" -platform=%PLATFORM% -configuration=%BuildConfiguration% -archive -archivedirectory="%~dp0../../Builds/Rev_%PLATFORM%_%BuildConfiguration%/" %ANDROID_ADDITIONALS% -nocompileeditor -unattended -utf8output -build -cook -stage -pak -prereqs -package -createreleaseversion=1.0 %IsIterativeBuild% %IsIterative% %IsDistribution% %UbtArgs%

echo F | xcopy "..\Engine\Engine\Programs\AutomationTool\Saved\Logs\Log.txt" ..\..\Builds\Rev_%PLATFORM%_%BuildConfiguration%\Log_%PLATFORM%.txt /Y /F
explorer ..\..\Builds\

pause
endlocal