#include "IntroState.hpp"
#include "PlayState.hpp"
#include "StateMachine.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <memory>

IntroState::IntroState(StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{ machine, window, replace }
, m_buttons{(MenuButtons::NewGame), (MenuButtons::LoadGame),(MenuButtons::Controls), (MenuButtons::Quit)}
{
	if (!m_backgroundTexture.loadFromFile("./assets/backgrounds/purple.png")) {
		throw std::runtime_error{ "Unable to load image './assets/backgrounds/purple.png'" };
	}

	m_background.setTexture(m_backgroundTexture, true);
     
    Game_Text::loadGameFont();
    
    Game_Text::initTitleText();
        
	std::cout << "IntroState Init\n";
}

void IntroState::pause()
{
	std::cout << "IntroState Pause\n";
}

void IntroState::resume()
{
	std::cout << "IntroState Resume\n";
}

void IntroState::update()
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
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Space:
                        
                        break;

                    case sf::Keyboard::Escape:
                        m_machine.quit();
                        break;

					default:
						break;
                }
                break;
            }
                
            case sf::Event::MouseButtonPressed:
            {
                switch (event.mouseButton.button)
                {
                    case sf::Mouse::Left:
                        //std::cout << "* Left mouse button clicked *" << std::endl;
                        
                        for (int i = 0; i < 4; i++) {
                            // Update button state as clicked (true)
                            this->m_buttons[ i ].update(mousePos, true);
                            
                            if (this->m_buttons[ i ].getClicked()) {
                                switch (this->m_buttons[ i ].getButton()) {
                                    case UI::Button_Properties::MenuButtons::NewGame:
                                        /// Build play state
                                        m_next = StateMachine::build<PlayState>(m_machine, m_window, true);
                                        break;
                                    case UI::Button_Properties::MenuButtons::LoadGame:
                                        /// Load save game file
                                        
                                        break;
                                    case UI::Button_Properties::MenuButtons::Controls:
                                        /// Temporarily replace state with controls screen
                                        
                                        break;
                                    case UI::Button_Properties::MenuButtons::Quit:
                                        /// Terminate state machine and shutdown
                                        m_machine.quit();
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        
                        break;
                        
                    case sf::Mouse::Right:
                        //std::cout << "* Right mouse button clicked *" << std::endl;
                        break;
                        
                    default:
                        break;
                }
                break;
            }
                
			default:
				break;
		}
	}
    
    for (int i = 0; i < 4; i++) {
        this->m_buttons[ i ].update(mousePos);
    }
    
}

void IntroState::draw()
{
	// Clear the previous drawing
	m_window.clear();
    

    m_window.draw(m_background);
        
    Game_Text::drawTitleText(this->m_window);
    
    for (int i = 0; i < 4; i++) {
        m_window.draw(this->m_buttons[ i ]);
    }
    
	m_window.display();
}
