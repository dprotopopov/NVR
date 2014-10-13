#include "remote.h"

#ifndef SIMULATE
uint64_t last_io = 0;
#endif

/*** cKEYBDHandler *********************************************************************************/
cKEYBDHandler::cKEYBDHandler() {
}
int cKEYBDHandler::getch() {
	struct termios oldattr, newattr;
	int ch;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );

	return ch;
}
int cKEYBDHandler::kbhit() {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

/*** cIOHandler *********************************************************************************/
cIOHandler::cIOHandler() {
	IO_Value = 0;
	IO_wasChanged = 0;
	#ifdef SIMULATE 
	IO_Mode = 1; // 1=sim
	#else
	IO_Mode = 0; // 0=hardware
	pinMode(IO_DATA, INPUT);
	pinMode(IO_CLOCK, OUTPUT);
	#endif

	if ((sigemptyset(&intmask) == -1) || (sigfillset(&intmask) == -1)){  
		fprintf(stderr, "Failed to initialize the signal mask\n");
	}
}
cIOHandler::~cIOHandler() {

}

uint32_t cIOHandler::IO_readValues() {
	uint32_t val = 0;
	
	usleep(10000);
 	if(IO_Mode == 1) {
		FILE* f = fopen("io.sim", "r+");
		uint32_t zero = 0;
		if(f == NULL) {
			val = 0;
			printf("io error\r\n");
		} else {
			ignore_result(fread(&val, sizeof(val), 1, f));
			fseek(f, 0, SEEK_SET);
			fwrite(&zero, sizeof(zero), 1, f);
			usleep(10000);
			fclose(f); // MUSS GEMACHT WERDEN
			usleep(10000);
		}
	} else {
		#ifndef SIMULATE
		int i;
		// block signals
		sigprocmask(SIG_BLOCK, &intmask, NULL);

		for(i = 0; i < 24; i++) {
			int j, tmp;

			for(j = 0; j < IO_REPEAT; j++)
				digitalWrite(IO_CLOCK, 1);

			for(j = 0; j < IO_REPEAT; j++)
				digitalWrite(IO_CLOCK, 0);

			tmp = 0;
			for(j = 0; j < IO_REPEAT; j++) {
				if(digitalRead(IO_DATA))
					tmp = 1;
			}

			if(tmp)
				val |= 1 << i;
		}
		// unblock signals
		sigprocmask(SIG_UNBLOCK, &intmask, NULL);
		#endif
	}
	return val;
}

void cIOHandler::IO_Get() {
	uint32_t old_io = IO_Value;
	IO_Value = IO_readValues();
	if(IO_Value != old_io) IO_wasChanged = 1;
}

uint8_t cIOHandler::IO_HasChanged() {
	uint8_t ret = IO_wasChanged;
	IO_wasChanged = 0;
	return ret;
}

int8_t cIOHandler::IO_GetRotaryValue() {
	int8_t ret = (IO_Value >> 16) & 0xff;
	IO_Value &= 0x0000ffff;
	return ret;
}

int cIOHandler::IO_GetRotaryWay() {
	int8_t rotary = IO_GetRotaryValue();
	if(rotary < 0) return 7; // Links dreher
	else if(rotary > 0) return 9; // Rechtsdreher
	else return 0;
}
uint32_t cIOHandler::IO_GetPressedButton() {
	int ret = IO_GetRotaryWay();
	if(ret != 0)
		return ret;
	else
		return IO_Value;	
}
/*** cRemote *************************************************************************************/
cRemote Remote;

cRemote::cRemote() {
	last_io = 0;
}
cRemote::~cRemote() {
}
uint64_t cRemote::getTimeMillisecond() {
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64_t ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret; 
}

void cRemote::setObserver(remoteTypes RemoteObserver) {
	remoteTyp = RemoteObserver;
}
void cRemote::putKey(eKeys key) {
	putsKey = true;
	putedValue = key;
}
eKeys cRemote::getKey() { /*key definitionen in remote.conf für evtl. remotecontroll un generell übersichtiger*/
	// Send a User send Key
	if(putsKey){
		putsKey = false;
		return putedValue;
	}

	// Get Keys from Keyboard | IO
	switch (remoteTyp)
	{
		case KEYBD:
			if(kbhit()) {
				int key = getch();
				// printf("%i\n", key);
				switch (key)
				{
					case 48: // 0
						return k0;
						break;
					case 49: // 1
						return k1;
						break;
					case 50: // 2
						return k2;
						break;
					case 51: // 3
						return k3;
						break;
					case 52: // 4
						return k4;
						break;
					case 33: // 1 LONG - Schift 1
						return k1long;
						break;
					case 34: // 2 LONG - Schift 2
						return k2long;
						break;
					case 167: // 3 LONG - Schift 3
						return k3long;
						break;
					case 36: // 4 LONG - Schift 4
						return k4long;
						break;
					case 10: // OK - Enter
						return kOk;
						break;
					case 109: //Home - m
						return kMenu;
						break;
					case 113: //PlayStop - q
						return kPlayStop;
						break;
					case 119: //Left - w
						return kLeft;
						break;
					case 101: //Right - e
						return kRight;
						break;
					default: 
						return kNone;
				}
			 }
			break;
		case IO: 
		default: {
 			#ifdef SIMULATE
			IO_Get();
			#else
			if(getTimeMillisecond() - last_io >= 25) {
				IO_Get();
				last_io = getTimeMillisecond();
			}
 			#endif

			uint32_t key = IO_GetPressedButton();
			switch (key)
			{
				case 8:
					return k1;
					break;
				case 16:
					return k2;
					break;
				case 32:
					return k3;
					break;
				case 64:
					return k4;
					break;
				case 2048:
					return k1long;
					break;
				case 4096:
					return k2long;
					break;
				case 8192:
					return k3long;
					break;
				case 16384:
					return k4long;
					break;
				case 1:
					return kOk;
					break;
				case 2:
					return kMenu;
					break;
				case 4:
					return kPlayStop;
					break;
				case 7:
					return kLeft;
					break;
				case 9:
					return kRight;
					break;
				default: 
					return kNone;
			}
			break;
		}
	}
	
	return kNone;
}