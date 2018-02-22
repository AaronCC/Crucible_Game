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
	if (showState != ShowState::SHOW_NONE)
	{
		this->game->window.draw(msgBack);
		if (showState == Hud::ShowState::SHOW_MSG)
		{ 
			for (int i = 0; i < gameMsgs.size(); i++)
			{
				gameMsgs[i].setPosition(msgStart + offset);
				this->game->window.draw(gameMsgs[i]);
				offset.y -= 10;
			}
			this->game->window.draw(msgTitle);
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
		if (showState == Hud::ShowState::SHOW_MSG)
			showState = Hud::ShowState::SHOW_NONE;
		else
			showState = Hud::ShowState::SHOW_MSG;
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
	this->game->window.draw(slider);
	this->game->window.draw(invTitle);
	this->game->window.draw(deleteButton.first);
	this->game->window.draw(deleteButton.second);
	for (int i = slotScIndex; i < maxScHeight + slotScIndex; i++)
	{
		if (i >= slots.size())
			break;
		itemTextBack.setPosition(slots[i].position);
		this->game->window.draw(slots[i].slotBack);
		for (auto text : slots[i].itemText)
		{
			this->game->window.draw(text);
		}
	}
	for (auto slot : equipped)
	{
		this->game->window.draw(slot.first.slotBack);
		for (auto text : slot.first.itemText)
		{
			this->game->window.draw(text);
		}
		this->game->window.draw(slot.second);
	}
	if (showInfo)
	{
		if (hoveringEq && equipped[hovering].first.nameInfoText.size() > 0)
		{
			this->game->window.draw(equipped[hovering].first.infoBack);

			for (auto text : equipped[hovering].first.nameInfoText)
			{
				this->game->window.draw(text);
			}
			for (auto text : equipped[hovering].first.buffInfoText)
			{
				this->game->window.draw(text);
			}
		}
		else if(!hoveringEq && slots[hovering].nameInfoText.size() > 0)
		{
			this->game->window.draw(slots[hovering].infoBack);

			for (auto text : slots[hovering].nameInfoText)
			{
				this->game->window.draw(text);
			}
			for (auto text : slots[hovering].buffInfoText)
			{
				this->game->window.draw(text);
			}
		}
	}
}

void Inventory::update(sf::Vector2f mousePos)
{
	mousePos += {2, 2};
	showInfo = false;
	float offset = 0.f;
	int index = 0;
	for (int s = slotScIndex; s < slots.size(); s++)
	{
		/*if (s >= slots.size())
			break;*/
		slots[s].updatePos({
			slots[s].start.x + (xOffset * (int)slots[s].equipped),
			slots[s].start.y + (spacing * index)
			});
		for (int i = 0; i < slots[s].itemText.size(); i++)
		{
			int textL = slots[s].itemText[i].getString().getSize();
			offset = (((slots[s].maxNameChar + 1) - textL) / 2.f)*(charWidth);
			slots[s].itemText[i].setPosition(slots[s].position.x + offset, slots[s].position.y + (i*tSize));
		}
		if (slots[s].isHovering(mousePos))
		{
			hoveringEq = false;
			hovering = s;
			showInfo = true;
		}
		else if (isHoveringDelete(mousePos))
		{
			delHover = true;
		}
		else
			delHover = false;
		index++;
	}
	for (int s = 0; s < equipped.size(); s++)
	{

		for (int i = 0; i < equipped[s].first.itemText.size(); i++)
		{
			int textL = equipped[s].first.itemText[i].getString().getSize();
			offset = (((equipped[s].first.maxNameChar + 1) - textL) / 2.f)*(charWidth);
			equipped[s].first.itemText[i].setPosition(equipped[s].first.position.x + offset, equipped[s].first.position.y + (i*tSize));
		}
		if (equipped[s].first.isHovering(mousePos))
		{
			hoveringEq = true;
			hovering = s;
			showInfo = true;
		}
		index++;
	}
	if (showInfo) {
		if (hoveringEq)
		{
			for (int i = 0; i < equipped[hovering].first.nameInfoText.size(); i++)
			{
				int textL = equipped[hovering].first.nameInfoText[i].getString().getSize();
				offset = (((equipped[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				equipped[hovering].first.nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = equipped[hovering].first.nameInfoText.size() + 1;
			for (int i = 0; i < equipped[hovering].first.buffInfoText.size(); i++)
			{
				int textL = equipped[hovering].first.buffInfoText[i].getString().getSize();
				offset = (((equipped[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				equipped[hovering].first.buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			equipped[hovering].first.infoBack.setPosition(mousePos);
			equipped[hovering].first.infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + equipped[hovering].first.buffInfoText.size()) });
		}
		else
		{
			for (int i = 0; i < slots[hovering].nameInfoText.size(); i++)
			{
				int textL = slots[hovering].nameInfoText[i].getString().getSize();
				offset = (((slots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				slots[hovering].nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = slots[hovering].nameInfoText.size() + 1;
			for (int i = 0; i < slots[hovering].buffInfoText.size(); i++)
			{
				int textL = slots[hovering].buffInfoText[i].getString().getSize();
				offset = (((slots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				slots[hovering].buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			slots[hovering].infoBack.setPosition(mousePos);
			slots[hovering].infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + slots[hovering].buffInfoText.size()) });
		}
	}
}
