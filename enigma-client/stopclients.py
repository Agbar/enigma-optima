# Stops all processes named "enigma-client".
# Stops all processes named "enigma".

import time, atexit
from killall import *
from winaux import mesg, exit_keep 

 
atexit.register(exit_keep)

 
if haveProc('enigma-client'):
    mesg("Stopping all enigma-client processes... ")
    if killAllProcName('enigma-client'):
        mesg("success\n")
    else:
        mesg("failure\n")
else:
    mesg("No enigma-client processes running... ok\n")
 
time.sleep(1)

if haveProc('enigma'):
    mesg("Stopping all enigma processes... ")
    if killAllProcName('enigma'):
        mesg("success\n")
    else:
        mesg("failure\n")
else:
    mesg("No enigma processes running... ok\n")

