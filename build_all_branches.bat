@echo off
setlocal enabledelayedexpansion

set PROJECT_NAME=ParallelCGP.exe

:: Define the build command (customize this for your project)
set BUILD_COMMAND=cmake -S ../.. -B . -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Boost"
set MAKE_COMMAND=make

set BUILD_FOLDER=win_build

if exist !BUILD_FOLDER! (
    rmdir /s /q !BUILD_FOLDER!
)
mkdir !BUILD_FOLDER!
cd !BUILD_FOLDER!

:: Fetch all branches
echo Fetching all branches...
git fetch --all

:: Get a list of all branches
for /f "tokens=*" %%B in ('git branch -r ^| findstr /v "HEAD"') do (
    set BRANCH=%%B
    set BRANCH=!BRANCH:origin/=!

    echo ==========================================
    echo Checking out branch: !BRANCH!
    echo ==========================================
    git checkout !BRANCH! || exit /b 1

    :: Create a separate folder for the branch
    set BUILD_DIR=!BRANCH!
    if exist !BUILD_DIR! (
        rmdir /s /q !BUILD_DIR!
    )
    mkdir !BUILD_DIR!
    cd !BUILD_DIR!

    :: Run the build command
    echo Building branch: !BRANCH!
    %BUILD_COMMAND%
    %MAKE_COMMAND%
    if errorlevel 1 (
        echo Build failed for branch: !BRANCH!
        cd ..
        exit /b 1
    )

    :: Keep only the executable and delete other files
    echo Cleaning up build directory: !BUILD_DIR!
    for /f "delims=" %%F in ('dir /b /a-d') do (
        if not "%%F"=="%PROJECT_NAME%" (
            del "%%F"
        )
    )
    for /d %%D in (*) do (
        rmdir /s /q "%%D"
    )

    cd ..
)

echo ==========================================
echo All branches have been built successfully!
echo ==========================================