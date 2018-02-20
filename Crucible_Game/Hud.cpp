#include "Hud.h"

void Hud::setSlotSprites(std::vector<std::pair<sf::Keyboard::Key, std::string>> slotSpriteIDs, std::string lmbID, std::string rmbID)
{
	unsigned int idNum = 0;
	for (auto id : slotSpriteIDs)
	{
		if (id.second == "")
		{
			this->aSlotSprites[id.first].second = false;
			continue;
		}
		this->aSlotSprites[id.first].second = true;
		this->aSlotSprites[id.first].first.setTexture(this->game->texmgr.getRef(id.second));
		this->aSlotSprites[id.first].first.setOrigin(0, 32.f);
		this->aSlotSprites[id.first].first.setPosition(elements[A_SLOT + std::to_string(idNum)].getPosition());
		idNum++;
	}
	sf::Vector2f pos{ 0,0 };
	if (lmbID == "")
		this->lmbSprite.second = false;
	else
	{
		this->lmbSprite.second = true;
		this->lmbSprite.first.setTexture(this->game->texmgr.getRef(lmbID));
		this->lmbSprite.first.setOrigin(0, 32.f);
		this->lmbSprite.first.setTextureRect({ 0, 0, 32, 32 });
		pos = elements[A_SLOT + std::to_string(A_SLOT_COUNT - 2)].getPosition();
		this->lmbSprite.first.setPosition(pos);
	}
	if (rmbID == "")
		this->rmbSprite.second = false;
	else
	{
		this->rmbSprite.second = true;
		this->rmbSprite.first.setTexture(this->game->texmgr.getRef(rmbID));
		this->rmbSprite.first.setOrigin(0, 32.f);
		this->rmbSprite.first.setTextureRect({ 0, 0, 32, 32 });
		this->rmbSprite.first.setPosition(elements[A_SLOT + std::to_string(A_SLOT_COUNT - 1)].getPosition());
	}
}

void Hud::queueMsg(std::string msg)
{
	sf::Text text;
	text.setFont(this->game->fonts["main_font"]);
	text.setString(msg);
	text.setCharacterSize(12);
	gameMsgs.push_back(text);
}

void Hud::draw(float dt)
{
	for (auto element : elements)
	{
		this->game->window.draw(element.second);
	}
	for (int i = 0; i < A_SLOT_COUNT; i++)
	{
		this->game->window.draw(slotText[i]);
	}
	if (rmbSprite.second)
	{
		this->game->window.draw(rmbSprite.first);
	}
	if (lmbSprite.second)
	{
		this->game->window.draw(lmbSprite.first);
	}
	for (int cd = 0; cd < A_SLOT_COUNT; cd++)
	{
		if (cooldowns[cd].active)
		{
			sf::Vector2f pos = elements[A_SLOT + std::to_string(cd)].getPosition();
			pos.y += cooldowns[cd].bounds.top;
			cdSprite.setPosition(pos);
			cdSprite.setTextureRect(cooldowns[cd].bounds);
			this->game->window.draw(cdSprite);
		}
	}
	sf::Vector2f offset = { 0,0 };
	if (showMsgs || showInv)
	{
		this->game->window.draw(msgBack);
		if (showMsgs)
			for (int i = 0; i < gameMsgs.size(); i++)
			{
				gameMsgs[i].setPosition(msgStart + offset);
				this->game->window.draw(gameMsgs[i]);
				offset.y -= 10;
			}
	}
}
void Hud::updateCD(unsigned int slot, unsigned int ticks)
{
	if (cooldowns[slot].active)
	{
		cooldowns[slot].timer -= ticks;
	}
}
void Hud::update(float dt)
{
	for (int cd = 0; cd < A_SLOT_COUNT; cd++)
	{
		if (cooldowns[cd].active)
		{
			//cooldowns[cd].timer -= dt;
			if (cooldowns[cd].timer < 0)
			{
				cooldowns[cd].active = false;
				cooldowns[cd].timer = 0;
			}
			else
			{
				float remaining = cooldowns[cd].timer / cooldowns[cd].totalTime;
				int newH = (1.f - remaining)*slotW;
				cooldowns[cd].bounds = sf::IntRect(
					cooldowns[cd].bounds.left,
					newH,
					slotW,
					(int)(slotW * remaining)
				);
			}
		}
	}
}

void Hud::updateHealth(float percent)
{
	sf::IntRect hRect = elements[H_GLOBE].getTextureRect();
	int newH = (1.f - percent)*(float)hRect.height;
	elements[H_POOL].setTextureRect({ 0, newH,hRect.width,(int)(hRect.height * percent) });
	sf::Vector2f pos = elements[H_GLOBE].getPosition();
	elements[H_POOL].setPosition(pos.x, pos.y + newH);
}

void Hud::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && keys[sf::Keyboard::Tilde] == false)
	{
		showMsgs = !showMsgs;
		keys[sf::Keyboard::Tilde] = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
	{
		keys[sf::Keyboard::Tilde] = false;
	}
}

Hud::~Hud()
{
}

void Inventory::draw()
{
	for (auto slot : slots)
	{
		itemTextBack.setPosition(slot.position);
		//itemInfoBack.setPosition(slot.position);
		this->game->window.draw(itemTextBack);
		//this->game->window.draw(itemInfoBack);
		for (auto text : slot.itemText)
		{
			this->game->window.draw(text);
		}
	}
	if (showInfo)
	{
		this->game->window.draw(hovering.infoBack);
		for (auto text : hovering.infoText)
		{
			this->game->window.draw(text);
		}
	}
}

void Inventory::update(sf::Vector2f mousePos)
{
	mousePos += {2, 2};
	showInfo = false;
	float offset = 0.f;
	for (int s = 0; s < slots.size(); s++)
	{
		for (int i = 0; i < slots[s].itemText.size(); i++)
		{
			int textL = slots[s].itemText[i].getString().getSize();
			offset = (((slots[s].maxNameChar+1) - textL) / 2.f)*(charWidth);
			slots[s].itemText[i].setPosition(slots[s].position.x + offset, slots[s].position.y + (i*tSize));
		}
		if (slots[s].isHovering(mousePos))
		{
			hovering = slots[s];
			showInfo = true;
			break;
		}
	}
	if (showInfo) {
		for (int i = 0; i < hovering.infoText.size(); i++)
		{
			int textL = hovering.infoText[i].getString().getSize();
			offset = (((hovering.maxInfoChar+1) - textL) / 2.f)*(charWidth);
			hovering.infoText[i].setPosition(mousePos.x+offset,mousePos.y+(i*tSize));
		}
		itemInfoBack.setPosition(mousePos);
		hovering.infoBack.setPosition(mousePos);
		hovering.infoBack.setSize({ infoWidth, (tSize+2) * (float)hovering.infoText.size() });
	}
}
