//********************************************************************************************** 
// 
// ReplayPC A utility for extracting mpg files from ReplayTV units
// with TCP/IP networking
// 
// Copyright (C) 2002 Matthew T. Linehan and John Todd Larason
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
// 
// Please submit patches and sugestions to
// replaypc-developer@lists.sourceforge.net For the latest version of
// ReplayPC, visit the Project ReplayPC home page at
// replaypc.sourceforge.net
// 
//**********************************************************************************************

#include "rtv.h"
#include "httpfsclient.h"
#include "httpclient.h"
#include "guideclient.h"

#include <stdio.h>                  //cross platform includes
#include <stdlib.h>                 //cross platform includes
#include <string.h>                 //cross platform includes
#include <time.h>                   //cross platform includes

#ifdef _WIN32
#include <conio.h>                  //for PRESSANYKEY
#endif

#define MIN(x,y) ((x) < (y) ? (x) : (y))

//********************************************************************************************** 
// Global Variables and Defines 
//********************************************************************************************** 
//Macros to convert windows specific stuff to Unix equivilants
#ifdef __unix__                     
#define PRESSANYKEY                     //In Unix, we don't do this
#endif

#ifdef _WIN32
//Macro for Press any key to continue
#define PRESSANYKEY printf("Press any key to continue.\n"); getch();
#endif


//********************************************************************************************** 
// Function Prototypes 
//********************************************************************************************** 
void GetDir(char *IPAddress); 
void GetStatus(char *IPAddress, char *FileName); 
void GetMpgFile(char *IPAddress, char *FileName, int ToStdOut);
void GetReplayGuide(char *IPAddress, int ToStdOut); 
void DoCommand(char *IPAddress, char *Command, int ToStdOut); 

//********************************************************************************************** 
// Main Loop 
//********************************************************************************************** 
int main(int argc, char* argv[]) 
{ 
    int     i = 0;              //temp counter 

    //Operational Parameters
    char    *IPAddress  = NULL; //pointer to 4K Unit's IP Address as a string 
    char    *FileName   = NULL; //pointer to filename user supplied filename
    char    *Command    = NULL; //pointer to user supplied command string 
    int     fGetDir     = 0;    //Flag, true if were getting a /video directory
    int     fGetStat    = 0;    //Flag, true if were getting a file status report
    int     fGetFile    = 0;    //Flag, true if were getting a file 
    int     fStdOut     = 0;    //Flag, true if output to stdout instead of a file
    int     fGetGuide   = 0;    //Flag, true if were getting the guide file 
    int     fDoCommand  = 0;    //Flag, true if were sending a command 

    //Print usage summary if options are not supplied 
    if(argc < 3) 
    { 
        printf("\n");
        printf("ReplayPC V0.4.0, built on " __DATE__ " " __TIME__ "\n"); 
        printf("Copyright (C) 2002 Matthew T. Linehan\n");
        printf("ReplayPC comes with ABSOLUTELY NO WARRANTY;\n"); 
        printf("This is free software, and you are welcome to redistribute it under\n");
        printf("certain conditions as described in the GNU GENERAL PUBLIC LICENSE\n"); 
        printf("\n");
        printf("USAGE:\n");
        printf("   %s <4k-ipaddress> <options>\n", argv[0]); 
        printf("   -d             Print directory listing (ls command)\n");
        printf("   -s <filename>  Print file status (fstat command)\n");
        printf("   -e <filename>  Retrieve MPG File Named filename, write to disk\n"); 
        printf("   -g             Retrieve Replay Guide File, write to disk\n"); 
        printf("   -c <\"command\"> Send a command string using HTTP GET, write to disk\n");
        printf("   -o             Send retrieved file to stdout instead of disk\n");
        printf("\n");

        //make clueless windoze users press any key to continue
        PRESSANYKEY

        printf("EXAMPLES:\n");
        printf("   %s 192.168.0.4 -d\n", argv[0]); 
        printf("   %s 192.168.0.4 -s  1009609197.mpg\n", argv[0]); 
        printf("   %s 192.168.0.4 -e  1009609197.mpg\n", argv[0]); 
        printf("   %s 192.168.0.4 -g\n", argv[0]); 
        printf("   %s 192.168.0.4 -c \"/httpfs-readfile?pos=0&name=\"/Video/1009760397.mpg\"\"\n", argv[0]); 
        printf("   %s 192.168.0.4 -o -e  1009609197.mpg | playstream\n", argv[0]); 
        printf("   %s 192.168.0.4 -o -g | interpretguide\n", argv[0]); 
        printf("   %s 192.168.0.4 -o -c <httpfs_cmd> | <httpfs_cmd_parser>\n", argv[0]); 
        printf("\n");

        //make clueless windoze users press any key to continue
        PRESSANYKEY
        return -1; 
    } 

    //scan command line arguments 
    IPAddress = argv[1]; 
    for(i=2;i<argc;i++) 
    { 
        if(argv[i][0] == '-') 
        { 
            if(argv[i][1] == 'd') 
            { 
                fGetDir = 1; 
            } 
            else if(argv[i][1] == 's') 
            { 
                fGetStat = 1; 
                if(((i+1)<argc) && (argv[i+1][0] != '-')) 
                { 
                    i++; 
                    FileName = argv[i]; 
                } 
            } 
            else if(argv[i][1] == 'e') 
            { 
                fGetFile = 1; 
                if(((i+1)<argc) && (argv[i+1][0] != '-')) 
                { 
                    i++; 
                    FileName = argv[i]; 
                } 
            } 
            else if(argv[i][1] == 'o')
            {
                fStdOut = 1;
            }
            else if(argv[i][1] == 'g') 
            { 
                fGetGuide = 1; 
            } 
            else if(argv[i][1] == 'c') 
            { 
                fDoCommand = 1; 
                if(((i+1)<argc) && (argv[i+1][0] != '-')) 
                { 
                    i++; 
                    Command = argv[i]; 
                } 
            } 
            else 
            { 
                fprintf(stderr,"Ignored command line option: %s\n",argv[i]); 
            } 
        } 
    } 

    //Do as we are instructed 
    if (fGetDir)    GetDir(IPAddress);
    if (fGetStat)   GetStatus(IPAddress, FileName); 
    if (fGetFile)   GetMpgFile(IPAddress, FileName, fStdOut); 
    if (fGetGuide)  GetReplayGuide(IPAddress, fStdOut); 
    if (fDoCommand) DoCommand(IPAddress, Command, fStdOut); 

    //cleanup and exit 
    return 0; 
} 


