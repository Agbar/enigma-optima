#!/usr/bin/env python


import sys, os, shutil, time, string
import warnings, atexit
import socket, httplib, xmlrpclib
import win32net, pywintypes
from win32com.shell import shellcon
from getpass import getpass
from winaux import *
import killall
import taskcntl


USER = ''
PASSWD = None
CLIENT = None
ARGS = ''

PREFIX = ''
BASEDIR = 'EnigmaClient'

TIMEOUT = 20
PROXYTIMEOUT = 180
RPC_URL = 'http://keyserver.bytereef.org:443'

RUNNING_CLIENT_CHECK = 0
PASSWD_GENERATED = 0
CREATETASKS = 0
HIDDEN = 0

BLURB = """

                            Terms of Use
                           ==============

This program is distributed under the General Public License, version 2.
The license can be found in doc/COPYING.

You may install this software on a computer system only if you own
the system or have the permission of the owner.

"""


FGSTART="""%(CLIENT)s %(ARGS)s\n"""

ECRUN="""%(CLIENT)s %(ARGS)s 1>NUL 2>>logfile.txt\n"""


class ProxiedTransport(xmlrpclib.Transport):
    """This class is taken from the Python docs, release 2.5a1."""
    def set_proxy(self, proxy):
        self.proxy = proxy
    def make_connection(self, host):
        self.realhost = host
        h = httplib.HTTP(self.proxy)
        return h
    def send_request(self, connection, handler, request_body):
        connection.putrequest("POST", 'http://%s%s' % (self.realhost, handler))
    def send_host(self, connection, host):
        connection.putheader('Host', self.realhost)


class ConnectTest:
    """This class tests connections to the Enigma Key Server."""

    def __init__(self, host=None, port=0):
        """Constructor. When called without arguments,
           create an unconnected instance.
        """
        self.host = host
        self.port = port
        self.sock = None
        self.file = None
 
    def connect(self, host=None, port=0):
        """ Create socket and connect to a host on a given port."""
        self.sock = None
        self.host = host
        self.port = port
        try:
            for res in socket.getaddrinfo(host, port, 0, socket.SOCK_STREAM):
                af, socktype, proto, canonname, sa = res
                try:
                    self.sock = socket.socket(af, socktype, proto)
                    self.sock.connect(sa)
                except socket.error:
                    if self.sock:
                        self.sock.close()
                    self.sock = None
                    continue
                break
        except socket.gaierror:
            return

    def testGetChunkRpc(self, server):
        """Tests the connection to the XML-RPC server"""
        reply = (None,)
        try:
            reply = server.want_chunk()
        except xmlrpclib.Fault, fault:
            # print "%s" % fault.faultString
            return 0
        except (xmlrpclib.Error, socket.error), e:
            # print "%s" % e
            return 0
        status = reply[0]
        if status == 'TEMP_FAIL' or status == 'TRY_AGAIN_LATER' or status == 'SUCCESS':
            return 1
        else:
            return 0
    
    def close(self):
        """Close the connection to the enigma key server."""
        if self.file:
            self.file.close()
        self.file = None
        if self.sock:
            self.sock.close()
        self.sock = None
 
    def filewrite(self, filename, s):
        """Writes 's' to filename."""
        try:
            f = open(filename, 'w')
        except EnvironmentError:
            return 0
        try:
            f.write(s)
        except EnvironmentError:
            f.close()
            return 0
        f.close()
        return 1



""" main """

# Keep window open:
atexit.register(exit_keep)

eclient = ConnectTest()
if hasattr(socket, 'setdefaulttimeout'):
    socket.setdefaulttimeout(TIMEOUT)


# Legal blurb:
mesg(BLURB)
answer = raw_input_strip("\nI have read the license and I accept Terms of Use. Y/N [N]? ")
if not answer or not answer[0] in "Yy":
    mesg("\n\n    @@@ Installation aborted. @@@\n\n")
    sys.exit(1)


# End running client processes:
mesg("""

#############################################################
# All running clients have to be stopped before proceeding. #
#############################################################

""")

mesg("Checking for running clients... ")

if killall.haveProc('enigma-client') or killall.haveProc('enigma'):
    mesg(" yes\n\n")
    answer = raw_input_strip("Stop all running clients Y/N [Y]? ")
    if not answer or answer[0] in "Yy":
        killall.killAllProcName('enigma-client')
        time.sleep(1)
        killall.killAllProcName('enigma')
    else:
        mesg("\n\n    @@@ Installation aborted @@@\n\n")
        sys.exit(1)
else:
    mesg(" no\n\n")



# We need a user with a password for the task scheduler.
mesg("""

######################################################################
# If you want to run the client automatically in the background, you #
# need a user account with a password. It is recommended that you    #
# create a limited account for the purpose of running the client.    #
######################################################################

""" )

