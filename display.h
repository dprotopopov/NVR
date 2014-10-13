#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define SCREEN_W 128
#define SCREEN_H 64

/*** cDisplayMenu ********************************************************************************/
class cDisplayMenu {
	private:
		const char* title;
	public:
		std::vector<std::string> list;
	public:
		cDisplayMenu(void);
		~cDisplayMenu(void);
		
		void setTitle(const char *Title);
		void setMenuList(std::vector<std::string> List);
		
		void clear();
		void drawBorder();
		void drawMenu();
		void drawTitle();
		void Draw();
};
#endif