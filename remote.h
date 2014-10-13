///////////////////////////////////////////////////////////
// IO Library (using IO Bridge)
// 20.08.2012 - Michael Schwarz
// 29.04.2013 - Alexander Lein
///////////////////////////////////////////////////////////

#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>

#include "tools.h"

#ifndef SIMULATE
 #include <wiringPi.h>
#endif

#define IO_REPEAT 160	// Speed (the higher the value, the slower the speed)
#define IO_CLOCK  23	// GPIO Pin
#define IO_DATA   24	// GPIO Pin

enum remoteTypes {  	IO = 0,
						KEYBD = 1
									};
enum eKeys		 {		kNone,
						kOk,
						kMenu,
						kPlayStop,
						kLeft,
						kRight,
						k0,
						k1,
						k2,
						k3,
						k4,
						k1long,
						k2long,
						k3long,
						k4long
									};

/*** cKEYBDHandler *******************************************************************************/
class cKEYBDHandler {
	public:
		cKEYBDHandler(void);
		int getch();
		int kbhit();
};

/*** cIOHandler **********************************************************************************/
class cIOHandler {
	private:
		uint8_t IO_Mode;
		uint32_t IO_Value;
		uint8_t IO_wasChanged;
		sigset_t intmask;  
	public:
		cIOHandler(void);
		~cIOHandler(void);
		uint32_t IO_readValues();
		void IO_Get();
		uint8_t IO_HasChanged();
		int8_t IO_GetRotaryValue();
		int IO_GetRotaryWay();
		uint32_t IO_GetPressedButton();
};

/*** cRemote *************************************************************************************/
class cRemote : public cIOHandler, public cKEYBDHandler {
	private:
		uint64_t last_io;
		remoteTypes remoteTyp;
		bool putsKey;
		eKeys putedValue;
	private:
		uint64_t getTimeMillisecond();
	public:
		cRemote();
		~cRemote(void);
		void setObserver(remoteTypes RemoteObserver);
		void putKey(eKeys key);
		eKeys getKey();
};
extern cRemote Remote;
#endif