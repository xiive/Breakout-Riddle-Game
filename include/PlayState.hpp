#ifndef _PLAY_STATE__HPP
#define _PLAY_STATE__HPP

#include "State.hpp"
#include "Game.hpp"
#include "UI.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

using namespace Level_Properties;
using namespace Level_One_Resources;
using namespace UI::Button_Properties;

class StateMachine;

namespace sf
{
	class RenderWindow;
}

class PlayState final
: public State {
public:
	PlayState(StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause() override;
	void resume() override;

	void update() override;
	void draw() override;

private:
    sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
    
    LevelOne m_level;
    Game_Properties::Game m_game;
    CursorContent m_cursorContent;
    ActionButton m_buttons [ 2 ];
};

#endif // _PLAY_STATE__HPP //
