#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <vector>
#include <map>    //подключили библиотеку для работы с map

#include "display.h"
#include "i18n.h"
#include "osd.h"

#include <tinystr.h>
#include <tinyxml.h>
#include <xpath_processor.h>
#include <boost/regex.hpp>

/*** cXmlMenu *******************************************************************************/
// Разработчик dmitru@protopopov.ru
class cXmlMenu : public cOsdMenu {
	private:
		std::string xpath;
		std::vector< std::pair< *std::string, *std::string > > collection;
		std::vector< std::pair< *std::string, *std::string > >::iterator selected;
		cDisplayMenu DisplayMenu;
	public:
		cXmlMenu(const char *Xpath);
		cXmlMenu(eOsdState State);
		~cXmlMenu(void);
		eOsdState ProcessKey(eKeys Key);
		void killSubMenu();
		bool executeItem(const char *Key, const char *Value);
		const char *GetTitle(std::string &Xpath);
		void Set();
		void Update();
		void LoadFile();
};

/*** cMenuMain ***********************************************************************************/
class cMenuMain : public cOsdMenu {
	private:
		cDisplayMenu DisplayMenu;
	public:
		cMenuMain(eOsdState State);
		~cMenuMain(void);
		eOsdState ProcessKey(eKeys Key);
		void killSubMenu();
		void Set();
		void Update();
};

// cMenuMain->cXmlMenu patch
//extern cMenuMain *Menu;
extern cXmlMenu *Menu;
extern TiXmlDocument *doc;

/*** cMenuSchedule *******************************************************************************/
class cMenuSchedule : public cOsdMenu {
	private:
		cDisplayMenu DisplayMenu;
	public:
		cMenuSchedule(void);
		~cMenuSchedule(void);
		eOsdState ProcessKey(eKeys Key);
		void Set();
		void Update();
};

#endif // STANDARD_H_INCLUDED */