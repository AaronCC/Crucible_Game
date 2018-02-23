#ifndef HUD_H
#define HUD_H
#include "Game.h"
#include "Item.h"
#include <queue>
#include <string>
#include <sstream>
#include <iterator>

#define H_GLOBE "hGlobe"
#define H_POOL "hPool"
#define A_SLOT "aSlot"
#define CAN_SELECT "can_select"
#define CANT_SELECT "cant_select"
#define A_SLOT_COUNT 8

class InvSlot {
#define xOffset 192
#define spacing 40
#define invSlotW 182
#define invSlotH 32
#define infoWidth 148
#define tSize 12
#define charWidth 8
#define slotBorder 2

public:
	sf::Vector2f start{ 25, 10 };
	sf::RectangleShape infoBack;
	sf::RectangleShape slotBack;
	sf::Text slotTypeText;
	std::vector<sf::Text> itemText;
	std::vector<sf::Text> nameInfoText;
	std::vector<sf::Text> buffInfoText;
	bool equipped = false;
	bool selected = false;
	float lineH = tSize + 4;
	int maxInfoChar = infoWidth / charWidth;
	int maxNameChar = invSlotW / charWidth;

	std::vector<std::string> wrapWord(int lineLength, std::string name)
	{
		std::vector<std::string> wrapped;
		int charcount = 0;
		std::vector<std::string> words = chopWord(name);
		std::string newLine = words[0];
		charcount += words[0].length();
		int linecount = 0;
		for (int i = 1; i < words.size(); i++)
		{
			charcount += words[i].length() + 1;
			if (charcount >= lineLength)
			{
				wrapped.push_back(newLine);
				charcount = words[i].length() + 1;;
				linecount++;
				newLine = "";
			}
			newLine += " " + words[i];
		}
		if (newLine != "")
		{
			wrapped.push_back(newLine);
		}
		return wrapped;
	}

	std::vector<std::string> chopWord(std::string word)
	{
		std::istringstream buffer(word);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> words(beg, end);
		return words;
	}

	int getItemSlotType() {
		if (item == nullptr)
			return -1;
		return this->item->slotType;
	}

	Item* getItem() {
		return item;
	}

	void setItem(Item* item) {
		this->item = item;
		clearText();
		if (item->name.size() <= 0)
			return;
		int linecount = 0;

		slotTypeText = sf::Text(item->getTypeString(), game->fonts["main_font"], tSize);
		slotTypeText.setFillColor({ 102, 102, 255 });

		for (auto line : wrapWord(maxNameChar, item->name))
		{
			itemText.push_back(sf::Text(line, game->fonts["main_font"], tSize));
			itemText[linecount].setFillColor({ 90, 210, 220 });
			linecount++;
		}

		linecount = 0;
		for (auto line : wrapWord(maxInfoChar, item->name))
		{
			nameInfoText.push_back(sf::Text(line, game->fonts["main_font"], tSize));
			nameInfoText[linecount].setFillColor({ 90, 210, 220 });
			linecount++;
		}

		linecount = 0;
		for (auto buff : item->getBuffString())
		{
			for (auto line : wrapWord(maxInfoChar, buff))
			{
				buffInfoText.push_back(sf::Text(line, game->fonts["main_font"], tSize));
				buffInfoText[linecount].setFillColor(sf::Color::White);
				linecount++;
			}
		}
	}

