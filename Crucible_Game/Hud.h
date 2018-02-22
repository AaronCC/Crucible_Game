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
#define xOffset 200
#define spacing 40
#define invSlotW 190
#define invSlotH 32
#define infoWidth 116
#define tSize 12
#define charWidth 8
#define slotBorder 2

public:
	sf::Vector2f start{ 25, 10 };
	sf::RectangleShape infoBack;
	sf::RectangleShape slotBack;
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
	InvSlot(int index, bool eq, Game* game)
	{
		this->item = nullptr;
		this->game = game;
		this->equipped = eq;
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
	std::vector<InvSlot> slots;
	std::vector<std::pair<InvSlot, sf::Text>> equipped;
	sf::Text invTitle;
	std::pair<sf::RectangleShape, sf::Text> deleteButton;
	sf::Sprite itemTextBack;
	sf::Sprite itemInfoBack;
	sf::RectangleShape slider;
	bool hoveringEq = false;
	bool lastSelected = false;
	int hovering;
	int selected;
	bool delHover = false;
	bool delSelect = false;

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

	bool eqToSlot(int eqIndex)
	{
		if (equipped[eqIndex].first.getItem() == nullptr)
			return false;
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i].getItem() == nullptr)
			{
				slots[i].setItem(equipped[eqIndex].first.getItem());
				equipped[eqIndex].first.removeItem();
				return true;
			}
		}
		return false;
	}
	bool hasOpenSlot()
	{
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i].getItem() == nullptr)
			{
				return true;
			}
		}
		return false;
	}
	bool slotToEq(int index, int eqIndex)
	{
		Item* item = slots[index].getItem();
		bool openSlot = hasOpenSlot();
		bool twoHEq = false;
		if (equipped[Item::SlotType::MAH].first.getItem() != nullptr)
			twoHEq = equipped[Item::SlotType::MAH].first.getItem()->twoHanded;
		if (slots[index].getItem() == nullptr)
			return false;
		if (item->twoHanded && openSlot)
		{
			eqToSlot(Item::SlotType::OFH);
		}
		else if (item->slotType == Item::SlotType::OFH &&
			twoHEq &&
			openSlot)
		{
			eqToSlot(Item::SlotType::MAH);
		}
		equipped[eqIndex].first.setItem(item);
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
		invTitle.setPosition({ invWidth - (charWidth * 10), 10 });
		invTitle.setFillColor(sf::Color::White);

		/* TEMP */
		Item* item;
		for (int i = 0; i < 25; i++)
		{
			slots.push_back(InvSlot(i, false, this->game));
		}
		item = new Item("Helm of the All-Seeing God",
			{ 0,0,0,12 },
			Item::SlotType::HED);
		slots[0].setItem(item);

		item = new Item("Ring of Power",
			{ 1,1,1,1 },
			Item::SlotType::RNG);
		slots[1].setItem(item);

		item = new Item("Two-handed Sword",
			{ 99,0,0,0 },
			Item::SlotType::MAH);
		slots[2].setItem(item);
		slots[2].getItem()->twoHanded = true;

		item = new Item("One-handed Sword",
			{ 10,0,1,0 },
			Item::SlotType::MAH);
		slots[3].setItem(item);

		item = new Item("Cardboard Shield",
			{ 0,2,0,0 },
			Item::SlotType::OFH);
		slots[4].setItem(item);


		std::vector<std::string> eqNames = { "Head","Body","Main-hand","Off-hand","Ring","Amulet","Cloak","Belt" };
		for (int i = 0; i < 8; i++)
		{
			equipped.push_back({ InvSlot(i,true,this->game),
				sf::Text(eqNames[i], game->fonts["main_font"], tSize) });
			equipped[i].second.setPosition(equipped[i].first.position - sf::Vector2f(0, 4 + tSize));
			equipped[i].second.setFillColor({ 255, 220, 125 });
		}
		/* END */
		slider.setSize({ 10, (invHeight + 10) / ((float)slots.size() - maxScHeight + 2) });
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
					equipped[selected].first.removeItem();
					equipped[selected].first.slotBack.setOutlineColor({ 150,150,200 });
				}
				else
				{
					slots[selected].removeItem();
					slots[selected].slotBack.setOutlineColor({ 150,150,200 });
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
			for (int i = 0; i < equipped.size(); i++)
				equipped[i].first.slotBack.setOutlineColor({ 150,150,200 });
			for (int i = 0; i < slots.size(); i++)
				slots[i].slotBack.setOutlineColor({ 150,150,200 });
			if (hoveringEq)
			{
				if (selected == hovering && lastSelected)
				{
					int slotType = equipped[hovering].first.getItemSlotType();
					equipped[hovering].first.selected = false;
					equipped[hovering].first.slotBack.setOutlineColor({ 150,150,200 });
					int selectedSlotType = equipped[hovering].first.getItemSlotType();
					if (selectedSlotType != -1)
					{
						eqToSlot(selectedSlotType);
					}
					selected = -1;
					lastSelected = true;
					return;
				}
				selected = hovering;
				lastSelected = true;
				equipped[hovering].first.selected = true;
				equipped[hovering].first.slotBack.setOutlineColor({ 255, 220, 125 });
				return;
			}
			else if (selected == hovering && !lastSelected)
			{
				int slotType = slots[selected].getItemSlotType();
				slots[selected].selected = false;
				slots[selected].slotBack.setOutlineColor({ 150,150,200 });
				if (slotType != -1)
				{
					eqToSlot(slotType);
					slotToEq(selected, slotType);
				}
				selected = -1;
				lastSelected = false;
				return;
			}
			selected = hovering;
			lastSelected = false;
			slots[selected].selected = true;
			slots[selected].slotBack.setOutlineColor({ 255, 220, 125 });

			delSelect = false;
		}
	}

	void scroll(int delta)
	{
		slotScIndex -= delta;
		if (slotScIndex < 0) {
			slotScIndex = 0;
		}
		else if (slotScIndex >= slots.size()) {
			slotScIndex = slots.size() - 1;
		}
		else if (slotScIndex >= slots.size() - maxScHeight) {
			slotScIndex = slots.size() - maxScHeight;
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