//********************************************************************************************** 
// Print out 4K Directory Listing
//********************************************************************************************** 
void GetDir(char *IPAddress)
{
    int     i;
    time_t  TimeStamp=0;            //A Unix Timestamp
    char * data, * cur, * e;

    fprintf(stderr, "[Directory Listing of /Video...]\n"); 

    i = hfs_do_simple(&data, IPAddress,
                      "ls",
                      "name", "\"/Video\"",
                      NULL);
    if (i != 0) {
        fprintf(stderr, "hfs_do_simple returned error code %d.\n", i);
        exit(-1);
    }
    
    cur = data;
    while (cur && *cur) {
        e = strchr(cur, '\n');
        if (e)
            *e = '\0';
        
        //if the file name is numeric
        if ((TimeStamp = (time_t)strtoul(cur, NULL, 10)) > 0) {
            //Round Timestamp to nearst minute, then print listing
            TimeStamp = ((TimeStamp+30)/60)*60;
            fprintf(stdout, "%s\tRecorded on %s", cur, asctime(localtime(&TimeStamp)));
        } else {
            fprintf(stdout, "%s\n", cur);
        }
        if (e)
            cur = e + 1;
        else
            cur = NULL;
    }

    //all done, cleanup as we leave 
    fprintf(stderr, "\n[End of listing.]\n");

    free(data);
}


//********************************************************************************************** 
// Print out a 4K file status report
//********************************************************************************************** 
void GetStatus(char *IPAddress, char *FileName)
{
    char VideoFileName[256];
    char * data;
    int i;

    fprintf(stderr, "[File stats of %s...]\n", FileName); 

    if (strlen(FileName) + strlen("\"/Video/\"") + 1 > sizeof VideoFileName) {
        fprintf(stderr, "Filename too long\n");
        exit(-1);
    }
    
    sprintf(VideoFileName, "\"/Video/%s\"", FileName);
    
    i = hfs_do_simple(&data, IPAddress,
                      "fstat",
                      "name", VideoFileName,
                      NULL);
    
    if (i != 0) {
        fprintf(stderr, "HTTPFS_do_simple returned error code %d.\n", i);
        exit(-1);
    }

    fprintf(stdout, "%s", data);

    //all done, cleanup as we leave 
    fprintf(stderr, "\n[End of status.]\n");

    free(data);
}