	void updatePos(sf::Vector2f newPos)
	{
		this->position = newPos;
		this->slotBack.setPosition(this->position);
	}
	void clearText() {
		itemText.clear();
		nameInfoText.clear();
		buffInfoText.clear();
	}
	void removeItem() {
		clearText();
		this->item = nullptr;
	}
	InvSlot() {}
	InvSlot(int index, bool eq, Game* game, int offset)
	{
		this->item = nullptr;
		this->game = game;
		this->equipped = eq;
		if (offset > 1)
			this->start.x += offset * xOffset;
		this->position = sf::Vector2f{
			start.x + (xOffset * (int)eq),
			start.y + ((spacing + ((int)eq*tSize)) * (index)) + ((int)eq*tSize)
		};
		infoBack.setSize({ infoWidth, lineH });
		infoBack.setFillColor(sf::Color::Black);
		infoBack.setOrigin({ 0,0 });
		infoBack.setPosition(this->position);
		infoBack.setOutlineThickness(slotBorder);
		infoBack.setOutlineColor({ 150, 150, 200 });

		slotBack.setSize({ invSlotW, invSlotH });
		slotBack.setFillColor(sf::Color::Black);
		slotBack.setOrigin({ 0,0 });
		slotBack.setPosition(this->position);
		slotBack.setOutlineThickness(slotBorder);
		slotBack.setOutlineColor({ 150, 150, 200 });
		sf::Vector2f padd{ 4, 2 };
	}

	~InvSlot() {}

	bool InvSlot::isHovering(const sf::Vector2f mousePos)
	{
		if (
			mousePos.x >= position.x &&
			mousePos.y >= position.y &&
			mousePos.x <= position.x + invSlotW + (2 * slotBorder) &&
			mousePos.y <= position.y + invSlotH + (2 * slotBorder)
			) return true;
		return false;
	}

	sf::Vector2f position;
private:
	Item * item;
	Game* game;
private:
};

class Inventory {
#define invHeight 500
#define invWidth 800
public:
	Game * game;
	sf::Font font;
	std::vector<InvSlot> itemSlots;
	std::vector<std::pair<InvSlot, sf::Text>> eqItems;
	std::vector<InvSlot> scrollSlots;
	std::vector<std::pair<InvSlot, sf::Text>> eqScrolls;
	sf::Text invTitle;
	std::pair<sf::RectangleShape, sf::Text> deleteButton;
	sf::Sprite itemTextBack;
	sf::Sprite itemInfoBack;
	sf::RectangleShape slider;
	bool hoveringEq = false;
	enum Hovering {
		ITM,
		EQI,
		SCR,
		EQS,
		NONE
	};
	Hovering hovType;
	Hovering selType;
	int scrollSelect = -1;
	bool lastSelected = false;
	int hovering;
	int selected;
	bool delHover = false;
	bool delSelect = false;
	std::map<std::string, Ability> abilityMap;

	bool isHoveringDelete(sf::Vector2f mousePos)
	{
		sf::Vector2f position = deleteButton.first.getPosition();
		sf::Vector2f size = deleteButton.first.getSize();
		if (
			mousePos.x >= position.x &&
			mousePos.y >= position.y &&
			mousePos.x <= position.x + size.x + (2 * slotBorder) &&
			mousePos.y <= position.y + size.y + (2 * slotBorder)
			) return true;
		return false;
	}

