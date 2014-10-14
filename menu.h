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

#include "bower_components/tinyxml_2_6_2/tinystr.h"
#include "bower_components/tinyxml_2_6_2/tinyxml.h"
#include "bower_components/tinyxpath_1_3_1/xpath_processor.h"
#include <boost/regex.hpp>

/*** cXmlMenu *******************************************************************************/
class cXmlMenu : public cOsdMenu {
	private:
		const char *xpath;
		std::vector<std::string> collection;
		std::vector<std::string>::iterator selected;
		cDisplayMenu DisplayMenu;
	public:
		cXmlMenu(const char *Xpath);
		cXmlMenu(eOsdState State);
		~cXmlMenu(void);
		eOsdState ProcessKey(eKeys Key);
		void killSubMenu();
		bool executeItem(const char *Xpath);
		const char *GetTitle(const char *Xpath);
		void Set(const char *Xpath);
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