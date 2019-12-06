#include "menu.hpp"

MenuElement::MenuElement(uint8_t* name, uint8_t type, MenuElement* parent, uint8_t max_children_amount = 0) {
	this->name = name;
	this->type = type;
	this->parent = parent;

	// prepare structure for a list of children
	if (type == MENU_SUBMENU) {
		this->children = new children_t;
		this->children->max_children_amount = max_children_amount;
		this->children->child = new MenuElement*[max_children_amount];
	}

	// add to parent as a child
	if (parent != NULL)
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
		return NULL;

	return this->children->child[index];
}

bool MenuElement::hasChild(uint8_t* name) {
	if (this->getChild(name) == NULL)
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

bool MenuElement::isType(uint8_t type) {
	return type == this->type;
}

MenuHandler::MenuHandler() {
	this->menu_offset = 0;
	this->menu_index = 0;

	MenuElement* menu_none = new MenuElement("HEAD", MENU_SUBMENU, NULL, 8);

	this->menu_head = menu_none;
	this->menu_open = menu_none;

	// REGISTERING MENU ELEMENTS
	new MenuElement("main", MENU_SUBMENU, menu_none, 8);
	new MenuElement("some_settings", MENU_ACTION, menu_none->getChild("main"), 8);
	new MenuElement("some_settings2", MENU_ACTION, menu_none->getChild("main"), 8);
}


// BUTTON CALLBACK FUNCTIONS

static void MenuHandler::buttonNext(MenuHandler* self, uint8_t type) {
	self->menu_open->runCallback(0); //dummy value

	MenuElement* new_open_menu = self->menu_open->getChildAtIndex(self->menu_offset + self->menu_index);

	if (new_open_menu == NULL)
		return;

	if (not new_open_menu->isType(MENU_SUBMENU))
		return;

	self->menu_open = new_open_menu;
	self->menu_offset = 0;
	self->menu_index = 0;
}

static void MenuHandler::buttonPrev(MenuHandler* self, uint8_t type) {
	MenuElement* new_open_menu = self->menu_open->getParent();

	if (new_open_menu == NULL)
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
