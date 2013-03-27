# Uninstall (partly) the M4 client.


import time, atexit
from winaux import *
from killall import *
from taskcntl import rmTasks, haveTask


atexit.register(exit_keep)


# Do you really want this? ;)
answer = raw_input_strip("\nDo you want to remove all enigma-client tasks Y/N [N]? ")
if not answer or not answer[0] in "Yy":
    mesg("\n\n    @@@ Uninstall aborted. @@@\n\n")
    sys.exit(1)
mesg("\n")


# Stop all client processes:
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


# Remove tasks:
if haveTask('ecrunXP'):
    rmTasks('ecrunXP')
else:
    mesg("No ecrunXP tasks found... ok\n")


# Remove user enigma-client:
if haveUser("enigma-client"):
    # Do you really want this? ;)
    answer = raw_input_strip("\nDo you want to remove user enigma-client Y/N [N]? ")
    if not answer or not answer[0] in "Yy":
        mesg("\n\n    @@@ Uninstall aborted. @@@\n\n")
        sys.exit(1)
    mesg("\n")
    
    mesg("Removing user enigma-client... ")
    if delUser('enigma-client') == 0:
        mesg("success\n")
    else:
        mesg("failure\n")
else:
    mesg("No user enigma-client found... ok\n")
 

mesg("""

    !!! Remember to remove the run directories. If you had !!!
    !!! a user enigma-client, remove the home directory of !!!
    !!! enigma-client as well.                             !!!

""")


#
# This file is part of enigma-suite-0.76, which is distributed under the terms
# of the General Public License (GPL), version 2. See doc/COPYING for details.
#
# Copyright (C) 2006 Stefan Krah
#
