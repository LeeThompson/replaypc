ReplayPC 0.4
-------------------------------------------------------------------------------
ReplayPC is available in both source and binary form.  In binary form, it
consists of a set of executables intended to be useful to a wide community of
ReplayTV 4000 and 4500 users; in source form, it contains source for those
executables, several executables of more limited interest, and a common
library.

This Readme only attempts to document the widely-useful binaries.

All of the distributed binaries give usage summaries when executed without
arguments.


ReplayPC: This is the current version of the grandfather of the whole project,
          Matthew Linehan's original ReplayPC client.  This packs most of the
          basic functionality into one executable, but is in some ways not as
          flexible or well-suited for building other tools on top of as the
          others.

	  This version is intended to be fully compatible from a user's point
	  of view with ReplayPC 0.3.  Depending on how it's built, it's
	  compatible with either ReplayTV software 4.1 or 4.3, but not both.

	  
httpfs:	  This is a more flexible file-transfer client, which is hopefully
	  also more suited for building higher-level tools on top of.

	  It supports the full range of file-over-http actions available from
	  the Replay software, and can be configured (either via a
	  command-line switch or an environment variable) to work with eith
	  version 4.1 or 4.3.

	  The ReplayPC command "ReplayPC 192.168.117.3 -e 1009609197.mpg" is
	  equivalent to the httpfs command "httpfs -a 192.168.117.3 readfile
	  -f 1009609197.mpg /Video/1009609197.mpg" -- verbosity is traded for
	  flexibility.  The address can be set with an environment variable
	  rather than on the command line, to avoid repeating it for each
	  command.

rtvguide: This is a more flexible and featureful guide-transfer client.

	  The ReplayPC command "ReplayPC 192.168.117.3 -g" is equivalent to
	  the rtvguide command "rtvguide -a 192.168.117.3 get -f guide.dat";
	  again, flexibility and verbosity trade off, and again an environment
	  variable can be used to specify the address.

	  rtvguide can also be used in automatic mode, with the "-a" switch to
	  the get subcommand; this will save the guide as a file named
	  "snapshot.<address>.<timestamp>", and output the timestamp; if the
	  guide hasn't been modified since the last automatic use, it won't be
	  transferred again.  This will hopefully be useful to other tools.

dump-guide: This is a simple formatter for the guide data; it displays every
	  last bit of information in a guide file in a human readable (but not
	  terribly pretty) format; Lee Thompson's GuideParser is more suitable
	  for human usage, while this is (again) hopefully useful to build
	  higher-level tools on.

rddns:	  This is a simple client for the ReplayTV Dynamic DNS protocol, used
	  to find other Replay units for Internet Video Sharing.

mark-commercial:  This is a semi-automated editor for ndx files, allowing
		  sections to be marked as being part of a commercial break or
		  not.

