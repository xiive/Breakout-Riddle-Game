#ifndef _APPLICATION__HPP
#define _APPLICATION__HPP

#include "StateMachine.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/String.hpp>

class Application {
private:
    StateMachine m_machine;
    sf::RenderWindow m_window;
public:
    void run();
};

#endif // _APPLICATION__HPP //
