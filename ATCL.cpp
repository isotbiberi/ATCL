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
#include <sstream>

std::string readTillSemicolon(int fd);
void readAck(int fd);
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


int moveAltAz(std::string altitude,std::string azimuth,int fd)
{
	std::string alt="";
	alt.append(SetTargetAlt,0,5);
	alt.append(altitude);
	sendCommand(alt,fd,15);
	readAck(fd);

	std::string az="";
	az.append(SetTargetAz,0,5);
	az.append(azimuth);
	sendCommand(az,fd,15);
	readAck(fd);

	sendCommand(GoToTargetAltAz,fd,6);
	readAck(fd);
	return 0;

}



int moveRaDec(std::string rightAscention,std::string declination,int fd)
{
	std::string ra="";
	ra.append(SetTargetRA,0,5);
	ra.append(rightAscention);
	sendCommand(ra,fd,15);
	readAck(fd);

	std::string dec="";
	dec.append(SetTargetDec,0,5);
	dec.append(declination);
	sendCommand(dec,fd,15);
	readAck(fd);

	sendCommand(GoToTargetRA_Dec,fd,6);
	readAck(fd);
	return 0;

}



unsigned char readOneChar(int fd)
{
	unsigned char firstChar[1];
	int rdlen = read(fd, firstChar, 1);
		if (rdlen > 0) {
			return firstChar[0];

		}
		else if (rdlen < 0)
		{
		 printf("Error from read: %d: %s\n", rdlen, strerror(errno));
         exit(0);
		}
std::cout<<" Reading one char "<<firstChar[1]<<std::endl;
return firstChar[0];
}

void readAck(int fd)
{
	unsigned char firstChar=readOneChar(fd);
	std::cout<<"first char in readAck is "<<firstChar<<std::endl;
	while(firstChar!=0x8F)
	{
		readTillSemicolon(fd);
		firstChar=readOneChar(fd);

	}
	std::cout<<"Got ATCL_ACK"<<std::endl;

}

std::string readReturnValue(int fd)
{
	unsigned char firstChar=readOneChar(fd);
	while(isSpecial(firstChar))
	{
		readTillSemicolon(fd);
		firstChar=readOneChar(fd);
	}
   std::string returnValue = readTillSemicolon(fd);
   std::stringstream ss;
   ss<<firstChar;
   ss<<returnValue;

   std::cout<<"Return value is " <<ss.str()<<std::endl;
   return ss.str();

}
std::string readTillSemicolon(int fd)
		{

	     unsigned char buf[88];
		 std::string commandReturn="";
		 int rdlen ;
		 do
		            {

					rdlen = read(fd, buf,1);
					if (rdlen > 0) {


					buf[rdlen] = 0;

					}
					else if (rdlen < 0)
					{
						printf("Error from read: %d: %s\n", rdlen, strerror(errno));

					}

		             commandReturn.append(reinterpret_cast<const char*>(buf));
		            // std::cout<<"Reading till semicolon"<<std::endl;
		            }
		            while(buf[rdlen-1]!=';');
		 //std::cout<<"Readed "<<commandReturn<<" in readtillsemicolon"<<std::endl;
		 return commandReturn;

		}

std::string getTrackRate(int fd)
{

	sendCommand(GetTrackRate,fd,6);
	return readReturnValue(fd);

}

std::string getRaRate(int fd)
{

	sendCommand(GetCustomTRateOffsetRA,fd,6);
	return readReturnValue(fd);
}

int setRaRate(int fd,std::string raRate)
{

	    std::string raTrackRate="";
		raTrackRate.append(SetCustomTRateOffsetRA,0,5);
		raTrackRate.append(raRate);//+1.00 ne demek in arc sec per hour ??
		sendCommand(raTrackRate,fd,15);
		readAck(fd);
}



std::string getDecRate(int fd)
{

	sendCommand(GetCustomTRateOffsetDec,fd,6);
	return readReturnValue(fd);


}


