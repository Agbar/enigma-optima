#
# Auxiliary functions for the windows installer 
#


import win32api, win32file, win32net, win32security
import pywintypes, ntsecuritycon, win32netcon
from win32com.shell import shell, shellcon
from random import choice
import os, sys, string


# User dialog:
def mesg(format, *args):
    sys.stdout.write(format % args)
    sys.stdout.flush()

def raw_input_strip(s):
    return raw_input(s).strip()

def exit_keep():
    os.system("cmd /k echo.")

def quote(s):
    return '"' + s + '"'


# Information about current user:
def LookupAliasFromRid(TargetComputer, Rid):
    """Based on: Mark Hammond, python-win32 mailing list:
       A Python port of the MS knowledge base article Q157234
    """
    sid = pywintypes.SID()
    sid.Initialize(ntsecuritycon.SECURITY_NT_AUTHORITY, 2)

    for i, r in enumerate((ntsecuritycon.SECURITY_BUILTIN_DOMAIN_RID, Rid)):
        sid.SetSubAuthority(i, r)

    name, domain, type = win32security.LookupAccountSid(TargetComputer, sid)
    return name

def getLoginDetails():
    """ Get current user, domain controller. """
    user = win32api.GetUserName()
    domain = win32api.GetDomainName()
    hostname = win32api.GetComputerName()

    if domain == hostname:
        return (user, None)
    else:
        try:
            d = win32security.DsGetDcName(domainName=domain)
            return (user, d['DomainControllerName'])
        except win32security.error:
            return (user, None)

def haveAdminPriv():
    """ Test if current process has admin privileges. """
    flags = ntsecuritycon.TOKEN_READ
    htoken = win32security.OpenProcessToken(win32api.GetCurrentProcess(), flags)
    groups = win32security.GetTokenInformation(htoken, ntsecuritycon.TokenGroups)
    for g in groups:
        try:
            t = win32security.LookupAccountSid(None, g[0])
            name_admingroup = LookupAliasFromRid(None, ntsecuritycon.DOMAIN_ALIAS_RID_ADMINS)
            if t[0] == name_admingroup:
                return 1
        except win32security.error:
            pass
    return 0


USER, SERVERNAME = getLoginDetails()

if haveAdminPriv():
    SERVERNAME = None


# Users and passwords:
def getCurUser():
    """ Returns current user or '' on failure. """
    try:
        user = win32api.GetUserName()
    except win32api.error:
        return ''
    return user

def haveUser(user, servername=SERVERNAME):
    try:
        userinfo = win32net.NetUserGetInfo(servername, user, 0)
        return 1
    except win32net.error:
        return 0

def addUser(user, password, servername=SERVERNAME):
    userinfo = {}
    userinfo['name'] = user
    userinfo['password'] = password
    userinfo['comment'] = ""
    userinfo['flags'] = win32netcon.UF_NORMAL_ACCOUNT | win32netcon.UF_SCRIPT
    userinfo['priv'] = win32netcon.USER_PRIV_USER
    try:
        win32net.NetUserAdd(servername, 1, userinfo)
        return 0
    except win32net.error:
        return 1

def delUser(user, servername=SERVERNAME):
    try:
        win32net.NetUserDel(servername, user)
        return 0
    except win32net.error:
        return 1

def genPasswd(len=12, chars=string.ascii_letters+string.digits):
    return ''.join([choice(chars) for i in range(len)])

def setPasswd(user, password, servername=SERVERNAME):
    try:
        userinfo = win32net.NetUserGetInfo(servername, user, 2)
        userinfo['password'] = password
        win32net.NetUserSetInfo(servername, user, 2, userinfo)
    except win32net.error:
        mesg("\n    @@@ Warning: Could not set password for user %s. @@@\n\n", user)

def disablePasswdExpiry(user, servername=SERVERNAME):
    try:
        userinfo = win32net.NetUserGetInfo(servername, user, 3)
        userinfo['flags'] |= win32netcon.UF_DONT_EXPIRE_PASSWD
        win32net.NetUserSetInfo(servername, user, 3, userinfo)
    except win32net.error:
        mesg("\n    @@@ Warning: Could not disable password expiry for user %s. @@@\n\n", user)


# Get various paths:
def getCsidl(csidl):
    return shell.SHGetFolderPath(0, csidl, 0, 0)

def getInstallPrefix(csidl, role_account=None):
    try:
        # Path of user who runs the script
        p = shell.SHGetFolderPath(0, csidl, 0, 0)
    except:
        return ''
    cur_user = win32api.GetUserName()
    if role_account and cur_user != role_account:
        p = p.replace(cur_user, role_account)
    if not os.path.isdir(p):
        return ''
    else:
        return p


# Permission related:
def haveWritePerm(directory):
    """ Test a directory for write permissions """
    rand = genPasswd(len=32)
    pathname = directory + "\\" + rand
    if not os.path.exists(pathname):
        try:
            f = open(pathname, "w")
        except EnvironmentError:
            return 0
        f.close()
        win32file.DeleteFile(pathname)
        return 1

def addFullPerm(filename, user, servername=SERVERNAME):
    """ Give a user full permissions for filename. """
    try:
        sidUser = win32security.LookupAccountName(servername, user)[0]
        sd = win32security.GetFileSecurity( filename, 
                 win32security.DACL_SECURITY_INFORMATION )


        acl = sd.GetSecurityDescriptorDacl()
 
        for aceno in range(acl.GetAceCount()):
            csid = acl.GetAce(aceno)[2]
            if csid == sidUser:
                # We already have access
                if acl.GetAce(aceno)[1] == win32file.FILE_ALL_ACCESS:
                    return 1

        acl.AddAccessAllowedAce(win32file.FILE_ALL_ACCESS, sidUser)

        sd.SetSecurityDescriptorDacl(1, acl, 0)
        win32security.SetFileSecurity( filename, 
                   win32security.DACL_SECURITY_INFORMATION, sd )

    except win32security.error:
        return 0

    return 1

def addFullPermRec(filename, user):
    """ Give a user full permissions for filename, recursively. """ 
    cacls = getCsidl(shellcon.CSIDL_SYSTEM) + '\\cacls'
    filename = quote(filename)
    lastarg = quote(user + ':f')
    os.spawnv(os.P_WAIT, cacls, ['cacls', filename, '/e', '/t', '/p', lastarg])


