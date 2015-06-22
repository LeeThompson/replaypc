//**********************************************************************************************
//
// ReplayPC.c  Retrieve the guide file and .mpg files from a 4000 series ReplayTV via network
// WARNING - This is ALPHA software. Don't try to use this unless you know what your doing!
//
//
//  Release history:
//  v0.1    01/01/2002  First Souce Code Release, Just to let others help in this effort
//                      This code comiles with VC++ 6.0, but I've attempted to keep it as
//                      portable as possible to other operating systems. Thats why I chose
//                      to write this as a Win32 Console Application. Microsoft makes this hard!
//                      Development and testing were done on a PII-400 Win2K system	
//
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// If you distribute a modified version of this program, the program MUST tell the user
// in the usage summary how the program has been modified, and who done it!
// Modified source code MUST be distributed with the modified program.
//
// Please submit patches and sugestions to mlinehan@columbus.rr.com
//
//**********************************************************************************************
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <winsock2.h>

//Tell the linker to link with the winsock library
#pragma comment(lib, "ws2_32.lib")


//**********************************************************************************************
// Global Variables and Defines
//**********************************************************************************************



//**********************************************************************************************
// Function Prototypes
//**********************************************************************************************
void GetReplayGuide(char *IPAddress);
void GetMpgFile(char *IPAddress, char *FileName);
int  RecieveHTTPData(SOCKET s, char* Buffer, int Len, char Terminator);


//**********************************************************************************************
// Main Loop
//**********************************************************************************************
int main(int argc, char* argv[])
{
    WSADATA wsaData;            //Winsock Garbage
    LPVOID  Dummy       = NULL; //Winsock Garbage
    int     fGetGuide   = 0;    //Flag, true if were getting the guide file
    int     fGetFile    = 0;    //Flag, true if were getting a file
    char    *FileName   = NULL; //pointer to filename
    char    *IPAddress  = NULL; //pointer to ip address as a string
    int     i           = 0;    //temp counter

    //fire up windows sockets version 2.2
    if(WSAStartup(MAKEWORD( 2, 2 ), &wsaData ) == -1)
    {
        perror("WSAStartup() Failed\n");
        exit(1);
    }

    //Print usage summary if options are not supplied
    if(argc < 3) 
    {
        printf("ReplayPC V0.1, built on " __DATE__ " " __TIME__ "\n");
        printf("USAGE: %s <4k-ipaddress> <options>\n", argv[0]);
        printf("  -g            Retrieve Replay Guide File, Write to Disk\n");
        printf("  -e filename   Retrieve MPG File Named filename, Write to Disk\n");
        printf("EXAMPLE: %s 192.168.0.4 -g\n", argv[0]);
        printf("EXAMPLE: %s 192.168.0.4 -e 1009609197.mpg\n", argv[0]);
        return -1;
    }
	
    //scan command line arguments
    IPAddress = argv[1];
    for(i=2;i<argc;i++) 
    {
        if(argv[i][0] == '-') 
		{
            if(argv[i][1] == 'g') 
            {
                fGetGuide = 1;		
            } 
            else if(argv[i][1] == 'e') 
            {
                fGetFile = 1;
                if(((i+1)<argc) && (argv[i+1][0] != '-')) 
                {
                    i++;
                    FileName=argv[i];
                } 
            } 
            else 
            {
                fprintf(stderr,"Ignored command line option: %s\n",argv[i]);
            }
        }
    }
	
    //Do as we are instructed
    if (fGetGuide) GetReplayGuide(IPAddress);
    if (fGetFile)  GetMpgFile(IPAddress, FileName);

    //shutdown windows sockets
    WSACleanup( );

    //cleanup and exit
    return 0;
}


