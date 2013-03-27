
After compiling you will have the following main programs:

enigma.exe         -  a command line Enigma simulator and
                      message breaking program

enigma-client.py   -  client for a distributed attack on a message

eclient-rpc.py     -  XML-RPC client for a distributed attack on a message


These are helper programs for the client:

install.py         -  installer
uninstall.py       -  uninstaller

showproc.py        -  shows all processes
showtasks.py       -  shows all tasks
stopclients.py     -  stops task scheduler supervised clients
startclients.py    -  starts task scheduler supervised clients


The tools directory will contain these programs:
  
mkdict.exe  -  creates log-ngram dictionary from sampletext

SGT.exe     -  does Good-Turing smoothing for mkdict; included
               by courtesy of Professor Geoffrey Sampson of the
               University of Sussex


It is probably best to start by reading the enigma(1) man page or
enigma.txt. You can also go straight to the EXAMPLES section in the
man page to get a quick idea of how things are supposed to work.



Note:

The command line syntax for the simulator was taken over from enigma-0.5.6.sh,
a command line bash (!) Enigma emulator written by David Barlow and Josh Holt.



Author:

Stefan Krah <stefan@bytereef.org>


Bugs:

Bug reports, suggestions should go to:  <enigma-suite@bytereef.org>

