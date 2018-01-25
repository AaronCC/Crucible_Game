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
}

void Hud::update(float dt)
{
}

void Hud::updateHealth(float percent)
{
	sf::IntRect hRect = elements[H_GLOBE].getTextureRect();
	int newH = (1.f - percent)*(float)hRect.height;
	elements[H_POOL].setTextureRect({ 0, newH,hRect.width,hRect.height });
	sf::Vector2f pos = elements[H_GLOBE].getPosition();
	elements[H_POOL].setPosition(pos.x, pos.y + newH);
}

Hud::~Hud()
{
}