mesg("Checking for user enigma-client... ")
# We already have enigma-client.
if haveUser("enigma-client"):
    USER = "enigma-client"
    mesg("yes\n\n")
    answer = raw_input_strip("Create a new random password for enigma-client Y/N [Y]? ")
    if not answer or answer[0] in "Yy":
        PASSWD = genPasswd()
        PASSWD_GENERATED = 1
        setPasswd("enigma-client", PASSWD)
        disablePasswdExpiry("enigma-client")
    else:
        mesg("\nEnter the password twice. You will not see any output while typing.\n\n")
        match = 0
        while not match:
            pw1 = getpass()
            pw2 = getpass()
            if pw1 == pw2:
                match = 1
                PASSWD = pw1
            else:
                mesg("\nThe passwords do not match. Try again.\n\n")

else:
    mesg("no\n\n")
    # Creating enigma-client is recommended.
    answer = raw_input_strip("\nCreate user enigma-client with a random password Y/N [Y]? ")
    if not answer or answer[0] in "Yy":
        USER = "enigma-client"
        PASSWD = genPasswd()
        PASSWD_GENERATED = 1
        mesg("Creating user enigma-client... ")
        if addUser(USER, PASSWD) != 0:
            mesg("failure\n")
            mesg("\n\n    @@@ failed to create user enigma-client. @@@\n\n")
            sys.exit(1)
        else:
            disablePasswdExpiry("enigma-client")
            mesg("success\n\n")
    else:
        answer = raw_input_strip("\nDo you have an existing account with a password? Y/N [N]? ")
        USER = getCurUser()
        if answer and answer[0] in "Yy":
            response = raw_input_strip("\nEnter the user name: [%s]? " % USER)
            if response != "":
                USER = response
            if not haveUser(USER):
                mesg("\n\n    @@@ user %s does not exist. @@@\n\n", USER)   
                sys.exit(1)
            mesg("\nEnter the password twice. You will not see any output while typing.\n\n")
            match = 0
            while not match:
                pw1 = getpass()
                pw2 = getpass()
                if pw1 == pw2:
                    match = 1
                    PASSWD = pw1
                else:
                    mesg("\nThe passwords do not match. Try again.\n\n")
        else: 
            USER = getCurUser()


mesg("""

#####################################################################
# The installer is going to check the connection to the key server. #
#####################################################################

""" )


# No point in running this script when you are not connected.
answer = raw_input_strip("Are you connected to the Internet Y/N [N]? ")
if not answer or not answer[0] in 'Yy':
    mesg("\nAborting, please run the installer again when you are connected."
         "\n\n")
    sys.exit(1)


# Is bytereef.org reachable at all?
mesg("\ntrying whether the key server is up... ")
eclient.connect("keyserver.bytereef.org", 80)
if eclient.sock is not None:
    eclient.close()
    mesg("yes\n")
else:
    mesg("no\n")
    mesg("\n    @@@ The key server might be down. Try again later. @@@\n\n");
    sys.exit(1)
 

# Test port 65521.
mesg("trying port 65521... ")
eclient.connect("keyserver.bytereef.org", 65521)
if eclient.sock is not None:
    eclient.close()
    CLIENT = "enigma-client.exe"
    mesg("success\n\n")
else:
    mesg("no connection\n");
 

# Check for pyexpat.
LACK_PYEXPAT = 0
if CLIENT == None:
    try:
        import pyexpat
    except ImportError:
        LACK_PYEXPAT = 1
        warnings.filterwarnings('ignore', '', DeprecationWarning)


# Test port 443.
if CLIENT == None:
    mesg("trying direct connection to port 443... ")
    server = xmlrpclib.Server(RPC_URL)
    if eclient.testGetChunkRpc(server) == 1:
        CLIENT = "eclient-rpc.exe"
        mesg("success\n\n")
    else:
        mesg("no connection\n")


# Test port 443 via proxy.
if CLIENT == None:

    if hasattr(socket, 'setdefaulttimeout'):
        socket.setdefaulttimeout(PROXYTIMEOUT)

    mesg("\n    @@@ All direct connection attempts failed. @@@\n\n")

    proxy = raw_input_strip( "Enter a proxy server using the form"
                             " \"127.0.0.1:3128\": ");

    if proxy == "":
        mesg("\n    @@@ Aborting, you have to enter a proxy server. @@@\n")
        sys.exit(1)

    p = ProxiedTransport()
    p.set_proxy(proxy)
    server = xmlrpclib.Server(RPC_URL, transport=p)
    mesg("\ntrying port 443 via proxy at %s... " % proxy)
    if eclient.testGetChunkRpc(server) == 1:
        CLIENT = "eclient-rpc.exe"
        ARGS = "-p " + proxy + " "
        mesg("success\n\n")
    else:
        mesg("no connection\n\n")
        mesg("All connection attempts failed. Make sure that your connection\n"
             "to the Internet is working and/or check the proxy settings.\n\n")
        sys.exit(1)


