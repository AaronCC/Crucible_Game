#include "MenuState.h"


MenuState::MenuState(Game* game)
{
	this->game = game;
	testFont.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	testText.setFont(testFont);
	testText.setPosition(200, 200);
	testText.setString("Crucible Game");
	initView();
}


MenuState::~MenuState()
{
}

void MenuState::draw(const float dt)
{

}

void MenuState::update(const float dt)
{
}

void MenuState::handleInput()
{
}
