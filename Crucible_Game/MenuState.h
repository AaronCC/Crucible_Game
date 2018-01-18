#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "Gui.h"

class MenuState : public GameState
{
public:
	MenuState(Game* game);
	~MenuState();	
	
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

};

#endif /* MENU_STATE_H*/
