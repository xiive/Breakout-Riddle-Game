#include "StateMachine.hpp"
#include "PauseState.hpp"
#include "IntroState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

PauseState::PauseState(StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{ machine, window, replace }
, m_buttons{ (MenuButtons::Resume), (MenuButtons::Exit) }
{
    if (!m_backgroundTexture.loadFromFile("./assets/backgrounds/red.png")) {
        throw std::runtime_error{ "Was unable to load image './assets/background/red.png'" };
	}

	m_background.setTexture(m_backgroundTexture, true);

	std::cout << "MenuState Init\n";
}

void PauseState::pause()
{
	std::cout << "MenuState Pause\n";
}

void PauseState::resume()
{
	std::cout << "MenuState Resume\n";
}

void PauseState::update()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    
	for (auto event = sf::Event{}; m_window.pollEvent(event);)
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				m_machine.quit();
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::Tab:
						m_machine.lastState();
						break;
                        
                    case sf::Keyboard::Key::Escape:
                        m_next = StateMachine::build<IntroState>(m_machine, m_window, true);
                        break;

					default:
						break;
				}
				break;

            case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        for (int i = 0; i < 2; i++) {
                            this->m_buttons[ i ].update(mousePos, true);
                            
                            if (this->m_buttons[ i ].getClicked()) {
                                switch (this->m_buttons[ i ].getButton()) {
                                    case UI::Button_Properties::MenuButtons::Resume:
                                        ///
                                        m_machine.lastState();
                                        break;
                                    case UI::Button_Properties::MenuButtons::Exit:
                                        ///
                                        m_next = StateMachine::build<IntroState>(m_machine, m_window, true);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        break;
                    case sf::Mouse::Right:
                        
                        break;
                        
                    default:
                        break;
                }
                break;
                
			default:
				break;
		}
	}
    
    for (int i = 0; i < 2; i++) {
        this->m_buttons[ i ].update(mousePos);
    }
}

void PauseState::draw()
{
	// Clear the previous drawing
	m_window.clear();
    
	m_window.draw(m_background);
    
    for (int i = 0; i < 2; i++) {
        m_window.draw(this->m_buttons[ i ]);
    }
    
	m_window.display();
}
