#ifndef _PAUSE_STATE__HPP
#define _PAUSE_STATE__HPP

#include "State.hpp"
#include "UI.hpp"

using namespace UI::Button_Properties;

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class StateMachine;

namespace sf
{
	class RenderWindow;
}

class PauseState final
: public State {
public:
    PauseState(StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause() override;
	void resume() override;

	void update() override;
	void draw() override;

private:
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
    
    MenuButton m_buttons [ 2 ];
};

#endif // _PAUSE_STATE__HPP //