//**********************************************************************************************
// Retrieve the 4K Replay Guide and write to disk
//**********************************************************************************************
void GetReplayGuide(char *IPAddress)
{
    SOCKET  s;                      //A socket request the guide data with
    struct  sockaddr_in adr_dst;    //Destination IPAddress structure
    char    Buffer[1500];           //A Buffer for our work
    int     len, chunklen;          //a temp for dealing with the buffer
    FILE    *fileptr;               //a normal file pointer
    int     Done;                   //temps

    //A HHTP request for the ReplayGuide File
    char ReqGuide[]="GET /http_replay_guide-get_snapshot?guide_file_name=0&serial_no=RTV4080K0000000000 HTTP/1.1\r\n"
                    "Host: 192.168.0.4:80\r\n"
                    "Accept-Encoding: gzip\r\n"
                    "\r\n";

    //fill the destination address/port structure
    memset(&adr_dst, 0, sizeof(adr_dst));
    adr_dst.sin_family = AF_INET;
    adr_dst.sin_port = htons(80);
    adr_dst.sin_addr.s_addr = inet_addr(IPAddress);

    //get a socket to work with
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        perror("GetGuide:socket() Failed\n");
        exit(1);
    }

    //Open the file we will use to store the guide data
    if ((fileptr = fopen("Guide.dat", "wb")) == NULL)
    {
        perror("GetGuide:fopen() Failed\n");
        exit(1);
    }

    //Initiate the connection to the 4K
    if (connect(s, (struct sockaddr*)&adr_dst, sizeof(adr_dst)) == -1)
    {
        perror("GetGuide:connect() Failed\n");
        exit(1);
    }
	
    //Tell the user were up to something
    printf("Retrieving ReplayGuide file...\n");

    //Send the request for the guide
    send(s, ReqGuide, strlen(ReqGuide), 0);

    //retrieve all the data
    Done=0;
    do
    {
        //Skip past the HTTP header junk by searching for the blank line
        do
        {
            len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x0A);
        }while(len != 2);

        //The next line should be a chunk lengh specifier
        len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x0A);
        sscanf(Buffer, "%x", &chunklen);
        printf(".", chunklen); 

        //If this chunk is non zero in length
        if (chunklen)
        {
            //Loop for chunklen bytes of data
            len=0;
            do
            {
                if (chunklen > sizeof(Buffer))
                {
                    //Recieve sizeof(Buffer) bytes of binary data
                    len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x00);
                    chunklen -= len;
                }
                else
                {
                    //Recieve chunklen bytes of binary data
                    len = RecieveHTTPData(s, Buffer, chunklen, 0x00);
                    chunklen = 0;
                }

                //write the data to disk
                fwrite(Buffer, len, 1, fileptr);

            }while(chunklen);

        }
        else
        {
            //chunklen is zero, signifying end of transmission
            Done=1;
        }
    }while(!Done);
	
    //all done, cleanup as we leave
    printf("\nDone, ReplayGuide written to [guide.dat].\n");
    fclose(fileptr);
    closesocket(s);
}


