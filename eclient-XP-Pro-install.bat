@ECHO OFF
net user enigma-client nominal /add
mkdir C:\EnigmaClient
copy /-Y enigma.exe C:\EnigmaClient
copy /-Y dict\00trigr.cur C:\EnigmaClient
copy /-Y dict\00bigr.cur C:\EnigmaClient
copy /-Y enigma-client\ecrunXP.bat C:\EnigmaClient
copy /-Y enigma-client\enigma-client.py C:\EnigmaClient
copy /-Y enigma-client\plock.py C:\EnigmaClient
cacls C:\EnigmaClient /e /t /p enigma-client:f
schtasks /create /tn ecrunXP /tr C:\EnigmaClient\ecrunXP.bat /sc onstart /ru enigma-client /rp nominal 
schtasks /run /tn ecrunXP
::
echo.
echo.
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo @                                                          @
echo @      Enigma Client has been installed successfully.      @
echo @                                                          @
echo @      Open the Task Manager and check if                  @
echo @      enigma-client.exe is running:                       @
echo @                                                          @
echo @          Ctrl-Alt-Del -^> Processes                       @
echo @                                                          @
echo @                                                          @      
echo @      You can now close this window.                      @           
echo @                                                          @
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo.
cmd /k echo.

