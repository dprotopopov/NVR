#ifndef _OSD_H_
#define _OSD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "remote.h"

enum eOsdState { 	osUnknown,
					osContinue,
					osBack,
					osMenu,			// MAIN MENU
					osSchedule,		// HOME
					osNowPlaying,
					osSetup
									};
									
/*** cOsdMenuList ********************************************************************************/
class cOsdMenuList
{
	private:
		std::vector<std::string> menuList;
	public: 
		cOsdMenuList(); 
		~cOsdMenuList(); 
		void add(std::string name); 
		void del();
		std::vector<std::string> show(); 
};

/*** cOsdMenu ************************************************************************************/
class cOsdMenu : public cOsdMenuList{
	private:
		static int instanzNr;
	public:
		cOsdMenu *subMenu;
		const char *title;
	protected:
		eOsdState AddSubMenu(cOsdMenu *SubMenu);
		eOsdState CloseSubMenu();
	public:
		cOsdMenu(const char *Title);
		~cOsdMenu(void);
		virtual eOsdState ProcessKey(eKeys Key);
};

extern cOsdMenu *OsdMenu;
#endif