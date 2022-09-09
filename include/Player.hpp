//
//  Player.hpp
//  Breakout
//
//  Created by Austin Horn on 9/6/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef _PLAYER__HPP
#define _PLAYER__HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>


#include <iostream>
#include <cmath>


//////////////////////////////////////////////////
/// MARK: Namespace 'Player_Properties'
//////////////////////////////////////////////////
namespace Player_Properties { /// Contains player attributes

static std::string s_filepath =  "./assets/icons/pin.png";

class Player
: public sf::Drawable {
private:
    sf::Vector2<float> m_position;
    sf::Vector2<float> m_velocity;
    float m_velocityMax = 4.f;
    float m_acceleration = 2.f;
    
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    
    void initTexture();
    void initSprite();
public:
    Player();
    
    // Mutators
    void setPosition(const sf::Vector2<float>&);
    
    // Accessors
    sf::Vector2<float> getPosition() const;
    
    void move();
    
    // Update Functions
    void update(const sf::Vector2<float>& position);
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

}

#endif /* _PLAYER__HPP */
