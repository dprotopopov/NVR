#include "menu.h"
using namespace std;
using namespace TinyXPath;

// Функция itoa — широко распространённое нестандартное расширение стандартного языка программирования Си. 
// Ее использование не предусматривает переносимости, поскольку эта функция не определена ни в одном стандарте языка Си; 
// тем не менее, зачастую компиляторы поддерживают ее за счет использования заголовка <stdlib.h>, причем не совсем в удобном виде, 
// так как она весьма близка по смыслу к стандартной библиотечной функции atoi.
template <typename T> string toStr(T tmp)
{
    ostringstream out;
    out << tmp;
    return out.str();
}
const char *c_str(string s) {
	return s.c_str();
}

/*** cMenuMain ***********************************************************************************/
// cMenuMain->cXmlMenu patch
//cMenuMain *Menu = NULL;
cXmlMenu *Menu = NULL;
TiXmlDocument *doc = NULL;

cMenuMain::cMenuMain(eOsdState State)
:cOsdMenu("Hauptmenu")
{
	bool submenuOpen = true;
	// open Submenu
	switch (State) {
		case osSchedule:
							AddSubMenu(new cMenuSchedule);
							break;
		default: 			
							submenuOpen = false;
							break;
	}

	if(!submenuOpen){
		Set();
	}
}
cMenuMain::~cMenuMain() {
	DisplayMenu.clear();
}
void cMenuMain::killSubMenu() {
	if(subMenu != NULL)
		CloseSubMenu();
}
eOsdState cMenuMain::ProcessKey(eKeys Key) {
 	if(subMenu != NULL) {
		return subMenu->ProcessKey(Key);
	}

	switch (Key) {
		case k0:
					add("add menu 0");
					printf("0 klicked\n");
					break;
		case k1:
					AddSubMenu(new cMenuSchedule);
					break;
		default:
					break;
	}
	return osUnknown;
}
void cMenuMain::Set() {
	// Add Menupoints
	add(tr("Summary"));
	add(tr("Channels"));
	add(tr("Favorites"));
	add(tr("MediaPlayer"));
	add(tr("Settings"));

	DisplayMenu.clear();
	DisplayMenu.setTitle(title);
	DisplayMenu.setMenuList(show());
	DisplayMenu.Draw();
}
void cMenuMain::Update() {
}
/*** cMenuSchedule *******************************************************************************/
cMenuSchedule::cMenuSchedule()
:cOsdMenu(tr("Schedule"))
{
	Set();
}
eOsdState cMenuSchedule::ProcessKey(eKeys Key) {
/*  	if(subMenu != NULL) {
		return subMenu->ProcessKey(Key);
	} */
	switch (Key) {
		case k1:
					add("add menu 1");
					printf("1 klicked\n");
					break;
		case k2:
					add("add menu 2");
					printf("2 klicked\n");
					break;
		case k3:
					show();
					printf("3 klicked\n");
					break;
		default:
					break;
	}
	return osUnknown;
}
void cMenuSchedule::Set() {
	add("Schedule 1");
	add("Schedule 2");
	add("Schedule 3");
	add("Schedule 4");

	DisplayMenu.clear();
	DisplayMenu.setTitle(title);
	DisplayMenu.setMenuList(show());
	DisplayMenu.Draw();
}
void cMenuSchedule::Update() {
	printf("update");
}
/*** cXmlMenu *******************************************************************************/
cXmlMenu::cXmlMenu(const char *Xpath)
:cOsdMenu(tr(GetTitle(Xpath)))
{
	if(doc == NULL) {
		// The simplest way to load a file into a TinyXML DOM is:
		doc = new TiXmlDocument( "menu.xml" );
		doc -> LoadFile();
	}
	Set(Xpath);
	xpath = Xpath;
}
cXmlMenu::cXmlMenu(eOsdState State)
:cOsdMenu(tr(GetTitle(xpath)))
{
	std::map<eOsdState, TiXmlString> stateFilter;
	stateFilter[osUnknown]="[@state='osUnknown']";
	stateFilter[osContinue]="[@state='osContinue']";
	stateFilter[osBack]="[@state='osBack']";
	stateFilter[osMenu]="[@state='osMenu']";
	stateFilter[osNowPlaying]="[@state='osNowPlaying']";
	stateFilter[osSetup]="[@state='osSetup']";
	if(doc == NULL) {
		// The simplest way to load a file into a TinyXML DOM is:
		doc = new TiXmlDocument( "menu.xml" );
		doc -> LoadFile();
	}
	bool submenuOpen = false;
	TiXmlString current(xpath);
	// open Submenu
	{
		// Обработка действий запрограммированных на статусы
		TiXmlString items(current+"/Item"+stateFilter[State]);
		xpath_processor xproc(doc -> RootElement(),items.c_str());
		unsigned count = xproc.u_compute_xpath_node_set ();
		for(unsigned i=0; i<count; i++) {
			TiXmlString item("(" + current + "/Item" + stateFilter[State] + ")[" + c_str(toStr(i)) + "]");
			submenuOpen = executeItem(item.c_str()) || submenuOpen;
		}
	}
	if(!submenuOpen){
		Set(xpath);
	}
}
cXmlMenu::~cXmlMenu() {
	DisplayMenu.clear();
}
eOsdState cXmlMenu::ProcessKey(eKeys Key) {
	std::map<eKeys, TiXmlString> keyFilter;
	keyFilter[kNone]="[@key='kNone']";
	keyFilter[kOk]="[@key='kOk']";
	keyFilter[kMenu]="[@key='kMenu']";
	keyFilter[kPlayStop]="[@key='kPlayStop']";
	keyFilter[kLeft]="[@key='kLeft']";
	keyFilter[kRight]="[@key='kRight']";
	keyFilter[k0]="[@key='k0']";
	keyFilter[k1]="[@key='k1']";
	keyFilter[k2]="[@key='k2']";
	keyFilter[k3]="[@key='k3']";
	keyFilter[k4]="[@key='k4']";
	keyFilter[k1long]="[@key='k1long']";
	keyFilter[k2long]="[@key='k2long']";
	keyFilter[k3long]="[@key='k3long']";
	keyFilter[k4long]="[@key='k4long']";
	TiXmlString current(xpath);
	{
		// Обработка действий запрограммированных на события
		TiXmlString items(current+"/Item"+keyFilter[Key]);
		xpath_processor xproc(doc -> RootElement(),items.c_str());
		unsigned count = xproc.u_compute_xpath_node_set ();
		for(unsigned i=0; i<count; i++) {
			TiXmlString item("(" + current + "/Item" + keyFilter[Key] + ")[" + c_str(toStr(i)) + "]");
			executeItem(item.c_str());
		}
	}
	{
		// Обработка событий согласно алгоритму
		switch (Key) {
			case kLeft:
			case kRight:
						{
							// Обработка перемещения по отображаемому меню
							switch (Key) {
								case kLeft:
									if(selected != collection.begin()) selected--;
									else selected = collection.end();
									break;
							case kRight:
									if(selected != collection.end()) selected++;
									else selected = collection.begin();
									break;
							}
						}
						break;					
			case kOk:
						{
							// Обработка выбранного отображаемого пункта меню
							executeItem(selected -> c_str());
						}
						break;
			case kMenu:
						// Закрытие меню
						return osContinue;
			case kNone:
						// Возврат на верхний уровень
						return osBack;
			default:
						break;
		}
	}
	return osUnknown;
}
// Обработка пункта меню
// Возвращает true если было добавлено подменю
bool cXmlMenu::executeItem(const char *Xpath) {
	bool submenuOpen = false;
	TiXmlString item(Xpath);
	TiXmlString attr(item + "/@type");
	xpath_processor xproc(doc -> RootElement(),attr.c_str());
	TiXmlString type(xproc.S_compute_xpath());
	// http://stackoverflow.com/questions/2931704/how-to-compare-string-with-const-char
	if(std::strcmp(type.c_str(),"menu")==0) {
		AddSubMenu(new cXmlMenu(item.c_str()));
		submenuOpen = true;
	}
	else if(std::strcmp(type.c_str(),"command")==0) {
		TiXmlString attr(item + "/@regex");
		xpath_processor xproc(doc -> RootElement(),item.c_str());
		xpath_processor xproc1(doc -> RootElement(),attr.c_str());
		TiXmlString replacement(xproc.S_compute_xpath());
		TiXmlString pattern(xproc1.S_compute_xpath());
		boost::regex rx(pattern.c_str(),boost::regex::ECMAScript|boost::regex::icase);
		string command(boost::regex_replace(selected -> c_str(),rx,replacement.c_str()));
		// http://stackoverflow.com/questions/8832326/how-can-i-execute-a-command-line-command-from-a-c-program
		system(command.c_str());
	}
	else if(std::strcmp(type.c_str(),"add")==0) {
		xpath_processor xproc(doc -> RootElement(),item.c_str());
		TiXmlString title(xproc.S_compute_xpath());
		add(tr(title.c_str()));
	}
	else if(std::strcmp(type.c_str(),"show")==0) {
		show();
	}
	else if(std::strcmp(type.c_str(),"goto")==0) {
		xpath_processor xproc(doc -> RootElement(),item.c_str());
		TiXmlString id(xproc.S_compute_xpath());
		TiXmlString next("(/Menu//[@id='"+id+"'])[0]");
		AddSubMenu(new cXmlMenu(next.c_str()));
		submenuOpen = true;
	}
	{
		// Вывод отладочной информации
		TiXmlString attr(item + "/@trace");
		xpath_processor trace(doc -> RootElement(),attr.c_str());
		TiXmlString msg(trace.S_compute_xpath());
		if(!msg.empty()) printf("%s\n",msg.c_str());
	}
	return submenuOpen;
}
void cXmlMenu::killSubMenu() {
	if(subMenu != NULL)
		CloseSubMenu();
}
void cXmlMenu::Set(const char *Xpath) {
	TiXmlString current(Xpath);
	TiXmlString items(current+"/Item[not(@hidden) or @hidden='false']");
	xpath_processor xproc(doc -> RootElement(),items.c_str());
	unsigned count = xproc.u_compute_xpath_node_set ();
	for(unsigned i=0; i<count; i++) {
		TiXmlString item("(" + current + "/Item[not(@hidden) or @hidden='false'])[" + c_str(toStr(i)) + "]");
		TiXmlString attr(item + "/@type");
		xpath_processor xproc(doc -> RootElement(),attr.c_str());
		TiXmlString type(xproc.S_compute_xpath());
		if(std::strcmp(type.c_str(),"list")==0) {
			xpath_processor xproc(doc -> RootElement(),item.c_str());
			TiXmlString command(xproc.S_compute_xpath());
			// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
			FILE* pipe = popen(command.c_str(), "r");
			char buffer[128];
			while(!feof(pipe)) {
				if(fgets(buffer, sizeof(buffer), pipe) != NULL)
					add(tr(buffer));
			}
			pclose(pipe);
		}
		else {
			TiXmlString attr(item + "/@title");
			xpath_processor xproc(doc -> RootElement(),attr.c_str());
			TiXmlString title(xproc.S_compute_xpath());
			add(tr(title.c_str()));
		}
	}
	TiXmlString items1(current+"/Item[not(@browsable='false') or @browsable]");
	xpath_processor xproc1(doc -> RootElement(),items1.c_str());
	unsigned count1 = xproc1.u_compute_xpath_node_set ();
	for(unsigned i=0; i<count1; i++) {
		TiXmlString item("(" + current + "/Item[not(@hidden) or @hidden='false'])[" + c_str(toStr(i)) + "]");
		TiXmlString attr(item + "/@type");
		xpath_processor xproc(doc -> RootElement(),attr.c_str());
		TiXmlString type(xproc.S_compute_xpath());
		if(std::strcmp(type.c_str(),"list")==0) {
			xpath_processor xproc(doc -> RootElement(),item.c_str());
			TiXmlString command(xproc.S_compute_xpath());
			// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
			FILE* pipe = popen(command.c_str(), "r");
			char buffer[128];
			while(!feof(pipe)) {
				if(fgets(buffer, sizeof(buffer), pipe) != NULL)
					collection.push_back(buffer);
			}
			pclose(pipe);
		}
		else {
			TiXmlString attr(item + "/@title");
			xpath_processor xproc(doc -> RootElement(),attr.c_str());
			TiXmlString title(xproc.S_compute_xpath());
			collection.push_back(title.c_str());
		}
	}
	selected = collection.begin();
	DisplayMenu.clear();
	DisplayMenu.setTitle(title);
	DisplayMenu.setMenuList(show());
	DisplayMenu.Draw();
}
// Получение заголовка меню
static const char * cXmlMenu::GetTitle(const char *Xpath) {
	TiXmlString item(Xpath);
	TiXmlString attr(item + "/@title");
	xpath_processor xproc(doc -> RootElement(),attr.c_str());
	static TiXmlString title;
	title = TiXmlString(xproc.S_compute_xpath());
	return title.c_str();
}
void cXmlMenu::Update() {
	printf("update");
}
