#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "i18n.h"
#include "menu.h"
#include "osd.h"
#include "remote.h"
#include "tools.h"

int main(int argc, char *argv[]) {
	// STANDARD VARS / DEFINES
	const char *LocaleDir = LOCDIR;
	remoteTypes RemoteObserver = KEYBD; // IO | KEYBD

	// User Options
	static struct option long_options[] = {	{ "localedir",   required_argument, NULL, 'l'},
											{ "remotetyp",   required_argument, NULL, 'r'},
											{ NULL } };

	int userOpt;
	while ((userOpt = getopt_long(argc, argv, "l:r:", long_options, NULL)) != -1)
	{
		switch (userOpt)
		{
			case 'l': 
				if (access(optarg, R_OK | X_OK) == 0)
					LocaleDir = optarg;
				else {
					fprintf(stderr, "Can't access locale directory: %s\n", optarg);
					return 2;
				}
				break;
			case 'r': 
					RemoteObserver = (remoteTypes)atoi(optarg);
				break;
			default: 
				return 0;
		}
	}
	
	std::string filepath("menu.xml");
	parser = new xmlpp::DomParser(filepath);
	root = parser->get_document()->get_root_node(); //deleted by DomParser.

	// INIT FUNCTIONS
	setlocale(LC_ALL, "");
	I18nInitialize(LocaleDir);
	Remote.setObserver(RemoteObserver);
	cDisplayMenu DisplayMenu;
	
	//while (!ShutdownHandler.DoExit()) {
	while(1) {
		// Global working keys
		eKeys key = Remote.getKey();
		// if(key > 0)
			// printf("key Pressed = %i \n ", key);
		switch (key) {
 			case kMenu:
						if(Menu == NULL)

						// cMenuMain->cXmlMenu patch
						//	Menu = new cMenuMain(osMenu);
							Menu = new cXmlMenu(osMenu);
						else {
							Menu->killSubMenu();
							delete Menu;
							Menu = NULL;
						}
						break;
			case kPlayStop:
						printf("PLAY STOP Pressed\n");
						break;
			default:	
						if(Menu != NULL)
							Menu->ProcessKey(key);
						break;
		}

		usleep(10000); // 1=100000 Millisekunde
	}

	
	return 0;
}