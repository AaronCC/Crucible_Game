#include "TestState.h"

TestState::TestState(Game* game)
{
	this->game = game;
	testFont.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	testText.setFont(testFont);
	testText.setPosition(200, 200);
	testText.setString("Crucible Game");
}

TestState::~TestState()
{
}

void TestState::draw(const float dt)
{
	this->game->window.setView(this->view);
	this->game->window.draw(testText);
}

void TestState::update(const float dt)
{
}

void TestState::handleInput()
{
}
