#ifndef HUD_H
#define HUD_H
#include "Game.h"
#include "Item.h"
#include <queue>

#define H_GLOBE "hGlobe"
#define H_POOL "hPool"
#define A_SLOT "aSlot"
#define CAN_SELECT "can_select"
#define CANT_SELECT "cant_select"
#define A_SLOT_COUNT 8

class InvSlot {
public:
	sf::Vector2f start{ 10, 10 };

	unsigned int xOffset = 180;
	unsigned int spacing = 36;
	unsigned int width = 160;
	unsigned int height = 32;

	InvSlot() {}
	InvSlot(int index, bool eq)
	{
		this->position = sf::Vector2f{
			start.y + (xOffset *(int)eq),
			start.x + (spacing * index)
		};
	}
	~InvSlot() {}

	bool InvSlot::isHovering(const sf::Vector2f mousePos)
	{
		if (
			mousePos.x >= position.x &&
			mousePos.y >= position.y &&
			mousePos.x <= position.x + width &&
			mousePos.y <= position.y + height
			) return true;
		return false;
	}

	void setItem(Item* item) { this->item = item; }

	sf::Vector2f position;
private:
	Item * item;

private:
};

class Inventory {
public:
	Game * game;
	sf::Font font;
	std::vector<InvSlot> slots;

	sf::Sprite itemTextBack;
	sf::Sprite itemInfoBack;

	bool showInfo = false;

	Inventory() {}
	Inventory(Game* game) {
		this->game = game;
		this->font = this->game->fonts["main_font"];
		itemTextBack.setTexture(this->game->texmgr.getRef("item_text_back"));
		itemInfoBack.setTexture(this->game->texmgr.getRef("item_info_back"));
		itemTextBack.setOrigin(0, 0);
		itemInfoBack.setOrigin(0, 0);
		for (int i = 0; i < 3; i++)
		{
			slots.push_back(InvSlot(i, false));
		}
	}

	~Inventory() {}

	void draw();
	void update(sf::Vector2f mousePos);
private:
};

class Hud
{
public:
	std::map<std::string, sf::Sprite> elements;
	Game* game;

	sf::Text slotText[A_SLOT_COUNT];
	sf::Font font;


	std::vector<sf::Text> itemSlotNames;

	sf::Vector2f slotStart;
	float slotW = 32.f;

	std::vector<sf::Text> gameMsgs;
	bool showMsgs;
	bool showInv;
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
			slotText[i].setCharacterSize(12);
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

		showInv = false;

		showMsgs = false;
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