//********************************************************************************************** 
// Retrieve the 4K Replay Guide and write to disk 
//********************************************************************************************** 
void GetReplayGuide(char *IPAddress, int ToStdOut) 
{
    unsigned char * result;
    unsigned char * timestamp;
    unsigned long size;
    unsigned long r;
    
    if (strlen(IPAddress) > 15) {
        fprintf(stderr, "Invalid IP address.\n");
        exit(-1);
    }

    //Tell the user were up to something 
    fprintf(stderr, "Retrieving ReplayGuide file...\n"); 

    r = guide_client_get_snapshot(&result, &timestamp, &size, IPAddress, "0");
    if (r) {
        fprintf(stderr, "get_snapshot: error %lx\n", r);
        exit(-1);
    }
    
    //Open the file we will use to store the guide data 
    if (ToStdOut) {
        fwrite(result, size, 1, stdout);
    } else {
        FILE * fp;
        if ((fp = fopen("guide.dat", "wb")) == NULL) { 
            perror("Error: GetGuide(): fopen()"); 
            exit(-1); 
        }
        fwrite(result, size, 1, fp);
        fclose(fp);
    }
    fprintf(stderr, "\nDone.\n"); 
}

struct mpgdata 
{
    FILE * out;
    u64 byteswritten;
};

static void GetMpgCallback(unsigned char * buf, size_t len, void * vd)
{
    struct mpgdata * data = vd;
    
    //write the data to disk 
    fwrite(buf, len, 1, data->out); 
    data->byteswritten += len;

    //update the progress report
    fprintf(stderr, "\r%"U64F"d KB received", data->byteswritten/1024);
    fflush(stderr);
}

//********************************************************************************************** 
// Retrieve a MPG file and write it to disk 
//********************************************************************************************** 
void GetMpgFile(char *IPAddress, char *FileName, int ToStdOut) 
{
    char pathname[256];
    struct mpgdata data;
    int i;

    if (strlen(FileName) + strlen("/Video/") + 1 > sizeof pathname) {
        fprintf(stderr, "Filename too long\n");
        exit(-1);
    }

    sprintf(pathname, "/Video/%s", FileName);

    //Open the file we will use to store the data 
    if (ToStdOut) {
        data.out = stdout;
    } else {
        if ((data.out = fopen(FileName, "wb")) == NULL) { 
            perror("Error: GetMpgData(): fopen()"); 
            exit(-1); 
        }
    }
    data.byteswritten = 0;
    
    //Tell the user we're up to something
    fprintf(stderr, "Retrieving /Video/%s...\n", FileName); 

    //Send the request for the file
    i = hfs_do_chunked(GetMpgCallback, &data, IPAddress, 75,
                       "readfile",
                       "pos", "0",
                       "name", pathname,
                       NULL);
    
    //all done, cleanup as we leave 
    fprintf(stderr, "\nDone.\n"); 
    fclose(data.out);
} 

struct cmddata 
{
    FILE * out;
    u64 byteswritten;
};

static void DoCommandCallback(unsigned char * buf, size_t len, void * vd)
{
    struct cmddata * data = vd;
    
    //write the data to disk 
    fwrite(buf, len, 1, data->out); 
    data->byteswritten += len;

    //update the progress report
    fprintf(stderr, "\r%"U64F"d KB received", data->byteswritten/1024);
    fflush(stderr);

    free(buf);
}

//********************************************************************************************** 
// Issue a command to the 4K using HTTP GET 
//********************************************************************************************** 
void DoCommand(char *IPAddress, char *Command, int ToStdOut) 
{
    struct hc * hc;
    char url[256];
    struct cmddata data;

    if (strlen(IPAddress) + strlen(Command) + strlen("http:///") + 1 > sizeof(url)) {
        fprintf(stderr, "IP address and command are too long\n");
        exit(-1);
    }
        
    sprintf(url, "http://%s/%s", IPAddress, Command);
    hc = hc_start_request(url);
    if (!hc) {
        perror("Error: DoCommand(): hc_start_request()");
        exit(-1);
    }
    hc_add_req_header(hc, "Authorization", "Basic Uk5TQmFzaWM6QTd4KjgtUXQ=" );
    hc_add_req_header(hc, "User-Agent", "Replay-HTTPFS/1");
    /* XXX gzip?  we don't, really, though */
    hc_add_req_header(hc, "Accept-Encoding", "text/plain");

    //Open the file we will use to store the result data 
    if (ToStdOut) {
        data.out = stdout;
    } else {
        if ((data.out = fopen("result.dat", "wb")) == NULL) { 
            perror("Error: DoCommand(): fopen()"); 
            exit(-1); 
        } 
    }
    data.byteswritten = 0;
    
    //Tell the user were up to something 
    fprintf(stderr, "Sending command \"%s\"...\n", Command); 

    hc_send_request(hc);

    /* XXX check status */

    hc_read_pieces(hc, DoCommandCallback, &data);
    
    //all done, cleanup as we leave 
    fprintf(stderr, "\nDone.\n"); 
    fclose(data.out);
    hc_free(hc);
} 



