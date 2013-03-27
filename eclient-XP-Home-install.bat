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
::
echo.
echo.
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo @                                                                @
echo @      Created user "enigma-client".                             @
echo @                                                                @
echo @      Created run folder C:\EnigmaClient.                       @
echo @                                                                @
echo @      You can now close this window.                            @
echo @                                                                @
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
echo.
cmd /k echo.