	bool eqToSlot(int eqIndex, std::vector<InvSlot>& slots, std::vector<std::pair<InvSlot, sf::Text>>& eqSlots)
	{
		if (eqSlots[eqIndex].first.getItem() == nullptr)
			return false;
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i].getItem() == nullptr)
			{
				slots[i].setItem(eqSlots[eqIndex].first.getItem());
				eqSlots[eqIndex].first.removeItem();
				return true;
			}
		}
		return false;
	}
	bool hasOpenSlot()
	{
		for (int i = 0; i < itemSlots.size(); i++)
		{
			if (itemSlots[i].getItem() == nullptr)
			{
				return true;
			}
		}
		return false;
	}
	bool slotToEq(int index, int eqIndex, std::vector<InvSlot>& slots, std::vector<std::pair<InvSlot, sf::Text>>& eqSlots)
	{
		Item* item = slots[index].getItem();
		bool openSlot = hasOpenSlot();
		bool twoHEq = false;
		if (eqSlots[Item::SlotType::MAH].first.getItem() != nullptr)
			twoHEq = eqSlots[Item::SlotType::MAH].first.getItem()->twoHanded;
		if (slots[index].getItem() == nullptr)
			return false;
		if (item->twoHanded && openSlot)
		{
			eqToSlot(Item::SlotType::OFH, slots, eqSlots);
		}
		else if (item->slotType == Item::SlotType::OFH &&
			twoHEq &&
			openSlot)
		{
			eqToSlot(Item::SlotType::MAH, slots, eqSlots);
		}
		eqSlots[eqIndex].first.setItem(item);
		slots[index].removeItem();
		return true;
	}

	bool showInfo = false;

	Inventory() {}
	Inventory(Game* game) {
		this->game = game;
		this->font = this->game->fonts["main_font"];
		itemTextBack.setTexture(this->game->texmgr.getRef("item_text_back"));
		itemInfoBack.setTexture(this->game->texmgr.getRef("item_info_back"));
		itemTextBack.setOrigin(0, 0);
		itemInfoBack.setOrigin(0, 0);
		invTitle = sf::Text("Inventory", game->fonts["main_font"], tSize);
		invTitle.setPosition({ invWidth - (charWidth * 10), invHeight - 30 });
		invTitle.setFillColor(sf::Color::White);
		hovType = Hovering::NONE;
		/* TEMP */
#pragma region ITEMS
		Item* item;
		for (int i = 0; i < 25; i++)
		{
			itemSlots.push_back(InvSlot(i, false, this->game, 0));
		}
		item = new Item("Helm of the All-Seeing God",
			{ 0,0,0,12 },
			Item::SlotType::HED);
		itemSlots[0].setItem(item);

		item = new Item("Ring of Power",
			{ 1,1,1,1 },
			Item::SlotType::RNG);
		itemSlots[1].setItem(item);

		item = new Item("Two-handed Sword",
			{ 20,0,-2,0 },
			Item::SlotType::MAH);
		itemSlots[2].setItem(item);
		itemSlots[2].getItem()->twoHanded = true;

		item = new Item("One-handed Sword",
			{ 10,0,2,0 },
			Item::SlotType::MAH);
		itemSlots[3].setItem(item);

		item = new Item("Cardboard Shield",
			{ 0,2,0,-10 },
			Item::SlotType::OFH);
		itemSlots[4].setItem(item);
		std::vector<std::string> eqNames = { "Head","Body","Main-hand","Off-hand","Ring","Amulet","Cloak","Belt" };
		for (int i = 0; i < 8; i++)
		{
			eqItems.push_back({ InvSlot(i,true,this->game,1),
				sf::Text(eqNames[i], game->fonts["main_font"], tSize) });
			eqItems[i].second.setPosition(eqItems[i].first.position - sf::Vector2f(0, 4 + tSize));
			eqItems[i].second.setFillColor({ 255, 220, 125 });
		}
#pragma endregion
#pragma region SCROLLS
		Scroll* scroll;
		for (int i = 0; i < 25; i++)
		{
			scrollSlots.push_back(InvSlot(i, false, this->game, 2));
		}
		abilityMap["slash"] = Ability(this->game, game->texmgr.getRef("slash"),
			{ 0,3,0.1f }, { 32,32 }, Ability::ID::SLASH, 10, 1, 7, 2, "slash",
			"Melee slash in an arc");
		Ability* slash = new Ability(abilityMap["slash"]);
		scroll = new Scroll("Slash",
			{ 0,0,0,0 },
			Item::SlotType::SCR,
			slash);
		scrollSlots[0].setItem(scroll);
		std::vector<std::string> scEqNames = { "0","1","2","3","4","5","6","RMB","LMB" };
		for (int i = 0; i < A_SLOT_COUNT; i++)
		{
			eqScrolls.push_back({ InvSlot(i,true,this->game,2),
				sf::Text(scEqNames[i], game->fonts["main_font"], tSize) });
			eqScrolls[i].second.setPosition(eqScrolls[i].first.position - sf::Vector2f(0, 4 + tSize));
			eqScrolls[i].second.setFillColor({ 255, 220, 125 });
		}
#pragma endregion
		/* END */
		slider.setSize({ 10, (invHeight + 10) / ((float)itemSlots.size() - maxScHeight + 2) });
		slider.setFillColor(sf::Color::White);
		slider.setOrigin({ 0,0 });
		slider.setPosition({ 10,10 });
		deleteButton.first.setSize({ invSlotW / 2,invSlotH });
		deleteButton.first.setFillColor(sf::Color::Black);
		deleteButton.first.setOrigin({ 0,0 });
		deleteButton.first.setPosition({ 25 + xOffset,10 + invHeight - (1.5*spacing) });
		deleteButton.first.setOutlineThickness(2.f);
		deleteButton.first.setOutlineColor({ 150,150,200 });
		deleteButton.second = sf::Text("Destroy", game->fonts["main_font"], tSize);
		deleteButton.second.setPosition(deleteButton.first.getPosition() + (deleteButton.first.getSize() / 4.2f));
		deleteButton.second.setFillColor({ 255, 90, 50 });
	}

	~Inventory() {}

	void select() {
		if (delHover)
		{
			if (delSelect)
			{
				if (lastSelected)
				{
					if (selType == Hovering::EQI)
					{
						eqItems[selected].first.removeItem();
						eqItems[selected].first.slotBack.setOutlineColor({ 150,150,200 });
					}
					else
					{
						eqScrolls[selected].first.removeItem();
						eqScrolls[selected].first.slotBack.setOutlineColor({ 150,150,200 });
					}
				}
				else
				{
					if (selType == Hovering::ITM)
					{
						itemSlots[selected].removeItem();
						itemSlots[selected].slotBack.setOutlineColor({ 150,150,200 });
					}
					else
					{
						scrollSlots[selected].removeItem();
						scrollSlots[selected].slotBack.setOutlineColor({ 150,150,200 });
					}
				}
				deleteButton.first.setOutlineColor({ 150,150,200 });
				selected = -1;
				delSelect = false;
				return;
			}
			if (selected >= 0)
			{
				delSelect = true;
				deleteButton.first.setOutlineColor({ 255,220,125 });
			}
		}
		else {
			deleteButton.first.setOutlineColor({ 150,150,200 });
			for (int i = 0; i < eqItems.size(); i++)
				eqItems[i].first.slotBack.setOutlineColor({ 150,150,200 });
			for (int i = 0; i < itemSlots.size(); i++)
				itemSlots[i].slotBack.setOutlineColor({ 150,150,200 });
			for (int i = 0; i < eqItems.size(); i++)
				eqScrolls[i].first.slotBack.setOutlineColor({ 150,150,200 });
			for (int i = 0; i < itemSlots.size(); i++)
				scrollSlots[i].slotBack.setOutlineColor({ 150,150,200 });
			switch (hovType)
			{
			case Hovering::EQI:
			{
				if (selected == hovering && lastSelected)
				{
					int slotType = eqItems[hovering].first.getItemSlotType();
					eqItems[hovering].first.selected = false;
					eqItems[hovering].first.slotBack.setOutlineColor({ 150,150,200 });
					int selectedSlotType = eqItems[hovering].first.getItemSlotType();
					if (selectedSlotType != -1)
					{
						eqToSlot(selectedSlotType, itemSlots, eqItems);
					}
					selected = -1;
					scrollSelect = -1;
					lastSelected = true;
					selType = Hovering::NONE;
					return;
				}
				selected = hovering;
				lastSelected = true;
				eqItems[hovering].first.selected = true;
				eqItems[hovering].first.slotBack.setOutlineColor({ 255, 220, 125 });
				scrollSelect = -1;
				selType = hovType;
				return;
			}
			case Hovering::ITM:
			{
				if (selected == hovering && !lastSelected)
				{
					int slotType = itemSlots[selected].getItemSlotType();
					itemSlots[selected].selected = false;
					itemSlots[selected].slotBack.setOutlineColor({ 150,150,200 });
					if (slotType != -1)
					{
						eqToSlot(slotType, itemSlots, eqItems);
						slotToEq(selected, slotType, itemSlots, eqItems);
					}
					selected = -1;
					scrollSelect = -1;
					lastSelected = false;
					selType = Hovering::NONE;
					return;
				}
				selected = hovering;
				lastSelected = false;
				itemSlots[selected].selected = true;
				itemSlots[selected].slotBack.setOutlineColor({ 255, 220, 125 });
				scrollSelect = -1;
				selType = hovType;
				break;
			}
			case Hovering::EQS:
			{
				if (selected == hovering && lastSelected)
				{
					int slotType = eqScrolls[hovering].first.getItemSlotType();
					eqScrolls[hovering].first.selected = false;
					eqScrolls[hovering].first.slotBack.setOutlineColor({ 150,150,200 });
					int selectedSlotType = eqScrolls[hovering].first.getItemSlotType();
					if (selectedSlotType != -1)
					{
						eqToSlot(hovering, scrollSlots, eqScrolls);
					}
					selected = -1;
					lastSelected = true;
					selType = Hovering::NONE;
					return;
				}
				selected = hovering;
				selType = hovType;
				if (scrollSelect != -1)
				{
					eqToSlot(selected, scrollSlots, eqScrolls);
					slotToEq(scrollSelect, selected, scrollSlots, eqScrolls);
				}
				else
				{
					lastSelected = true;
					eqScrolls[hovering].first.selected = true;
					eqScrolls[hovering].first.slotBack.setOutlineColor({ 255, 220, 125 });
				}
				scrollSelect = -1;
				return;
			}
			case Hovering::SCR:
			{
				if (selected == hovering && !lastSelected)
				{
					int slotType = scrollSlots[selected].getItemSlotType();
					scrollSlots[selected].selected = false;
					scrollSlots[selected].slotBack.setOutlineColor({ 150,150,200 });
					if (slotType != -1)
					{
						/*eqToSlot(slotType, scrollSlots, eqScrolls);
						slotToEq(selected, slotType, scrollSlots, eqScrolls);*/
					}
					selected = -1;
					lastSelected = false;
					selType = Hovering::NONE;
					return;
				}
				selected = hovering;
				scrollSelect = selected;
				lastSelected = false;
				scrollSlots[selected].selected = true;
				scrollSlots[selected].slotBack.setOutlineColor({ 255, 220, 125 });
				selType = hovType;
				break;
			}
			default:
				scrollSelect = -1;
				break;
			}
			delSelect = false;
		}
	}

	void scrollItems(int delta)
	{
		slotScIndex -= delta;
		if (slotScIndex < 0) {
			slotScIndex = 0;
		}
		else if (slotScIndex >= itemSlots.size()) {
			slotScIndex = itemSlots.size() - 1;
		}
		else if (slotScIndex >= itemSlots.size() - maxScHeight) {
			slotScIndex = itemSlots.size() - maxScHeight;
		}
		slider.setPosition({ 10,10 + (slotScIndex*(float)slider.getSize().y) });
	}

	void draw();
	void update(sf::Vector2f mousePos);
