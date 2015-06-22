# replaypc
A simple text mode utility for extracting mpg files from ReplayTV Personal Video Recorders via TCP/IP.

This utility is Linux, Win32 and Mac OS X native, and is being developed with an eye for easy porting to other operating systems.

This is an archive of an old project from 2002 from sourceforge (https://sourceforge.net/projects/replaypc/) that I'm importing here in case it vanishes there someday.

This was originally authored by Matthew Linehan, Todd Larason and myself

#

~~Please submit patches and sugestions to replaypc-developer@lists.sourceforge.net For the latest version of ReplayPC, visit the Project ReplayPC home page at replaypc.sourceforge.net~~

```
ReplayPC Usage:
   replaypc <4k_ipaddress> <options>
   -d             Print directory listing (ls command)
   -s <filename>  Print file status (fstat command)
   -e <filename>  Retrieve MPG File Named filename, write to disk
   -g             Retrieve Replay Guide File, write to disk
   -c <"command"> Send a command string using HTTP GET, write to disk
   -o             Send retrieved file to stdout instead of disk

   <4k_ipaddress> is the TCP/IP addreess of the 4000 series ReplayTV unit
   The following examples assume a ReplayTV at 192.168.0.4

   To get a directory listing of Video files available on the ReplayTV use:
     ReplayPC 192.168.0.4 -d

   To get the status of a mpg or ndx file on the ReplayTV, use:
     ReplayPC 192.168.0.4 -s 1009760397.mpg
     ReplayPC 192.168.0.4 -s 1009760397.ndx

   To fetch a mpg or ndx file from the ReplayTV and write it to disk use:
     ReplayPC 192.168.0.4 -e 1009760397.mpg
     ReplayPC 192.168.0.4 -e 1009760397.ndx

   To fetch the ReplayTV guide file and write it to guide.dat use:
     ReplayPC 192.168.0.4 -g
    
   The -o option can be used with any ReplayPC command that writes a file
   to disk. -o will make ReplayPC send the file data to stdout instead.

   To fetch a mpg or ndx file and send it to stdout instead of disk use:
     ReplayPC 192.168.0.4 -o -e 1009760397.mpg
     ReplayPC 192.168.0.4 -o -e 1009760397.ndx

   To fetch the ReplayTV guide file and send it to stdout use:
     ReplayPC 192.168.0.4 -o -g

   The -c option is used to send any arbitrary http get command to the
   ReplayPC. Any data that the ReplayTV returns is written to result.dat.
   The -o option can be used to divert output from disk to stdout.
     ReplayPc 192.168.0.4 -c "/httpfs-readfile&name="/Video/1009760397.mpg""	
     ReplayPc 192.168.0.4 -o -c <httpfs_cmd> | <httpfs_cmd_parser>	
```


The Windows version of ReplayPC is built using MSVC++ 6.0 on a Win2K system by Matt Linehan who can be reached at ~~mlinehan@users.sourceforge.net~~. To build the Windows version of ReplayPC, in Microsoft Visual C++, create a new "Win32 Console Application" called ReplayPC. Tell the wizard that you want an empty project. Add the ReplayPC.cpp source file to the project and build it.

