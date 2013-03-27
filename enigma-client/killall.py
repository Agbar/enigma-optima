## Both functions are derived from killproc.py, which is part of win32api.
## The original killproc.py is copyright (c) 1994-2001, Mark Hammond, see
## below for the License.
##
## Modifications by Stefan Krah, 2006.
## You have to keep Mark Hammond's License.


###########################################################################
#                   License of the original killproc.py                   #
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

import sys, win32api, win32pdhutil, win32con
import win32security, ntsecuritycon


def haveProc(procname):
    """ Tests if procname is running. """
    try:
        win32pdhutil.GetPerformanceAttributes('Process', 'ID Process', procname)
    except:
        pass

    pids = win32pdhutil.FindPerformanceAttributesByName(procname)
    try:
        pids.remove(win32api.GetCurrentProcessId())
    except ValueError:
        pass

    if len(pids) == 0:
        return 0
    else:
        return 1


def killAllProcName(procname):
    """ Terminates _all_ processes with procname. Administrator can
        end processes owned by the system account.
    """
    try:
        win32pdhutil.GetPerformanceAttributes('Process','ID Process',procname)
    except:
        pass

    pids = win32pdhutil.FindPerformanceAttributesByName(procname)
    try:
        pids.remove(win32api.GetCurrentProcessId())
    except ValueError:
        pass

    success = 1
    if len(pids) > 0:
        priv = win32security.LookupPrivilegeValue(None, win32con.SE_DEBUG_NAME)
        newpriv = [(priv, win32con.SE_PRIVILEGE_ENABLED)]
        flags = ntsecuritycon.TOKEN_ADJUST_PRIVILEGES | ntsecuritycon.TOKEN_QUERY
        htoken = win32security.OpenProcessToken(win32api.GetCurrentProcess(), flags)
        win32security.AdjustTokenPrivileges(htoken, False, newpriv)
        result = []
        for p in pids:
            try:
                handle = win32api.OpenProcess(True, win32con.PROCESS_TERMINATE, p)
                win32api.TerminateProcess(handle, -1)
                win32api.CloseHandle(handle)
            except win32api.error:
                success = 0

    return success
 

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for procname in sys.argv[1:]:
            if not killAllProcName(procname):
                print "failed to end process %s" % procname
    else:
        print "Usage: killAllProcName.py procname ..."


