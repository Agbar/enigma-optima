#!/usr/bin/env python

# Credit (Python Cookbook): Jonathan Feinberg, John Nielsen
#
# Win9x code was apparently added by Richard Jones and should be
# covered by this license (http://roundup.sourceforge.net/):
#
# Copyright (c) 2003 Richard Jones (richard@mechanicalcat.net)
# Copyright (c) 2002 eKit.com Inc (http://www.ekit.com/)
# Copyright (c) 2001 Bizar Software Pty Ltd (http://www.bizarsoftware.com.au/)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# ATTENTION: roundup portalocker.py code was modified by Stefan Krah, 2006.
# This is why the module is called plock.py.


"""Cross-platform (posix/nt) API for flock-style file locking.

Synopsis:

   import plock
   file = open("somefile", "r+")
   plock.lock(file, plock.LOCK_EX)
   file.seek(12)
   file.write("foo")
   file.close()

If you know what you're doing, you may choose to:

   plock.unlock(file)

before closing the file, but why?

Methods:

   lock( file, flags )
   unlock( file )

Constants:

   LOCK_EX
   LOCK_SH
   LOCK_NB

Errors:
   Raises IOError if LOCK_NB is used and the lock cannot be acquired.
"""


import os

if os.name == 'nt':
    import win32con, win32file, pywintypes
    LOCK_EX = win32con.LOCKFILE_EXCLUSIVE_LOCK
    LOCK_SH = 0 # the default
    LOCK_NB = win32con.LOCKFILE_FAIL_IMMEDIATELY
    __overlapped = pywintypes.OVERLAPPED()
    FFFF0000 = -65536

    def lock(file, flags):
        """Try LockFileEx first, then move to (Win95, Win98, WinME) solutions."""
        hfile = win32file._get_osfhandle(file.fileno())
        try:
            win32file.LockFileEx(hfile, flags, 0, FFFF0000, __overlapped)
        except win32file.error, e:
            import winerror
            if e[0] != winerror.ERROR_CALL_NOT_IMPLEMENTED:
                if e[0] == winerror.ERROR_LOCK_VIOLATION:
                    raise IOError
                else:
                    raise e
            if not flags & LOCK_EX:
                import warnings
                warnings.warn("plock does not support shared locking on Win9x", RuntimeWarning)
            if flags & LOCK_NB:
                try:
                    win32file.LockFile(hfile, 0, 0, FFFF0000, 0)
                except win32file.error, e:
                    if e[0] == winerror.ERROR_LOCK_VIOLATION:
                        raise IOError
                    else:
                        raise e
            else:
                import time
                while 1:
                    try:
                        win32file.LockFile(hfile, 0, 0, FFFF0000, 0)
                        break
                    except win32file.error, e:
                        if e[0] != winerror.ERROR_LOCK_VIOLATION:
                            raise e
                    time.sleep(0.1)

    def unlock(file):
        """Try UnlockFileEx first, then move to (Win95, Win98, WinME) solutions."""
        hfile = win32file._get_osfhandle(file.fileno())
        try:
            win32file.UnlockFileEx(hfile, 0, FFFF0000, __overlapped)
        except win32file.error, e:
            import winerror
            if e[0] != winerror.ERROR_CALL_NOT_IMPLEMENTED:
                raise e
            win32file.UnlockFile(hfile, 0, 0, FFFF0000, 0)

elif os.name == 'posix':
    import fcntl
    from fcntl import LOCK_EX, LOCK_SH, LOCK_NB
    def lock(file, flags):
        fcntl.flock(file.fileno(), flags)
    def unlock(file):
        fcntl.flock(file.fileno(), fcntl.LOCK_UN)

else:
    raise RuntimeError("plock only defined for nt and posix platforms")


if __name__ == '__main__':
    from time import time, strftime, localtime
    import sys
    import plock

    log = open('log.txt', "a+")
    try:
        plock.lock(log, plock.LOCK_EX | plock.LOCK_NB)
    except IOError:
        print "plock: error: another process is using log.txt"
        sys.exit(1)

    timestamp = strftime("%m/%d/%Y %H:%M:%S\n", localtime(time()))
    log.write( timestamp )

    print "Wrote lines. Hit enter to release lock."
    dummy = sys.stdin.readline()

    log.close()