# How many cores?
mesg("""
############################################################
# If you have more than one CPU/core, you can run several  #
# instances of the client.                                 #
############################################################

""")

while 1:
    response = raw_input_strip("Enter the number of CPUs/cores you want "
                               "to run the client on [1]: ")
    if response == "":
        response = 1
    try:
        response = int(response)
        if response >= 1 and response <= 32:
            break
    except ValueError:
        pass
    mesg("\n    @@@ Invalid response, try again. @@@\n\n")


# Determine suitable PREFIX:
PREFIX = getInstallPrefix(shellcon.CSIDL_PROGRAM_FILES)
if not haveWritePerm(PREFIX):
    PREFIX = getInstallPrefix(shellcon.CSIDL_PERSONAL, USER)
PREFIX += '\\M4Project'


# Example of run directory structure:
CORES = response
if CORES == 1:
    mesg( "\nThe installer will create this run directory: %s\n" \
                         % (PREFIX + "\\" + BASEDIR) )
else:
    mesg("\nThe installer will create these run directories: \n\n")
    mesg("%s\n" % (PREFIX + "\\" + BASEDIR))
    for i in range(1, CORES):
        mesg("%s\n" % (PREFIX + "\\" + BASEDIR + str(i+1)))


# Allow choice of prefix for the run directory.
while 1:
    response = raw_input_strip("\nChange the prefix [%s]: " % PREFIX)
    if response == "":
        break
    if not os.path.isabs(response):
        mesg("\n    @@@ Error: %s is not an absolute path. @@@\n" % response)
        continue
    if os.path.exists(response):
        if os.path.isdir(response):
            PREFIX = response
            break
        else:
            mesg("\n @@@ Error: %s exists and is not a directory. @@@\n" % response)
            continue
    else:
        answer = raw_input_strip("\n%s does not exist. Create it Y/N [N]? " % response)
        if answer and answer[0] in 'Yy':
            PREFIX = response
            break


# Allow choice of name for the run directory.
response = raw_input_strip("\nChange the name(s) [%s]: " % BASEDIR)
if response != "":
    BASEDIR = response


PREFIX = os.path.normpath(PREFIX)
BASEDIR = os.path.normpath(BASEDIR)


mesg("""

#######################################################################
# The installer can create and run the background task automatically. #
#######################################################################

""")


answer = raw_input_strip("Create it Y/N [Y]? ")
if not answer or answer[0] in "Yy":
    
    CREATETASKS = 1
    if getCurUser() == USER:
        HIDDEN = 1
    
    d = {
          'TASKNAME'  : '',
          'CREATOR'   : USER,
          'LOGIN'     : USER,
          'PASSWD'    : PASSWD,
          'APPNAME'   : '',
          'SETCWD'    : '',
          'MAXRUNTIME': -1,
          'COMMENT'   : '',
          'HIDDEN'    : HIDDEN
        }


# Do you really want all of this? :)
mesg("""

##############################
#          Summary           #
##############################

""")

mesg("User:     %s\n" % USER)
if PASSWD_GENERATED:
    mesg("Password: %s\n" % PASSWD)
mesg("Client:   %s\n" % CLIENT)

if CORES == 1:
    mesg( "\nThe installer will create/use this run directory: %s\n" \
                         % (PREFIX + "\\" + BASEDIR) )
else:
    mesg("\nThe installer will create/use these run directories: \n\n")
    mesg("%s\n" % (PREFIX + "\\" + BASEDIR))
    for i in range(1, CORES):
        mesg("%s\n" % (PREFIX + "\\" + BASEDIR + str(i+1)))

if CREATETASKS:
    mesg("\nThe installer will create and run the background task(s).\n\n")
    

answer = raw_input_strip("\nAccept Y/N [N]? ")
if not answer or not answer[0] in "Yy":
    mesg("\n\n    @@@ Installation aborted. @@@\n\n")
    sys.exit(1)

mesg("\n\n")

# Put the command line together.
if CLIENT == "enigma-client.exe":
    ARGS = "keyserver.bytereef.org 65521"
elif CLIENT == "eclient-rpc.exe":
    ARGS = ARGS + RPC_URL


# Create first run directory.
RUNDIR = PREFIX + "\\" + BASEDIR
if not os.path.exists(RUNDIR):
    os.makedirs(RUNDIR)

# If there are files from a previous installation, remove them.
for filename in ( "enigma.exe", "enigma-client.exe", "eclient-rpc.exe",
                  "ecrunXP.bat", "fgstart.bat" ):
    try:
        f = RUNDIR + "\\" + filename
        if os.path.exists(f):
            os.remove(f)
    except OSError:
        mesg("""

    @@@ Could not remove:                                    @@@

        %s
        
    @@@ Check if old clients are still using this directory. @@@

        ==>  Ctrl-Alt-Del  ==> Task Manager

""", f)
        sys.exit(1)


