
from distutils.core import setup
import py2exe, sys


# From spambayes source code:
#
# ModuleFinder can't handle runtime changes to __path__, but win32com uses them.

try:
    import modulefinder
    import win32com
    for p in win32com.__path__[1:]:
        modulefinder.AddPackagePath("win32com", p)
    for extra in ["win32com.shell","win32com.taskscheduler"]:
        __import__(extra)
        m = sys.modules[extra]
        for p in m.__path__[1:]:
            modulefinder.AddPackagePath(extra, p)
except ImportError:
    # no build path setup, no worries.
    pass


setup( console=["enigma-client.py", "eclient-rpc.py", "install.py",
                "killall.py", "showproc.py", "showtasks.py",
                "startclients.py", "stopclients.py", "uninstall.py"] )
