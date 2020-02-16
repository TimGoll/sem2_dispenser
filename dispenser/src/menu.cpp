#include "menu.hpp"

MenuElement::MenuElement(uint8_t* name, uint8_t type, MenuElement* parent, uint8_t max_children_amount = 0) : Callback(1) {
	this->name = name;
	this->type = type;
	this->parent = parent;

	this->text = new uint8_t[20]; // 20 char

	// prepare structure for a list of children
	if (type == MENU_SUBMENU) {
		this->children = new children_t;
		this->children->max_children_amount = max_children_amount;
		this->children->child = new MenuElement*[max_children_amount];
	}

	// add to parent as a child
	if (parent != nullptr)
		parent->addChild(this);
}

MenuElement* MenuElement::getChild(uint8_t* name) {
	for (uint8_t i = 0; i < this->children->children_amount; i++) {
		if (strcmp(this->children->child[i]->name, name) == 0)
			return this->children->child[i];
	}
}

MenuElement* MenuElement::getChildAtIndex(uint8_t index) {
	if (index >= this->children->children_amount)
		return nullptr;

	return this->children->child[index];
}

bool MenuElement::hasChild(uint8_t* name) {
	if (this->getChild(name) == nullptr)
		return false;

	return true;
}

uint8_t MenuElement::getChildAmount() {
	return this->children->children_amount;
}

MenuElement* MenuElement::getParent() {
	return this->parent;
}

bool MenuElement::addChild(MenuElement* child) {
	if (this->children->children_amount == this->children->max_children_amount)
		return false;

	this->children->child[this->children->children_amount] = child;
	this->children->children_amount++;

	return true;
}

uint8_t* MenuElement::getName() {
	return this->name;
}

uint8_t MenuElement::getType() {
	return this->type;
}

uint8_t* MenuElement::getText() {
	return this->text;
}

void MenuElement::setText(uint8_t* text) {
	strcpy(this->text, text);
}

void MenuElement::setName(uint8_t* name) {
	strcpy(this->name, name);
}

bool MenuElement::isType(uint8_t type) {
	return type == this->type;
}

bool MenuElement::isTopLevel() {
	return this->parent == nullptr;
}




MenuHandler::MenuHandler() {
	this->menu_offset = 0;
	this->menu_index = 0;

	MenuElement* menu_none = new MenuElement("HEAD", MENU_SUBMENU, nullptr, 2);

	this->menu_head = menu_none;
	this->menu_open = menu_none;

	// REGISTERING MENU ELEMENTS
	MenuElement* main = new MenuElement("main", MENU_SUBMENU, menu_none, 16);

	// this callback updates all menu names once the menu is opened
	main->registerCallback(this, &MenuHandler::updateContents);

	new MenuElement("refill_a", MENU_ACTION, main, 0);
	new MenuElement("refill_b", MENU_ACTION, main, 0);
	new MenuElement("state_a_amount", MENU_ACTION, main, 0);
	new MenuElement("state_a_interval", MENU_ACTION, main, 0);
	new MenuElement("state_a_time", MENU_ACTION, main, 0);
	new MenuElement("state_b_amount", MENU_ACTION, main, 0);
	new MenuElement("state_b_interval", MENU_ACTION, main, 0);
	new MenuElement("state_b_time", MENU_ACTION, main, 0);
	new MenuElement("reset_data", MENU_ACTION, main, 0);
	new MenuElement("set_a_0", MENU_ACTION, main, 0);
	new MenuElement("set_a_10", MENU_ACTION, main, 0);
	new MenuElement("set_b_0", MENU_ACTION, main, 0);
	new MenuElement("set_b_10", MENU_ACTION, main, 0);
}

MenuElement** MenuHandler::menuOpenPtr() {
	return &this->menu_open;
}

uint8_t* MenuHandler::menuIndexPtr() {
	return &this->menu_index;
}

uint8_t* MenuHandler::menuOffsetPtr() {
	return &this->menu_offset;
}

