@echo off
 
echo f | xcopy.exe /q /s "%~dp0..\Release\shesc.exe" "%USERPROFILE%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\shesc.exe" 1>NUL

IF %ERRORLEVEL%==0 (
    echo shesc.exe copied to startup folder successfully.
) ELSE IF %ERRORLEVEL%==1 (
    echo No files were found to copy!
) ELSE IF %ERRORLEVEL%==2 (
    echo shesc.exe exists in startup folder!
) ELSE (
    echo Invalid error.
)

Pause