private:
	int slotScIndex = 0;
	unsigned int maxScHeight = invHeight / spacing;
};

class Hud
{
public:
	std::map<std::string, sf::Sprite> elements;
	Game* game;

	sf::Text slotText[A_SLOT_COUNT];
	sf::Font font;

	sf::Text msgTitle;

	std::vector<sf::Text> itemSlotNames;

	sf::Vector2f slotStart;
	float slotW = 32.f;

	std::vector<sf::Text> gameMsgs;
	enum ShowState {
		SHOW_MSG,
		SHOW_INV,
		SHOW_NONE
	};
	ShowState showState;

	sf::Sprite msgBack;

	struct Cooldown {
		float totalTime;
		float timer;
		bool active;
		sf::IntRect bounds;
	};

	std::vector<Cooldown> cooldowns;

	std::map<sf::Keyboard::Key, bool> keys;

	std::map<sf::Keyboard::Key, std::pair<sf::Sprite, bool>> aSlotSprites;
	std::pair<sf::Sprite, bool> lmbSprite;
	std::pair<sf::Sprite, bool> rmbSprite;
	sf::Vector2f msgStart = sf::Vector2f(6, 478);

	sf::Sprite cdSprite;

	void setSlotSprites(std::vector<std::pair<sf::Keyboard::Key, std::string>> slotSpriteIDs,
		std::string lmbID, std::string rmbID);