void MenuHandler::setObjectPointer(PillHandler* pillHandler) {
	this->pillHandler = pillHandler;

	// register refill callbacks
	this->menu_head->getChild("main")->getChild("refill_a")->registerCallback(this->pillHandler, &PillHandler::refillA);
	this->menu_head->getChild("main")->getChild("refill_b")->registerCallback(this->pillHandler, &PillHandler::refillB);
	this->menu_head->getChild("main")->getChild("reset_data")->registerCallback(this->pillHandler, &PillHandler::reset);
	this->menu_head->getChild("main")->getChild("set_a_0")->registerCallback(this->pillHandler, &PillHandler::setA0);
	this->menu_head->getChild("main")->getChild("set_a_10")->registerCallback(this->pillHandler, &PillHandler::setA10);
	this->menu_head->getChild("main")->getChild("set_b_0")->registerCallback(this->pillHandler, &PillHandler::setB0);
	this->menu_head->getChild("main")->getChild("set_b_10")->registerCallback(this->pillHandler, &PillHandler::setB10);
}

// BUTTON CALLBACK FUNCTIONS

static void MenuHandler::updateContents(MenuHandler* self, uint8_t type) {
	MenuElement* main_menu = self->menu_head->getChild("main");

	DateTime now = RTC->now();

	char buffer[21];

	main_menu->getChild("refill_a")->setText("Refill Container A");
	main_menu->getChild("refill_b")->setText("Refill Container B");

	sprintf(buffer, "Amount A: %02d", self->pillHandler->getPillAmount(0));
	main_menu->getChild("state_a_amount")->setText(buffer);

	sprintf(buffer, "Amount B: %02d", self->pillHandler->getPillAmount(1));
	main_menu->getChild("state_b_amount")->setText(buffer);

	DateTime time1(self->pillHandler->getPillRefTime(0));
	sprintf(buffer, "Next A: %s %02d:%02d", DAYS[time1.dayOfTheWeek()], time1.hour(), time1.minute());
	main_menu->getChild("state_a_time")->setText(buffer);

	DateTime time2(self->pillHandler->getPillRefTime(1));
	sprintf(buffer, "Next B: %s %02d:%02d", DAYS[time2.dayOfTheWeek()], time2.hour(), time2.minute());
	main_menu->getChild("state_b_time")->setText(buffer);

	TimeSpan timespan1 = time1 - now;
	sprintf(buffer, "Interval A: %01d:%02d:%02d", timespan1.days(), timespan1.hours(), timespan1.minutes());
	main_menu->getChild("state_a_interval")->setText(buffer);

	TimeSpan timespan2 = time2 - now;
	sprintf(buffer, "Interval B: %01d:%02d:%02d", timespan2.days(), timespan2.hours(), timespan2.minutes());
	main_menu->getChild("state_b_interval")->setText(buffer);

	main_menu->getChild("reset_data")->setText("D - RESET DATA");
	main_menu->getChild("set_a_0")->setText("D - SET A TO 0");
	main_menu->getChild("set_a_10")->setText("D - SET A TO 10");
	main_menu->getChild("set_b_0")->setText("D - SET B TO 0");
	main_menu->getChild("set_b_10")->setText("D - SET B TO 10");
}

static void MenuHandler::buttonNext(MenuHandler* self, uint8_t type) {
	MenuElement* new_open_menu = self->menu_open->getChildAtIndex(self->menu_offset + self->menu_index);

	if (not new_open_menu)
		return;

	new_open_menu->runCallback(0); // pass a dummy value

	if (new_open_menu->isType(MENU_SUBMENU))
		self->menu_open = new_open_menu;
	else
		self->menu_open = self->menu_head;

	self->menu_offset = 0;
	self->menu_index = 0;
}

static void MenuHandler::buttonPrev(MenuHandler* self, uint8_t type) {
	MenuElement* new_open_menu = self->menu_open->getParent();

	if (not new_open_menu)
		return;

	self->menu_open = new_open_menu;
	self->menu_offset = 0;
	self->menu_index = 0;
}

static void MenuHandler::buttonUp(MenuHandler* self, uint8_t type) {
	if (self->menu_offset + self->menu_index == 0)
		return;
	
	if (self->menu_index == 0)
		self->menu_offset--;
	else
		self->menu_index--;
}

static void MenuHandler::buttonDown(MenuHandler* self, uint8_t type) {
	if (self->menu_offset + self->menu_index == self->menu_open->getChildAmount() -1)
		return;

	if (self->menu_index == MENU_DISPLAY_LINES -1)
		self->menu_offset++;
	else
		self->menu_index++;
}
