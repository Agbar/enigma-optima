@echo off
   if not '%1=='2nd start /m %0 2nd %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
   if not '%1=='2nd goto :EOF
   for %%v in (0 1) do shift
C:\Python23\python.exe enigma-client.py keyserver.bytereef.org 65521
:EOF
