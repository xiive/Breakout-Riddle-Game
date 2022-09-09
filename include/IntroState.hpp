#ifndef _INTRO_STATE__HPP
#define _INTRO_STATE__HPP

#include "State.hpp"
#include "UI.hpp"
#include "Game.hpp"

using namespace UI::Button_Properties;
using namespace Game_Text;
using namespace UI::Cursor;

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class StateMachine;

namespace sf
{
	class RenderWindow;
}

class IntroState final
: public State {
public:
	IntroState(StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause() override;
	void resume() override;

	void update() override;
	void draw() override;

private:
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
    
    MenuButton m_buttons [ 4 ];
};

#endif // _INTRO_STATE__HPP //
