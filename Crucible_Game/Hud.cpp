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
		if (i >= itemSlots.size())
			break;
		itemTextBack.setPosition(itemSlots[i].position);
		this->game->window.draw(itemSlots[i].slotBack);
		for (auto text : itemSlots[i].itemText)
		{
			this->game->window.draw(text);
		}
	}
	for (auto slot : eqItems)
	{
		this->game->window.draw(slot.first.slotBack);
		for (auto text : slot.first.itemText)
		{
			this->game->window.draw(text);
		}
		this->game->window.draw(slot.second);
	}
	for (int i = slotScIndex; i < maxScHeight + slotScIndex; i++)
	{
		if (i >= scrollSlots.size())
			break;
		itemTextBack.setPosition(scrollSlots[i].position);
		this->game->window.draw(scrollSlots[i].slotBack);
		for (auto text : scrollSlots[i].itemText)
		{
			this->game->window.draw(text);
		}
	}
	for (auto slot : eqScrolls)
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
		switch (hovType)
		{
		case Hovering::EQI:
		{
			if (eqItems[hovering].first.nameInfoText.size() > 0)
			{
				this->game->window.draw(eqItems[hovering].first.infoBack);

				for (auto text : eqItems[hovering].first.nameInfoText)
				{
					this->game->window.draw(text);
				}
				for (auto text : eqItems[hovering].first.buffInfoText)
				{
					this->game->window.draw(text);
				}
				this->game->window.draw(eqItems[hovering].first.slotTypeText);
			}
			break;
		}
		case Hovering::ITM:
		{
			if (itemSlots[hovering].nameInfoText.size() > 0)
			{
				this->game->window.draw(itemSlots[hovering].infoBack);

				for (auto text : itemSlots[hovering].nameInfoText)
				{
					this->game->window.draw(text);
				}
				for (auto text : itemSlots[hovering].buffInfoText)
				{
					this->game->window.draw(text);
				}
				this->game->window.draw(itemSlots[hovering].slotTypeText);
			}
			break;
		}
		case Hovering::EQS:
		{
			if (eqScrolls[hovering].first.nameInfoText.size() > 0)
			{
				this->game->window.draw(eqScrolls[hovering].first.infoBack);

				for (auto text : eqScrolls[hovering].first.nameInfoText)
				{
					this->game->window.draw(text);
				}
				for (auto text : eqScrolls[hovering].first.buffInfoText)
				{
					this->game->window.draw(text);
				}
				this->game->window.draw(eqScrolls[hovering].first.slotTypeText);
			}
			break;
		}
		case Hovering::SCR:
		{
			if (scrollSlots[hovering].nameInfoText.size() > 0)
			{
				this->game->window.draw(scrollSlots[hovering].infoBack);

				for (auto text : scrollSlots[hovering].nameInfoText)
				{
					this->game->window.draw(text);
				}
				for (auto text : scrollSlots[hovering].buffInfoText)
				{
					this->game->window.draw(text);
				}
				this->game->window.draw(scrollSlots[hovering].slotTypeText);
			}
			break;
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
	for (int s = slotScIndex; s < scrollSlots.size(); s++)
	{
		/*if (s >= slots.size())
		break;*/
		scrollSlots[s].updatePos({
			scrollSlots[s].start.x + (xOffset * (int)scrollSlots[s].equipped),
			scrollSlots[s].start.y + (spacing * index)
			});
		for (int i = 0; i < scrollSlots[s].itemText.size(); i++)
		{
			int textL = scrollSlots[s].itemText[i].getString().getSize();
			offset = (((scrollSlots[s].maxNameChar + 1) - textL) / 2.f)*(charWidth);
			scrollSlots[s].itemText[i].setPosition(scrollSlots[s].position.x + offset, scrollSlots[s].position.y + (i*tSize));
		}
		if (scrollSlots[s].isHovering(mousePos))
		{
			hovType = Hovering::SCR;
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
	for (int s = 0; s < eqScrolls.size(); s++)
	{
		for (int i = 0; i < eqScrolls[s].first.itemText.size(); i++)
		{
			int textL = eqScrolls[s].first.itemText[i].getString().getSize();
			offset = (((eqScrolls[s].first.maxNameChar + 1) - textL) / 2.f)*(charWidth);
			eqScrolls[s].first.itemText[i].setPosition(eqScrolls[s].first.position.x + offset, eqScrolls[s].first.position.y + (i*tSize));
		}
		if (eqScrolls[s].first.isHovering(mousePos))
		{
			hovType = Hovering::EQS;
			hoveringEq = true;
			hovering = s;
			showInfo = true;
		}
		index++;
	}
	index = 0;
	for (int s = slotScIndex; s < itemSlots.size(); s++)
	{
		/*if (s >= slots.size())
			break;*/
		itemSlots[s].updatePos({
			itemSlots[s].start.x + (xOffset * (int)itemSlots[s].equipped),
			itemSlots[s].start.y + (spacing * index)
			});
		for (int i = 0; i < itemSlots[s].itemText.size(); i++)
		{
			int textL = itemSlots[s].itemText[i].getString().getSize();
			offset = (((itemSlots[s].maxNameChar + 1) - textL) / 2.f)*(charWidth);
			itemSlots[s].itemText[i].setPosition(itemSlots[s].position.x + offset, itemSlots[s].position.y + (i*tSize));
		}
		if (itemSlots[s].isHovering(mousePos))
		{
			hovType = Hovering::ITM;
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
	for (int s = 0; s < eqItems.size(); s++)
	{
		for (int i = 0; i < eqItems[s].first.itemText.size(); i++)
		{
			int textL = eqItems[s].first.itemText[i].getString().getSize();
			offset = (((eqItems[s].first.maxNameChar + 1) - textL) / 2.f)*(charWidth);
			eqItems[s].first.itemText[i].setPosition(eqItems[s].first.position.x + offset, eqItems[s].first.position.y + (i*tSize));
		}
		if (eqItems[s].first.isHovering(mousePos))
		{
			hovType = Hovering::EQI;
			hoveringEq = true;
			hovering = s;
			showInfo = true;
		}
		index++;
	}
	if (showInfo) {
		int textL;
		switch (hovType)
		{
		case Hovering::EQI:
		{
			for (int i = 0; i < eqItems[hovering].first.nameInfoText.size(); i++)
			{
				textL = eqItems[hovering].first.nameInfoText[i].getString().getSize();
				offset = (((eqItems[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				eqItems[hovering].first.nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = eqItems[hovering].first.nameInfoText.size() + 3;
			textL = eqItems[hovering].first.slotTypeText.getString().getSize();
			offset = (((eqItems[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
			eqItems[hovering].first.slotTypeText.setPosition(mousePos.x + offset, mousePos.y + ((nSize - 2)*tSize));
			for (int i = 0; i < eqItems[hovering].first.buffInfoText.size(); i++)
			{
				textL = eqItems[hovering].first.buffInfoText[i].getString().getSize();
				offset = (((eqItems[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				eqItems[hovering].first.buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			eqItems[hovering].first.infoBack.setPosition(mousePos);
			eqItems[hovering].first.infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + eqItems[hovering].first.buffInfoText.size()) });
			break;
		}
		case Hovering::ITM:
		{
			for (int i = 0; i < itemSlots[hovering].nameInfoText.size(); i++)
			{
				textL = itemSlots[hovering].nameInfoText[i].getString().getSize();
				offset = (((itemSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				itemSlots[hovering].nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = itemSlots[hovering].nameInfoText.size() + 3;
			textL = itemSlots[hovering].slotTypeText.getString().getSize();
			offset = (((itemSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
			itemSlots[hovering].slotTypeText.setPosition(mousePos.x + offset, mousePos.y + ((nSize - 2)*tSize));
			for (int i = 0; i < itemSlots[hovering].buffInfoText.size(); i++)
			{
				textL = itemSlots[hovering].buffInfoText[i].getString().getSize();
				offset = (((itemSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				itemSlots[hovering].buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			itemSlots[hovering].infoBack.setPosition(mousePos);
			itemSlots[hovering].infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + itemSlots[hovering].buffInfoText.size()) });
			break;
		}
		case Hovering::EQS:
		{
			for (int i = 0; i < eqScrolls[hovering].first.nameInfoText.size(); i++)
			{
				textL = eqScrolls[hovering].first.nameInfoText[i].getString().getSize();
				offset = (((eqScrolls[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				eqScrolls[hovering].first.nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = eqScrolls[hovering].first.nameInfoText.size() + 3;
			textL = eqScrolls[hovering].first.slotTypeText.getString().getSize();
			offset = (((eqScrolls[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
			eqScrolls[hovering].first.slotTypeText.setPosition(mousePos.x + offset, mousePos.y + ((nSize - 2)*tSize));
			for (int i = 0; i < eqScrolls[hovering].first.buffInfoText.size(); i++)
			{
				textL = eqScrolls[hovering].first.buffInfoText[i].getString().getSize();
				offset = (((eqScrolls[hovering].first.maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				eqScrolls[hovering].first.buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			eqScrolls[hovering].first.infoBack.setPosition(mousePos);
			eqScrolls[hovering].first.infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + eqScrolls[hovering].first.buffInfoText.size()) });
			break;
		}
		case Hovering::SCR:
		{
			for (int i = 0; i < scrollSlots[hovering].nameInfoText.size(); i++)
			{
				textL = scrollSlots[hovering].nameInfoText[i].getString().getSize();
				offset = (((scrollSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				scrollSlots[hovering].nameInfoText[i].setPosition(mousePos.x + offset, mousePos.y + (i*tSize));
			}
			int nSize = scrollSlots[hovering].nameInfoText.size() + 3;
			textL = scrollSlots[hovering].slotTypeText.getString().getSize();
			offset = (((scrollSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
			scrollSlots[hovering].slotTypeText.setPosition(mousePos.x + offset, mousePos.y + ((nSize - 2)*tSize));
			for (int i = 0; i < scrollSlots[hovering].buffInfoText.size(); i++)
			{
				textL = scrollSlots[hovering].buffInfoText[i].getString().getSize();
				offset = (((scrollSlots[hovering].maxInfoChar + 1) - textL) / 2.f)*(charWidth);
				scrollSlots[hovering].buffInfoText[i].setPosition(mousePos.x + offset, mousePos.y + ((i + nSize)*tSize));
			}
			itemInfoBack.setPosition(mousePos);
			scrollSlots[hovering].infoBack.setPosition(mousePos);
			scrollSlots[hovering].infoBack.setSize({ infoWidth, (tSize + 2) * ((float)nSize + scrollSlots[hovering].buffInfoText.size()) });
			break;
		}
		default:
			break;
		}
	}
}
