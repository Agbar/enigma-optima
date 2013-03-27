## newTask() is derived from test_addtask.py, which is part of win32comext.
## The original test_addtask.py is copyright (c) 1994-2001, Mark Hammond,
## see below for the License.
##
## Modifications and other functions by Stefan Krah, 2006.
## You have to keep Mark Hammond's License.


###########################################################################
#                  License of the original test_addtask.py                #
###########################################################################
#
# Unless stated in the specific source file, this work is
# Copyright (c) 1994-2001, Mark Hammond 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions 
# are met:
# 
# Redistributions of source code must retain the above copyright notice, 
# this list of conditions and the following disclaimer.
# 
# Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in 
# the documentation and/or other materials provided with the distribution.
# 
# Neither name of Mark Hammond nor the name of contributors may be used 
# to endorse or promote products derived from this software without 
# specific prior written permission. 
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import pythoncom, time
from win32com.taskscheduler import taskscheduler
from winaux import mesg
ts = pythoncom.CoCreateInstance( taskscheduler.CLSID_CTaskScheduler, None,
                                 pythoncom.CLSCTX_INPROC_SERVER,
                                 taskscheduler.IID_ITaskScheduler )

def showTasks():
    """ Displays all tasks. """
 
    tasks = ts.Enum()
    for t in tasks:
        print t
 
def haveTask(s):
    """ Checks for task where substring s is part of the task name. """
 
    tasks = ts.Enum()
    for t in tasks:
        if s in t: return 1
    return 0

def startTasks(s):
    """ Starts all tasks where substring s is part of the task name. """

    tasks = ts.Enum()
    match = 0
    for t in tasks:
        if s in t:
            match = 1
            act = ts.Activate(t)
            if act.GetStatus() == taskscheduler.SCHED_S_TASK_RUNNING:
                mesg('%s is already running\n', t)
            else:
                mesg("Starting task %s... ", t)
                act.Run()
                time.sleep(1)
                act = ts.Activate(t)
                if act.GetStatus() == taskscheduler.SCHED_S_TASK_RUNNING:
                    mesg("success\n")
                else:
                    mesg("failure\n")

    return match

def rmTasks(s):
    """ Removes all tasks where substring s is part of the task name. """

    tasks = ts.Enum()
    for t in tasks:
        if s in t:
            print 'Deleting existing task %s' % t
            ts.Delete(t)

def newTask(d, start):
    """ Creates new task according to specifications in d. Removes
        existing task with the same name. If start is true, task is
        started instantly.
    """

    if not d['TASKNAME'].endswith('.job'):
        d['TASKNAME'] += '.job'

    tasks = ts.Enum()
    if d['TASKNAME'] in tasks:
        print 'Deleting existing task %s' % d['TASKNAME']
        ts.Delete(d['TASKNAME'])

    t = ts.NewWorkItem(d['TASKNAME'])
    t.SetCreator(d['CREATOR'])
    t.SetAccountInformation(d['LOGIN'], d['PASSWD'])
    t.SetApplicationName(d['APPNAME'])
    t.SetWorkingDirectory(d['SETCWD'])
    t.SetMaxRunTime(d['MAXRUNTIME'])
    t.SetComment(d['COMMENT'])

    # Determine task flags, trigger type:
    flags = ( taskscheduler.TASK_FLAG_DONT_START_IF_ON_BATTERIES
             |taskscheduler.TASK_FLAG_KILL_IF_GOING_ON_BATTERIES )
    if d['HIDDEN']:
        flags |= taskscheduler.TASK_FLAG_HIDDEN
    if d['PASSWD'] is None:
        flags |= taskscheduler.TASK_FLAG_RUN_ONLY_IF_LOGGED_ON
        trigger_type = taskscheduler.TASK_EVENT_TRIGGER_AT_LOGON
    else:
        trigger_type = taskscheduler.TASK_EVENT_TRIGGER_AT_SYSTEMSTART

    # Set task flags:
    t.SetFlags(flags)

    tr_ind, tr = t.CreateTrigger()
    tt = tr.GetTrigger()
 
    # Set trigger flags (default to TASK_TRIGGER_FLAG_DISABLED (4)):
    tt.Flags = taskscheduler.TASK_TRIGGER_FLAG_KILL_AT_DURATION_END

    # Set trigger type:
    tt.TriggerType = trigger_type
    tr.SetTrigger(tt)
 
    pf = t.QueryInterface(pythoncom.IID_IPersistFile)
    pf.Save(None,1)
 
    print t.GetTriggerString(tr_ind)

    if start:
        # Run task instantly:
        act = ts.Activate(d['TASKNAME'])
        act.Run()


if __name__ == '__main__':

    import win32api
    from winaux import quote

    USER = win32api.GetUserName()
    d = {
          'TASKNAME'  : 'RunNotepad',
          'CREATOR'   : USER,
          'LOGIN'     : USER,
          'PASSWD'    : None,
          'APPNAME'   : quote('C:\\windows\\system32\\notepad.exe'),
          'SETCWD'    : quote('C:\\Documents And Settings\\All Users'),
          'MAXRUNTIME': -1,
          'COMMENT'   : 'Just another test.',
          'HIDDEN'    : 1
        }

    start = 1
    newTask(d, start)


