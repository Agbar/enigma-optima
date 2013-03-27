# Shows all tasks

import atexit
from taskcntl import showTasks
from winaux import exit_keep

atexit.register(exit_keep)
showTasks()

