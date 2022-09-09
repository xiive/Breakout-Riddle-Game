#include <iostream>
#include <memory>
#include <stdexcept>

#include "StateMachine.hpp"
#include "IntroState.hpp"
#include "PlayState.hpp"
#include "PauseState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

PlayState::PlayState(StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State{ machine, window, replace }
, m_buttons{(Actions::Move), (Actions::Examine)}
, m_game(m_level)
{
    if (!m_backgroundTexture.loadFromFile("./assets/backgrounds/green.png")) {
        throw std::runtime_error{ "Was unable to load image 'assets/backgrounds/green.png'" };
    }
    
	m_background.setTexture(m_backgroundTexture, true);
    
    sf::IntRect rect = roomsRectBounds.at(static_cast<Level_One_Resources::Rooms>(static_cast<int>(Level_One_Resources::Rooms::LivingRoom)));
    float x = rect.width / 2.f;
    float y = rect.height / 2.f;
    const sf::Vector2<float> pos = sf::Vector2<float>(rect.left+x, rect.top+y);
    m_game.getPlayer().setPosition(pos);
    m_game.getPlayer().move();
    
    std::cout << pos.x << ", " << pos.y << std::endl;
    /// Update player's room location and fetch the corresponding adjacent rooms
    m_game.Level_Properties::Level::update(static_cast<const unsigned int>(Level_One_Resources::Rooms::LivingRoom), Level_One_Resources::adjMatrix);
    
    for (auto adjRoom : m_game.getAdjRooms())
        std::cout << Level_One_Resources::roomsTypeMap.at(static_cast<Level_One_Resources::Rooms>(static_cast<int>(adjRoom))) << std::endl;
    
	std::cout << "PlayState Init\n";
}

void PlayState::pause()
{
	std::cout << "PlayState Pause\n";
}

void PlayState::resume()
{
	std::cout << "PlayState Resume\n";
}

void PlayState::update()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    
	for (auto event = sf::Event{}; m_window.pollEvent(event);)
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				m_machine.quit();
				break;
                
            case sf::Event::TextEntered:
                break;

			case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Tab:
                        m_next = StateMachine::build<PauseState>(m_machine, m_window, false);
                        break;

                    case sf::Keyboard::Escape:
                        m_machine.quit();
                        break;

                    default:
                        break;
                }
                break;
                break;
                
            case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left:
                        for (int i = 0; i < 2; i++)
                            if (m_buttons[ i ].contains(mousePos))
                                m_buttons[ i ].setClicked(true);
                           
                                
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
    
    
    
    for (int i = 0; i < 2; i++)
        this->m_buttons[ i ].update(mousePos);
    
    
    for (int i = 0; i < 2; i++)
        if (this->m_buttons[ i ].getClicked())
            switch (this->m_buttons[ i ].getButton())
            {
                case Level_One_Resources::Actions::Move:
                    for (auto adjRoom : m_level.getAdjRooms())
                        std::cout << static_cast<int>(adjRoom) << std::endl;
                    break;
                case Level_One_Resources::Actions::Examine:
                    
                    break;
            }
    
    this->m_cursorContent.update(mousePos);
    
    for (auto room : roomsRectBounds)
        if (room.second.contains(mousePos))
            this->m_cursorContent.setString(roomsTypeMap.at(room.first));
  
    for (auto dz : deadZones)
        if (dz.second.contains(mousePos)  ||  mousePos.x >= 365)
            this->m_cursorContent.setString("");
    
    
}

void PlayState::draw()
{
	// Clear the previous drawing
	m_window.clear();
    
	m_window.draw(m_background);
    
    m_window.draw(m_level);
    
    m_window.draw(m_game.getPlayer());
    
    for (int i = 0; i < 2; i++)
        m_window.draw(this->m_buttons[ i ]);
    
    m_window.draw(m_cursorContent);
    
	m_window.display();
}
