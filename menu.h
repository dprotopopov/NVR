#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <regex>
#include <vector>
#include <sstream>
#include <map>    //подключили библиотеку для работы с map

#include "display.h"
#include "i18n.h"
#include "osd.h"

#include <tinyxml.h>
#include <xpath_processor.h>

#include <boost/regex.hpp>

#ifdef BOOST_MSVC
#pragma warning(disable:4512 4244)
#endif

#include <boost/program_options.hpp>

/*** cXmlMenu *******************************************************************************/
class cXmlMenu : public cOsdMenu {
	private:
		const char *xpath = "/Menu";
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
		static const char *GetTitle(const char *Xpath);
		void Set(const char *Xpath);
		void Update();
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