	void queueMsg(std::string msg);

	void setCooldown(int index, float timer) {
		if (index >= A_SLOT_COUNT)
			return;
		cooldowns[index].active = true;
		cooldowns[index].timer = timer;
		cooldowns[index].totalTime = timer;
		cooldowns[index].bounds = sf::IntRect(slotStart.x + (index * slotW), slotStart.y, slotW, slotW);
	}

	Hud() {};
	Hud(Game * game, std::vector<std::string> eData)
	{
		this->showState = ShowState::SHOW_NONE;
		msgTitle = sf::Text("Game Messages", game->fonts["main_font"], tSize);
		msgTitle.setPosition({ invWidth - (charWidth * 13), 10 });
		msgTitle.setFillColor(sf::Color::White);
		//font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		for (int i = 0; i < A_SLOT_COUNT; i++)
		{
			slotText[i].setFont(game->fonts["main_font"]);
			if (i < A_SLOT_COUNT - 2)
				slotText[i].setString(std::to_string(i));
		}
		slotText[A_SLOT_COUNT - 2].setString("LMB");
		slotText[A_SLOT_COUNT - 1].setString("RMB");

		unsigned int eCount = 0;
		this->game = game;
		/* Health */
		elements[H_GLOBE].setTexture(game->texmgr.getRef(eData[eCount]));
		elements[H_GLOBE].setPosition(15.f, game->windowSize.y - 15.f);
		elements[H_GLOBE].setOrigin(0, elements[H_GLOBE].getTexture()->getSize().y);
		eCount++;

		elements[H_POOL].setTexture(game->texmgr.getRef(eData[eCount]));
		elements[H_POOL].setPosition(15.f, game->windowSize.y - 15.f);
		elements[H_POOL].setOrigin(0, elements[H_POOL].getTexture()->getSize().y);
		eCount++;

		slotStart = { 95.f, game->windowSize.y - 15.f };
		for (int i = 0; i < A_SLOT_COUNT; i++)
		{
			std::string slotID = A_SLOT + std::to_string(i);
			sf::Vector2f slotPos = { slotStart.x + (slotW * i), slotStart.y };
			elements[slotID].setTexture(game->texmgr.getRef(eData[eCount]));
			elements[slotID].setPosition(slotPos);
			elements[slotID].setOrigin(0, elements[slotID].getTexture()->getSize().y);
			slotText[i].setPosition(slotPos.x + (slotW / 2) - (slotText[i].getString().getSize() * 4.f), slotPos.y - slotW - 20.f);
			slotText[i].setCharacterSize(tSize);
		}
		eCount++;

		cooldowns.reserve(A_SLOT_COUNT);

		for (int cd = 0; cd < A_SLOT_COUNT; cd++)
		{
			cooldowns.push_back(Cooldown());
			setCooldown(cd, 0);
		}

		cdSprite.setTexture(this->game->texmgr.getRef("cooldown_icon"));
		cdSprite.setOrigin(0, slotW);
		msgBack.setTexture(this->game->texmgr.getRef("text_back"));
		msgBack.setOrigin(0, 0);
		msgBack.setPosition(0, 0);

		keys[sf::Keyboard::Tilde] = false;
	}

	void draw(float dt);
	void updateCD(unsigned int slot, unsigned int ticks);
	void update(float dt);

	void updateHealth(float dmg);

	void handleInput();

	~Hud();
};

#endif /* HUD_H */