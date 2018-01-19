#ifndef GUI_H
#define GUI_H

#include "Game.h"

enum EType { BUTTON };

class GuiElement {

public:
	sf::Sprite sprite;
	sf::Vector2f size;
	sf::Vector2f pos;
	sf::Text text;
	std::string msg;
	std::string id;
	EType type;
	int eVariant;
	AnimationHandler animHandler;

	void setPosition(sf::Vector2f position) { this->pos = position; }

	std::string activate() { return msg; }

	void draw(sf::RenderWindow& window, float dt);
	void update(const float dt);
	void onHover();

	GuiElement(sf::Texture& texture,
		const unsigned int height,
		const sf::Vector2f size,
		const std::vector<Animation>& animations,
		const EType type,
		const std::string& msg,
		const std::string& id)
	{
		this->type = type;
		this->msg = msg;
		this->id = id;
		this->eVariant = 0;

		animHandler.changeAnim(this->eVariant);

		this->sprite.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
		this->sprite.setTexture(texture);
		this->animHandler.frameSize = sf::IntRect(0, 0, size.x, size.y*height);
		for (auto animation : animations)
		{
			this->animHandler.addAnim(animation);
		}
		this->animHandler.update(0.0f);
	}
};

class GuiStyle {
public:
	sf::Color textCol;
	sf::Color textHighlightCol;
	sf::Font* font;
	bool centered;
	
	GuiStyle() {};

	GuiStyle(sf::Color textCol, sf::Color textHighlightCol, sf::Font* font, bool centered) {
		this->textCol = textCol;
		this->textHighlightCol = textHighlightCol;
		this->font = font;
		this->centered = centered;
	}
	~GuiStyle() {}
};

class Gui
{
public:
	GuiStyle style;
	bool centeredText;
	bool horizontal;

	std::vector<GuiElement> elements;
	GuiElement getEntry(const sf::Vector2f mousePos);

	void draw(sf::RenderWindow& window, float dt);
	void update(float dt);

	Gui() {}
	Gui(GuiStyle style, bool horizontal,
		std::vector<sf::Texture> eTextures,
		std::vector<std::vector<Animation>> eAnims,
		std::vector<std::pair<std::string, std::string>> eMsgs,
		sf::Vector2f eSize,
		EType eType,
		int padding) 
	{
		this->style = style;
		this->horizontal = horizontal;
		for (size_t e = 0; e < eMsgs.size(); ++e)
		{
			sf::Text text;
			text.setString(eMsgs[e].first);
			text.setFont(*style.font);
			text.setFillColor(style.textCol);
			text.setCharacterSize(eSize.y - padding);
			elements.push_back(GuiElement(eTextures[e], 1, eSize, eAnims[e], eType, eMsgs[e].first, eMsgs[e].second));
		}
	}
	~Gui();
};

#endif /* GUI_H */