//**********************************************************************************************
// Retrieve a MPG file and write it to disk
//**********************************************************************************************
void GetMpgFile(char *IPAddress, char *FileName)
{
    SOCKET  s;                      //A socket request the guide data with
    struct  sockaddr_in adr_dst;    //Destination Address
    char    Buffer[1500];           //Buffer to recieve the guide data into
    int     len, chunklen;          //a temp for dealing with buffers
    FILE    *fileptr;               //a normal file pointer
    int     Done=0;                 //temp flag
    int     FirstTime=0;            //another flag
    char    ReqMpg[500];            //build the HTTP request here

    //build the HTTP request
    //NOTE: The Authorization is very important. This code works with my 4K
    //      I don't know if it will work for others....
    sprintf(ReqMpg, "GET /httpfs-readfile?pos=0&name=\"/Video/%s\" HTTP/1.1\r\n", FileName);
    strcat(ReqMpg, 	"Authorization: Basic Uk5TQmFzaWM6QTd4KjgtUXQ=\r\n");
    strcat(ReqMpg,	"User-Agent: Replay-HTTPFS/1\r\n");
    strcat(ReqMpg,	"Host: 192.168.0.2\r\n");
    strcat(ReqMpg,	"Accept-Encoding: gzip\r\n");
    strcat(ReqMpg,	"\r\n");

    //fill the destination address/port structure
    memset(&adr_dst, 0, sizeof(adr_dst));
    adr_dst.sin_family = AF_INET;
    adr_dst.sin_port = htons(80);
    adr_dst.sin_addr.s_addr = inet_addr(IPAddress);

    //get a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        perror("GetMpgFile:socket() Failed\n");
        exit(1);
    }

    //Open the file we will use to store the response data
    if ((fileptr = fopen(FileName, "wb")) == NULL)
    {
        perror("GetMpgFile:fopen() Failed\n");
        exit(1);
    }

    //Connect to the 4K
    if (connect(s, (struct sockaddr*)&adr_dst, sizeof(adr_dst)) == -1)
    {
        perror("GetMpgFile:connect() Failed\n");
        exit(1);
    }
	
    //Tell the user were up to something
    printf("Retrieving /Video/%s...\n", FileName);

    //Send the request for the file
    send(s, ReqMpg, strlen(ReqMpg), 0);
	

    //Initialize flags
    Done=0;
    chunklen=0;
    FirstTime=1;
    do
    {
        //Skip the HTTP header stuff by searching for the blank line
        do
        {
            len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x0A);
        }while(len != 2);

        //This line should be a chunk lengh specifier
        len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x0A);
        sscanf(Buffer, "%x", &chunklen);
        printf(".", chunklen); 

        //For some bizzar reason, the fist chunk has a line feed terminated
        //number before the mpg data, skip over it
        if (FirstTime)
        {
            chunklen -= RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x0A);
        }

        //If the chunk lenght is nonzero
        if (chunklen)
        {
            //Loop for chunklen bytes of data
            len=0;
            do
            {
                if (chunklen > sizeof(Buffer))
                {
                    //Recieve sizeof(Buffer) bytes of binary data
                    len = RecieveHTTPData(s, Buffer, sizeof(Buffer), 0x00);
                    chunklen -= len;
                }
                else
                {
                    //Recieve chunklen bytes of binary data
                    len = RecieveHTTPData(s, Buffer, chunklen, 0x00);
                    chunklen = 0;
                }

                //mpg file fixup from extract_rtv!
                if (FirstTime)
                {
                    // Write a small pack that includes a system header and a program stream map
                    // This fixes incompatibility issues with Womble MPEG-2 VCR and MProbe and
                    // makes the MPEG-2 files much more MPEG-2 compliant which apparently they
                    // don't need to be for the ReplayTV box to play them back.
                    FirstTime=0;
                    unsigned long rate_bound;
                    unsigned char pack_hdr[] = {0x00, 0x00, 0x01, 0xba,             // pack_start_code
                                                0x44, 0x00, 0x04, 0x00, 0x04, 0x01,	// system_clock_reference_base
                                                0x00, 0x6f, 0xe7,                   // program_mux_rate
                                                0xf8};                              // pack_stuffing_length
                    unsigned char sys_hdr[] =  {0x00, 0x00, 0x01, 0xbb,             // system_header_start_code
                                                0x00, 0x0c,                         // header_length
                                                0x80, 0x37, 0xf3,                   // rate_bound
                                                0x06,                               // audio_bound/CSPS_flag
                                                0xe1,                               // system_audio_lock_flag/system_video_lock_flag/video_bound
                                                0xff,                               // packet_rate_restriction_flag/reserved
                                                0xe0,                               // stream_id
                                                0xe0, 0xe0,                         // P-STD_buffer_bound_scale/P-STD_buffer_size_bound
                                                0xc0,                               // stream_id
                                                0xc0, 0x20};                        // P-STD_buffer_bound_scale/P-STD_buffer_size_bound
                    unsigned char psmap[] =	   {0x00, 0x00, 0x01,                   // packet_start_code_prefix
                                                0xbc,                               // map_stream_id
                                                0x00, 0x12,                         // program_stream_map_length
                                                0xe1,                               // current_next_indicator/program_stream_map_version
                                                0xff,                               // reserved
                                                0x00, 0x00,                         // program_stream_info_length
                                                0x00, 0x08,                         // elementary_stream_map_length
                                                0x02,                               // stream_type
                                                0xe0,                               // elementary_stream_id
                                                0x00, 0x00,                         // elementary_stream_info_length
                                                0x03,                               // stream_type
                                                0xc0,                               // elementary_stream_id
                                                0x00, 0x00,                         // elementary_stream_info_length
                                                0xaa, 0x58, 0x7b, 0xf9};            // CRC_32

                    // Make sure this is really an MPEG-2 file before we mess around with it
                    if ((Buffer[0]!=0x00) || (Buffer[1]!=0x00) || (Buffer[2]!=0x01) || (Buffer[3]!=(char)0xba))
                    {
                        printf("Not an MPG file! %X %X %X %X\n", (int) Buffer[0], (int) Buffer[1], (int) Buffer[2], (int) Buffer[3]);
                        exit(1);
                    }
						
                    // Copy the first pack header from the Replay's MPEG file
                    // but don't copy the pack_stuffing_length since we don't
                    // want any stuffing_bytes
                    memcpy(pack_hdr, Buffer, sizeof(pack_hdr) - 1);
                    fwrite(pack_hdr,1,sizeof(pack_hdr),fileptr);

                    // Set the system header's rate_bound to the same as the
                    // pack header's program_mux_rate. Since this is a CBR
                    // MPEG-2 file we're dealing with, these values can be
                    // the same
                    rate_bound = (pack_hdr[10] << 16 | pack_hdr[11] << 8 | pack_hdr[12]) >> 2;
                    rate_bound = (rate_bound << 1) | 0x800001;
                    sys_hdr[6] = (unsigned char)(rate_bound >> 16);
                    sys_hdr[7] = (unsigned char)(rate_bound >> 8);
                    sys_hdr[8] = (unsigned char)(rate_bound & 0xff);
                    fwrite(sys_hdr,1,sizeof(sys_hdr),fileptr);

                    //Finally, write the program stream map
                    fwrite(psmap,1,sizeof(psmap),fileptr);
                }

                //write the data to disk
                fwrite(Buffer, len, 1, fileptr);

            }while(chunklen);

        }
        else
        {
            //zero lengh chunk marks end of transmission
            Done=1;
        }
    }while(!Done);
	
    //all done, cleanup as we leave
    printf("\nDone, output written to [%s]\n", FileName);
    fclose(fileptr);
    closesocket(s);
}