# Copy the new files.
shutil.copy("enigma.exe", RUNDIR)
shutil.copy("library.zip", RUNDIR)
shutil.copy(CLIENT, RUNDIR)

filename = RUNDIR + "\\" + "fgstart.bat"
s = FGSTART % {"RUNDIR": RUNDIR, "CLIENT": CLIENT, "ARGS": ARGS}
eclient.filewrite(filename, s)

filename = RUNDIR + "\\" + "ecrunXP.bat"
s = ECRUN % {"RUNDIR": RUNDIR, "CLIENT": CLIENT, "ARGS": ARGS}
eclient.filewrite(filename, s)


# Make sure that USER has write permissions.
oldcwd = os.getcwd()
os.chdir(PREFIX)
addFullPermRec(BASEDIR, USER)
os.chdir(oldcwd)
    

# Create the task:
if CREATETASKS:
    
    d['TASKNAME'] = 'ecrunXP'
    d['APPNAME'] = quote(RUNDIR + '\\' + 'ecrunXP.bat')
    d['SETCWD'] = quote(RUNDIR)
    d['COMMENT'] = "Enigma Client 1"

    try:
        taskcntl.newTask(d, 1)
        time.sleep(1)
    except pywintypes.com_error:
        mesg("""
    @@@ Warning: Could not create task. You might have entered a   @@@
    @@@          wrong user/password combination.                  @@@

        """)



# Create and populate additional run directories as needed.
for i in range(1, CORES):

    # Create additional run directory.
    CBASEDIR = BASEDIR + str(i+1)
    RUNDIR = PREFIX + "\\" + CBASEDIR
    if not os.path.exists(RUNDIR):
        os.makedirs(RUNDIR)

    # If there are files from a previous installation, remove them.
    for filename in ( "enigma.exe", "enigma-client.exe", "eclient-rpc.exe",
                      "ecrunXP.bat", "fgstart.bat" ):
        try:
            f = RUNDIR + "\\" + filename
            if os.path.exists(f):
                os.remove(f)
        except OSError:
            mesg("   @@@ Error: could not remove %s @@@\n\n", f)
            mesg("   @@@ Check if old clients are still using this directory. @@@\n")
            mesg("\n\n   ==>  Ctrl-Alt-Del  ==> Task Manager\n")
            sys.exit(1)

    # Copy the new files.
    shutil.copy("enigma.exe", RUNDIR)
    shutil.copy("library.zip", RUNDIR)
    shutil.copy(CLIENT, RUNDIR)

    filename = RUNDIR + "\\" + "fgstart.bat"
    s = FGSTART % {"RUNDIR": RUNDIR, "CLIENT": CLIENT, "ARGS": ARGS}
    eclient.filewrite(filename, s)

    filename = RUNDIR + "\\" + "ecrunXP.bat"
    s = ECRUN % {"RUNDIR": RUNDIR, "CLIENT": CLIENT, "ARGS": ARGS}
    eclient.filewrite(filename, s)

    # Make sure that USER has write permissions.
    oldcwd = os.getcwd()
    os.chdir(PREFIX)
    addFullPermRec(CBASEDIR, USER)
    os.chdir(oldcwd)

    # Create the task:
    if CREATETASKS:
    
        d['TASKNAME'] = 'ecrunXP' + str(i+1)
        d['APPNAME'] = quote(RUNDIR + '\\' + 'ecrunXP.bat')
        d['SETCWD'] = quote(RUNDIR)
        d['COMMENT'] = "Enigma Client %s" % str(i+1) 

        try:
            taskcntl.newTask(d, 1)
            time.sleep(1)
        except pywintypes.com_error:
            mesg("""
        @@@ Warning: Could not create task. You might have entered a   @@@
        @@@          wrong user/password combination.                  @@@

            """)



mesg("\n\n    $$$ Created run directories. $$$\n\n")
if CREATETASKS:
    mesg("    $$$ Started client(s)        $$$\n\n")



# Missing pyexpat.so warning:
if LACK_PYEXPAT:
    mesg("""
    !!!  python: DeprecationWarning: Could not find pyexpat.  !!!
    !!!                                                       !!!
    !!!  Depending on your distribution, consider installing  !!!
    !!!  python-expat or python-xml.                          !!!
    !!!  Otherwise you will have annoying but harmless        !!!
    !!!  deprecation warnings in your logs.                   !!!


""")


#
# This file is part of enigma-suite-0.76, which is distributed under the terms
# of the General Public License (GPL), version 2. See doc/COPYING for details.
#
# Copyright (C) 2006 Stefan Krah
#
