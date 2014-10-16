#include "osd.h"

/*** cOsdMenuList ********************************************************************************/
cOsdMenuList::cOsdMenuList() {
	std::vector<std::string> menuList;
}
cOsdMenuList::~cOsdMenuList() {
}
void cOsdMenuList::add(std::string name) {
	menuList.push_back(name);
}
void cOsdMenuList::del() {

}
std::vector<std::string> cOsdMenuList::show() {
	return menuList;
}

/*** cOsdMenu ************************************************************************************/
cOsdMenu *OsdMenu = NULL;
int cOsdMenu::instanzNr = 0;
cOsdMenu::cOsdMenu(const char *Title) {
	instanzNr++;
	title = Title;
}
cOsdMenu::~cOsdMenu() {
	instanzNr--;
}
eOsdState cOsdMenu::AddSubMenu(cOsdMenu *SubMenu) {
 	if(subMenu && subMenu != NULL) {
		delete subMenu;
		subMenu = NULL;
	}
	subMenu = SubMenu;
	return osContinue;
}

eOsdState cOsdMenu::CloseSubMenu() {
	delete subMenu;
	subMenu = NULL;

	return osContinue;
}

eOsdState cOsdMenu::ProcessKey(eKeys Key) {
	return osUnknown;
}