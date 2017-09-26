//============================================================================
// Name        : ATCL.cpp
// Author      : ismail baslar
// Version     :
// Copyright   : Your copyright notice
// Description :ATCL PROTOCOL Document 05/05/2017
//============================================================================
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "ATCLCommands.h"
#include <bitset>


#define thereIsReturn 1
#define thereIsNoReturn 0

int getReturnAsync(int fd);
int sendCommand(std::string command,int fd,int length)
{
  int wlen;
  std::cout<<"sending command "<<command<<std::endl;
  wlen = write(fd, reinterpret_cast<const void*>(command.c_str()) , length);
	            if (wlen != length) {
	                printf("Error from write: %d, %d\n", wlen, errno);
	            }
	            tcdrain(fd);    /* delay for output */
	            return 0;
}
bool isSpecial(unsigned char c)
{
	 std::bitset<8> IntBits=std::bitset<8>(c);

	 bool isSpecial = IntBits.test(7);
	 return isSpecial;

}
std::string getReturnSync(int fd,bool thereReturn)
{
	 unsigned char buf[88];
	 std::string commandReturn="";
	 int rdlen ;
	 bool isAsync = false;
	 if(thereReturn==true)
	 {
	            do
	            {

	                    rdlen = read(fd, buf, sizeof(buf) - 1);
	                    if (rdlen > 0) {
                        if(isSpecial(buf[0]))
                        {
                        	std::cout<<"special character returned "<<std::hex<<(int)buf[0]<<std::endl;

                        	getReturnAsync(fd);
                        	isAsync = true;
                            buf[0]=0;

                        }

	            //#ifdef DISPLAY_STRING
	                        buf[rdlen] = 0;
	                      //  printf("Read %d: \"%s\"\n", rdlen, buf);

	            //#else /* display hex */
	           /*
	                       unsigned char *p;
	                        printf("Read %d:", rdlen);
	                        for (p = buf; rdlen-- > 0; p++)
	                            printf(" 0x%x", *p);
	                        printf("\n");
	           */
	           //#endif
	                    } else if (rdlen < 0) {
	                        printf("Error from read: %d: %s\n", rdlen, strerror(errno));

	                    }
	             // printf("character is %c rdlen is %d\n",buf[rdlen-1],rdlen);
	             //std::cout<<"appending "<<buf<<std::endl;
	             commandReturn.append(reinterpret_cast<const char*>(buf));
	            }
	            while(buf[rdlen-1]!=';'&& !isAsync);
	            std::cout<<"First char is "<<std::hex<<(int)commandReturn.at(0)<<std::endl;
	           std::cout<<"Sync return value is " <<commandReturn<<std::endl;
	           return commandReturn;
	 }
	 else
	 {
		 do
		 {
		 rdlen = read(fd, buf, 1);
		 std::cout<<"Waiting for the ATCL_ACK"<<std::endl;
		 }
		 while(buf[0]!=0x8F);
		 std::cout<<"Got ATCL_ACK"<<std::endl;
		 commandReturn.append(reinterpret_cast<const char*>(buf));
		 return commandReturn;
	 }

}
int getReturnAsync(int fd)
{
	unsigned char buf[88];
		 std::string commandReturn;
		 int rdlen ;
		            do
		            {

		                    rdlen = read(fd, buf, sizeof(buf) - 1);
		                    if (rdlen > 0) {
		            //#ifdef DISPLAY_STRING
		                        buf[rdlen] = 0;
		                  //      printf("Read %d: \"%s\"\n", rdlen, buf);

		            //#else /* display hex */
		           /*
		                       unsigned char *p;
		                        printf("Read %d:", rdlen);
		                        for (p = buf; rdlen-- > 0; p++)
		                            printf(" 0x%x", *p);
		                        printf("\n");
		           */
		           //#endif
		                    } else if (rdlen < 0) {
		                        printf("Error from read: %d: %s\n", rdlen, strerror(errno));
		                    }
		            //  printf("character is %c rdlen is %d\n",buf[rdlen-1],rdlen);
		             commandReturn.append(reinterpret_cast<const char*>(buf));
		            }
		            while(buf[rdlen-1]!=';');

		           std::cout<<"Async message is " <<commandReturn<<std::endl;
		         //  getReturnSync(fd);
		           return 0;

}

int startATCL(int fd)
{
	  /* simple output */
	    unsigned char ch = '\xB1';//ATCL mode
	   int wlen = write(fd, &ch , 1);
	    if (wlen != 1) {
	        printf("Error from write: %d, %d\n", wlen, errno);
	    }
	    tcdrain(fd);    /* delay for output */


	    /* simple noncanonical input */

	        unsigned char buf[80];
	        int rdlen;

	        rdlen = read(fd, buf, sizeof(buf) - 1);
	        if (rdlen > 0) {

	//#ifdef DISPLAY_STRING
	            buf[rdlen] = 0;
	            printf("Read %d: \"%s\"\n", rdlen, buf);
	//#else /* display hex */
	            unsigned char *p;
	            printf("Read %d:", rdlen);
	            for (p = buf; rdlen-- > 0; p++)
	            printf(" 0x%x", *p);
	            printf("\n");
	//#endif
	        } else if (rdlen < 0) {
	            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
	        }
            char re = '\x8F';
            const char *er = &re;
	        if(strcmp(reinterpret_cast<const char*>(buf),er))
	        {
	        	printf("started ATCL\n"); return 0;
	        }
	        else {
	        	printf("ATCL protocol cant be started");
	        	return -1;
	        }
	        //return 0;


}




int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}


int main()
{
    const char *portname = "/dev/ttyUSB0";
    int fd;
   // int wlen;


    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 19200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B19200);
    //set_mincount(fd, 0);                /* set to pure timed read */


startATCL(fd);
/*
sendCommand(GetAlignmentState,fd);
sendCommand(AlignFromLastPosition,fd);
sendCommand(GetAlignmaentState,fd);

*/
sendCommand(GetRa,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetDec,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetAlt,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetAz,fd,6);
getReturnSync(fd,thereIsReturn);

std::string alt="";
alt.append(SetTargetAlt,0,5);
alt.append("+80:00:00;");
sendCommand(alt,fd,15);
getReturnSync(fd,0);

std::string az="";
az.append(SetTargetAz,0,5);
az.append("080:00:00;");
sendCommand(az,fd,15);
getReturnSync(fd,thereIsNoReturn);

sendCommand(GoToTargetAltAz,fd,6);
getReturnSync(fd,thereIsNoReturn);

std::string progress;
do{
	sendCommand(GetGoToProgressPercent,fd,6);
	progress=getReturnSync(fd,thereIsReturn);
	std::cout<<"slewing and progress is "<<progress<<std::endl;
	sleep(0.5);
  }
while(progress.compare("100%;")!=0);


sendCommand(GetRa,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetDec,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetAlt,fd,6);
getReturnSync(fd,thereIsReturn);
sendCommand(GetAz,fd,6);
getReturnSync(fd,thereIsReturn);

close(fd);
}
