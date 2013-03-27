# Starts all tasks containing the string "ecrunXP".


import atexit
from taskcntl import startTasks
from winaux import mesg, exit_keep

atexit.register(exit_keep)

if startTasks("ecrunXP") == 0:
    mesg("Failure: could not find task matching ecrunXP.\n")
 