int setDecRate(int fd,std::string decRate)
{

	    std::string decTrackRate="";
		decTrackRate.append(SetCustomTRateOffsetDec,0,5);
		decTrackRate.append(decRate);//+1.00 ne demek in arc sec per hour ??
		sendCommand(decTrackRate,fd,15);
		readAck(fd);
}






int setTrackrate(int fd,std::string mode)
{
	std::string trackRate="";
	trackRate.append(SetTrackRate,0,5);
	trackRate.append(mode);
	sendCommand(trackRate,fd,15);
	readAck(fd);
}

std::string getDec(int fd)
{
	sendCommand(GetDec,fd,6);
	return readReturnValue(fd);

}


std::string getRa(int fd)
{

	sendCommand(GetRa,fd,6);
	return readReturnValue(fd);

}

std::string getAlt(int fd)
{

	sendCommand(GetAlt,fd,6);
	return readReturnValue(fd);

}

std::string getAz(int fd)
{
	sendCommand(GetAz,fd,6);
	return readReturnValue(fd);
}

std::string getVelocityX(int fd)
{

	sendCommand(GetVelMaxX ,fd,6);
	return readReturnValue(fd);

}

int setVelocityX(int fd,std::string velocity)
{
	    std::string velocityX="";
		velocityX.append(SetVelMaxX,0,5);
		velocityX.append(velocity);
		sendCommand(velocityX,fd,15);
		readAck(fd);

}



std::string getVelocityY(int fd)
{

	sendCommand(GetVelMaxY ,fd,6);
	return readReturnValue(fd);

}


int setVelocityY(int fd,std::string velocity)
{
	    std::string velocityY="";
		velocityY.append(SetVelMaxY,0,5);
		velocityY.append(velocity);
		sendCommand(velocityY,fd,15);
		readAck(fd);

}



std::string getAccelX(int fd)
{

	sendCommand(GetAccelMaxX  ,fd,6);
	return readReturnValue(fd);

}

std::string setAccelX(int fd,std::string accel)
{
	        std::string accelX="";
			accelX.append(SetAccelMaxX,0,5);
			sendCommand(accelX,fd,15);
			readAck(fd);

}



std::string getAccelY(int fd)
{

	sendCommand(GetAccelMaxY ,fd,6);
	return readReturnValue(fd);

}


std::string setAccelY(int fd,std::string accel)
{
	    std::string accelY="";
		accelY.append(SetAccelMaxY,0,5);
		sendCommand(accelY,fd,15);
		readAck(fd);

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
sendCommand(GetRa,fd,6);
readReturnValue(fd);
sendCommand(GetDec,fd,6);
readReturnValue(fd);
sendCommand(GetAlt,fd,6);
readReturnValue(fd);
sendCommand(GetAz,fd,6);
readReturnValue(fd);
*/










/*

sendCommand(GetTrackRate,fd,6);
readReturnValue(fd);


sendCommand(GetCustomTRateOffsetRA,fd,6);
readReturnValue(fd);

sendCommand(GetCustomTRateOffsetDec,fd,6);
readReturnValue(fd);

*/


getRa(fd);
getDec(fd);
getAlt(fd);
getAz(fd);


getTrackRate(fd);
getRaRate(fd);
getDecRate(fd);

setTrackrate(fd,"Custom;");
setRaRate(fd,"+1.00;");//sidereal plus this value
setDecRate(fd,"+1.00;");//sidereal plus this value


getTrackRate(fd);
getRaRate(fd);
getDecRate(fd);


getAccelX(fd);
getAccelY(fd);

getVelocityX(fd);
getVelocityY(fd);


//setVelocityX(fd,"3.000000deg/sec;");
//setVelocityY(fd,"4.000000deg/sec;");


setAccelX(fd,"1.000000deg/sec0x1E;");



//moveAltAz("+80:00:00;","080:00:00;",fd);
//moveRaDec("12:00:00.0;","+50:00:00;",fd);

/*
std::string progress;
do{
	sendCommand(GetGoToProgressPercent,fd,6);
	progress=readReturnValue(fd);
}
while(progress.compare("100%;")!=0);
*/






close(fd);
}
