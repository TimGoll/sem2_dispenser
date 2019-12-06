#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

#include "callback.hpp"

#define MENU_ACTION 0
#define MENU_SUBMENU 1

#define MENU_DISPLAY_LINES 2

class MenuElement;

typedef struct children_struct {
	uint8_t children_amount = 0;
	uint8_t max_children_amount = 0;
	MenuElement** child;
} children_t;

class MenuElement : public Callback {
	public:
		MenuElement(uint8_t* name, uint8_t type, MenuElement* parent, uint8_t max_children_amount);
		~MenuElement();

		MenuElement* getChild(uint8_t* name);
		MenuElement* getChildAtIndex(uint8_t index);
		bool hasChild(uint8_t* name);
		uint8_t getChildAmount();
		MenuElement* getParent();
		bool addChild(MenuElement* child);
		uint8_t* getName();
		uint8_t getType();
		bool isType(uint8_t type);
		bool isTopLevel();

	private:
		uint8_t* name;
		uint8_t type;
		MenuElement* parent;
		children_t* children;
};

class MenuHandler {
	public:
		MenuHandler();
		~MenuHandler();

		static void buttonNext(MenuHandler* self, uint8_t type);
		static void buttonPrev(MenuHandler* self, uint8_t type);
		static void buttonUp(MenuHandler* self, uint8_t type);
		static void buttonDown(MenuHandler* self, uint8_t type);

		MenuElement** menuOpenPtr();
		uint8_t* menuIndexPtr();
		uint8_t* menuOffsetPtr();

	private:
		MenuElement* menu_open;
		MenuElement* menu_head;

		uint8_t menu_offset;
		uint8_t menu_index;
};

#endif
