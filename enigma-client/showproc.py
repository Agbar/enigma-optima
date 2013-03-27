# Shows all processes

import atexit
from win32pdhutil import ShowAllProcesses
from winaux import exit_keep

atexit.register(exit_keep)
ShowAllProcesses()

