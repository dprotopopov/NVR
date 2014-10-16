#include "display.h"

/*** cDisplayMenu *********************************************************************************/
cDisplayMenu::cDisplayMenu() {
	title = "";
}
cDisplayMenu::~cDisplayMenu() {

}

// SET
void cDisplayMenu::setTitle(const char *Title) {
	title = Title;
}
void cDisplayMenu::setMenuList(std::vector<std::string> List) {
	list.clear();
	
	for(unsigned int i = 0; i < List.size(); i++) {
		list.push_back(List.at(i));
	}
}

// DRAW
void cDisplayMenu::clear() {
/* 	GLCDD_Clear();
	GLCDD_Draw();
 */
	system("clear");
}
void cDisplayMenu::drawBorder() {
	/* GLCDD_Font* fnt_screen_border;
	
	GLCDD_Rectangle(0, 0, SCREEN_W, SCREEN_H, 0);
	GLCDD_Rectangle(0, 0, SCREEN_W, 7, 1);
 	GLCDD_ClearPixel(0, 0);
	GLCDD_ClearPixel(SCREEN_W - 1, 0);
	GLCDD_ClearPixel(0, SCREEN_H - 1);
	GLCDD_ClearPixel(SCREEN_W - 1, SCREEN_H - 1);

	fnt_screen_border->color = 1;
	printf("x = %i\n", GLCDD_StringWidth(fnt_screen_border, const_cast<char*>(title))); */
/*  	GLCDD_Rect r;
	r.x = SCREEN_W / 2 - GLCDD_StringWidth(fnt_screen_border, const_cast<char*>(title)) / 2;
	r.y = 0;
	r.w = SCREEN_W;
	r.h = -1; */
/* 	GLCDD_Print(fnt_screen_border, &r, const_cast<char*>(title));
	fnt_screen_border->color = 0;

	GLCDD_Draw(); */
/* 	if(isSnooze()) {
		GLCDD_ClearEx(SCREEN_W -, 12, 1, SCREEN_W - 5, 5);
		GLCDD_XBMDraw((uint8_t*)img_snooze, SCREEN_W - 12, 1, 8, 5);
	} */
}
void cDisplayMenu::drawMenu() {
	for(unsigned int i = 0; i < list.size(); i++) { 
		printf("%i - %s \n", i, list[i].c_str());
	}
}
void cDisplayMenu::drawTitle() {
	printf("%s\n", title);
}
void cDisplayMenu::Draw() {
	drawTitle();
	drawBorder();
	drawMenu();
}