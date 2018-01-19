#include "Gui.h"

Gui::~Gui()
{
}

void GuiElement::draw(sf::RenderWindow& window, float dt)
{
	this->animHandler.changeAnim(this->eVariant);

	this->animHandler.update(dt);

	this->sprite.setTextureRect(this->animHandler.bounds);

	window.draw(this->sprite);

	return;
}

void GuiElement::update(const float dt)
{
}

void GuiElement::onHover()
{
}
