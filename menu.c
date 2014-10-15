#include "menu.h"

using namespace std;
using namespace Glib;
using namespace boost;
using namespace xmlpp;

#define LOG(x) // do { std::cerr << x; std::cerr << std::endl; } while (0)

/*** cMenuMain ***********************************************************************************/
// cMenuMain->cXmlMenu patch
//cMenuMain *Menu = NULL;
cXmlMenu *Menu = NULL;
DomParser *parser = NULL;
Node* root = NULL;

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
// Разработчик dmitru@protopopov.ru

cXmlMenu::cXmlMenu(const char *Xpath)
:cOsdMenu(tr(GetTitle(Xpath)))
,xpath(Xpath)
{
	subMenu = NULL; // А инициализировать кто будет - Пушкин?
	LOG("BEGIN XmlMenu::cXmlMenu(const char *Xpath)");
	LOG(Xpath);
	Set();
	LOG("END XmlMenu::cXmlMenu(const char *Xpath)");
}
cXmlMenu::cXmlMenu(eOsdState State)
:cOsdMenu(tr(GetTitle("/Menu")))
,xpath("/Menu")
{
	subMenu = NULL; // А инициализировать кто будет - Пушкин?
	LOG("BEGIN cXmlMenu::cXmlMenu(eOsdState State)");
	// open Submenu
	switch(State) {
		case osUnknown:
			break;
		default:
			std::map<eOsdState, Glib::ustring> stateFilter;
			stateFilter[osUnknown]="[@state='osUnknown']";
			stateFilter[osContinue]="[@state='osContinue']";
			stateFilter[osBack]="[@state='osBack']";
			stateFilter[osMenu]="[@state='osMenu']";
			stateFilter[osNowPlaying]="[@state='osNowPlaying']";
			stateFilter[osSetup]="[@state='osSetup']";
			LOG(stateFilter[State].c_str());
			bool submenuOpen = false;
			Glib::ustring current(xpath.c_str());
			LOG(current.c_str());
			// Обработка действий запрограммированных на статусы
			Glib::ustring items(current+"/Item"+stateFilter[State]);
			NodeSet set = root->find(items);
			for(NodeSet::iterator i = set.begin(); i != set.end(); ++i) {
				Glib::ustring key((*i)->get_path());
				Glib::ustring value(total?values[highlighted].c_str():"");
				submenuOpen = executeItem(key.c_str(),value.c_str()) || submenuOpen;
			}
			if(!submenuOpen){
				Set();
			}
			break;
	}
	LOG("END cXmlMenu::cXmlMenu(eOsdState State)");
}
cXmlMenu::~cXmlMenu() {
	LOG("BEGIN XmlMenu::~cXmlMenu()");
	DisplayMenu.clear();
	LOG("END XmlMenu::~cXmlMenu()");
}
eOsdState cXmlMenu::ProcessKey(eKeys Key) {
	// LOG("BEGIN cXmlMenu::ProcessKey(eKeys Key)");
 	if(subMenu != NULL) {
		return subMenu->ProcessKey(Key);
	}
	
	switch(Key) {
		case kNone:
			break;
		default:
			std::map<eKeys, Glib::ustring> keyFilter;
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
			LOG(keyFilter[Key].c_str());
			Glib::ustring current(xpath.c_str());
			// Обработка действий запрограммированных на события
			Glib::ustring items(current+"/Item"+keyFilter[Key]);
			NodeSet set = root->find(items);
			for(NodeSet::iterator i = set.begin(); i != set.end(); ++i) {
				Glib::ustring key((*i)->get_path());
				Glib::ustring value(total?values[highlighted].c_str():"");
				executeItem(key.c_str(),value.c_str());
			}
			break;
	}
	{
		// Обработка событий согласно алгоритму
		switch (Key) {
			case kLeft:
			case kRight:
						if(total) {
							// Обработка перемещения по отображаемому меню
							switch (Key) {
								case kLeft:
									highlighted = (highlighted + total - 1) % total;
									break;
							case kRight:
									highlighted = (highlighted + total + 1) % total;
									break;
							}
						}
						break;					
			case kOk:
						if(total) {
							LOG("Обработка выбранного отображаемого пункта меню");
							LOG(keys[highlighted]);
							LOG(values[highlighted]);
							executeItem(keys[highlighted].c_str(),values[highlighted].c_str());
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
	// LOG("END cXmlMenu::ProcessKey(eKeys Key)");
	return osUnknown;
}
// Обработка пункта меню
// Возвращает true если было добавлено подменю
bool cXmlMenu::executeItem(const char *Key, const char *Value) {
	LOG("BEGIN cXmlMenu::executeItem(const char *Key, const char *Value)");
	bool submenuOpen = false;
	LOG(Key);
	LOG(Value);
	Glib::ustring item(Key);
	Glib::ustring value(Value);
	Glib::ustring attrType(item + "/@type");
	Glib::ustring attrRegex(item + "/@regex");
	Glib::ustring attrTitle(item + "/@title");
	Glib::ustring attrText(item + "/text()");
	Glib::ustring type(root->eval_to_string(attrType));
	Glib::ustring pattern(root->eval_to_string(attrRegex));
	Glib::ustring header(root->eval_to_string(attrTitle));
	Glib::ustring text(root->eval_to_string(attrText));
	// http://stackoverflow.com/questions/2931704/how-to-compare-string-with-const-char
	if(std::strcmp(type.c_str(),"menu")==0) {
		AddSubMenu(new cXmlMenu(item.c_str()));
		submenuOpen = true;
	}
	else if(std::strcmp(type.c_str(),"command")==0) {
		if(value.empty() == false && pattern.empty() == false) {
			// https://github.com/dprotopopov/Regex.MatchReplace
			boost::regex expression(string(pattern.c_str()),boost::regex::ECMAScript|boost::regex::icase);
			std::string file(value.c_str());
			std::string::const_iterator start = file.begin(); 
			std::string::const_iterator end = file.end(); 
			boost::match_results<std::string::const_iterator> what; 
			boost::match_flag_type flags = boost::match_default; 
			while(regex_search(start, end, what, expression, flags)) 
			{ 
				// what[0] contains the whole string 
				string command(boost::regex_replace(string(what[0].first, what[0].second),expression,string(text.c_str())));
				LOG(command.c_str());
				// http://stackoverflow.com/questions/8832326/how-can-i-execute-a-command-line-command-from-a-c-program
				system(command.c_str());
				// update search position:
				start = what[0].second;      
				// update flags:
				flags |= boost::match_prev_avail;
				flags |= boost::match_not_bob;
			}
		}
		else {
			// http://stackoverflow.com/questions/8832326/how-can-i-execute-a-command-line-command-from-a-c-program
			system(text.c_str());
		}
	}
	else if(std::strcmp(type.c_str(),"add")==0) {
		add(text.c_str());
		DisplayMenu.clear();
		DisplayMenu.setTitle(title);
		DisplayMenu.setMenuList(show());
		DisplayMenu.Draw();
	}
	else if(std::strcmp(type.c_str(),"show")==0) {
		DisplayMenu.clear();
		DisplayMenu.setTitle(title);
		DisplayMenu.setMenuList(show());
		DisplayMenu.Draw();
	}
	else if(std::strcmp(type.c_str(),"clear")==0) {
		DisplayMenu.clear();
	}
	else if(std::strcmp(type.c_str(),"goto")==0) {
		xpath.assign("/Menu//Item[@id='"+text+"'][1]");
		show().clear();
		keys.clear();
		values.clear();
		Set();
	}
	{
		// Вывод отладочной информации
		Glib::ustring attr(item + "/@trace");
		Glib::ustring msg(root->eval_to_string(attr));
		if(!msg.empty()) printf("%s\n",msg.c_str());
	}
	LOG(item.c_str());
	LOG(value.c_str());
	LOG(attrType.c_str());
	LOG(attrRegex.c_str());
	LOG(attrText.c_str());
	LOG(type.c_str());
	LOG(pattern.c_str());
	LOG(header.c_str());
	LOG(text.c_str());
	LOG("END cXmlMenu::executeItem(const char *Key, const char *Value)");
	return submenuOpen;
}
void cXmlMenu::killSubMenu() {
	LOG("BEGIN cXmlMenu::killSubMenu()");
	if(subMenu != NULL)
		CloseSubMenu();
	LOG("END cXmlMenu::killSubMenu()");
}
void cXmlMenu::Set() {
	LOG("BEGIN cXmlMenu::Set()");
	Glib::ustring current(xpath.c_str());
	Glib::ustring attrTitle(current + "/@title");
	Glib::ustring attrText(current + "/text()");
	Glib::ustring header(root->eval_to_string(attrTitle));
	Glib::ustring text(root->eval_to_string(attrText));
	Glib::ustring items(current+"/Item[@type='list']");
	total = 0;
	NodeSet set = root->find(items);
	unsigned count = 0;
    for(NodeSet::iterator i = set.begin(); i != set.end(); ++i)
    {
		count++;
		Glib::ustring item((*i)->get_path());
		Glib::ustring text(item + "/text()");
		Glib::ustring command(root->eval_to_string(text));
		// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
		FILE* pipe = popen(command.c_str(), "r");
		char buffer[1024];
		while(!feof(pipe)) {
			if(fgets(buffer, sizeof(buffer), pipe) != NULL){
				add(tr(buffer));
				string first(item.c_str());
				string second(buffer);
				keys.push_back(first);
				values.push_back(second);
				total++;
				LOG(item.c_str());
				LOG(buffer);
			}
		}
		pclose(pipe);
	}
	// Glib::ustring items1(current+"/Item[not(@type='list') and not(@hidden='true')]");
	Glib::ustring items1(current+"/Item[not(@type='list')][not(@hidden='true')]");
	NodeSet set1 = root->find(items1);
	unsigned count1 = 0;
    for(NodeSet::iterator i1 = set1.begin(); i1 != set1.end(); ++i1)
    {
		count1++;
		Glib::ustring item((*i1)->get_path());
		Glib::ustring attr(item + "/@title");
		Glib::ustring header(root->eval_to_string(attr));
		add(tr(header.c_str()));
		LOG(item.c_str());
		LOG(header.c_str());
	}
	// Glib::ustring items2(current+"/Item[not(@type='list') and not(@browsable='false')]");
	Glib::ustring items2(current+"/Item[not(@type='list')][not(@browsable='false')]");
	NodeSet set2 = root->find(items2);
	unsigned count2 = 0;
    for(NodeSet::iterator i2 = set2.begin(); i2 != set2.end(); ++i2)
    {
		count2++;
		Glib::ustring item((*i2)->get_path());
		string first(item.c_str());
		string second("");
		keys.push_back(first);
		values.push_back(second);
		total++;
		LOG(item.c_str());
		LOG(data.c_str());
	}
	highlighted = 0;
	assert(total >= count2);
	assert(keys.size() == total);
	assert(values.size() == total);
	LOG(total);
	
	DisplayMenu.clear();
	DisplayMenu.setTitle(title);
	DisplayMenu.setMenuList(show());
	DisplayMenu.Draw();
	LOG(current.c_str());
	LOG(items.c_str());
	LOG(items1.c_str());
	LOG(items2.c_str());
	LOG(value.c_str());
	LOG(header.c_str());
	LOG("END cXmlMenu::Set()");
}
// Получение заголовка меню
const char * cXmlMenu::GetTitle(const char *Xpath) {
	LOG("BEGIN cXmlMenu::GetTitle(const char *Xpath)");
	Glib::ustring item(Xpath);
	Glib::ustring attr(item + "/@title");
	Glib::ustring header(root->eval_to_string(attr));
	static char buffer[1024];
	strcpy(buffer, header.c_str());
	LOG(Xpath);
	LOG(buffer);
	LOG("END cXmlMenu::GetTitle(const char *Xpath)");
	return buffer;
}
void cXmlMenu::Update() {
	LOG("BEGIN cXmlMenu::Update()");
	printf("update");
	LOG("END cXmlMenu::Update()");
}