//**********************************************************************************************
// A helper function for dealing with HTTP chuncked data
// We have to do this when using WinSock, because WinSock sockets are NOT streams
// in a Unix system a socket is also a file descriptor allowing use of stream I/O
//**********************************************************************************************
int RecieveHTTPData(SOCKET s, char *Buffer, int Len, char Terminator)
{
    static int PacketLen=0;
    static int SrcIndex=0;
    static int DstIndex=0;
    static char PacketBuffer[1500];
    char c;

    //Destination is zero lenght each call
    DstIndex=0;
	
    if (Terminator)
    {
        //Were retrieving terminated data
        do
        {
            //Retrieve HTTP data as required into PacketBuffer[]
            if (SrcIndex == PacketLen)
            {
                PacketLen = recv(s, PacketBuffer, sizeof(PacketBuffer), 0);
                SrcIndex = 0;
            }

            //Copy a byte from PacketBuffer[SrcIndex] to Buffer[DstIndex]
            c = Buffer[DstIndex++] = PacketBuffer[SrcIndex++];
            Buffer[DstIndex]=0x00;

        }while(c != Terminator);
    }
    else
    {
        //Were retrieving binary data
        do
        {
            //Retrieve HTTP data as required into PacketBuffer[]
            if (SrcIndex == PacketLen)
            {
                PacketLen = recv(s, PacketBuffer, sizeof(PacketBuffer), 0);
                SrcIndex = 0;
            }

            //Copy a byte from PacketBuffer[SrcIndex] to Buffer[DstIndex]
            c = Buffer[DstIndex++] = PacketBuffer[SrcIndex++];

        }while(DstIndex < Len);
    }

    //Return number of bytes coppied
    return(DstIndex);
}

//Thats all she wrote!!!
//Happy hunting my friends!
