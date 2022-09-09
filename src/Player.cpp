//
//  Player.cpp
//  Breakout
//
//  Created by Austin Horn on 9/6/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#include "Player.hpp"

Player_Properties::Player::Player() {
    initTexture();
    initSprite();
}

void Player_Properties::Player::initTexture() {
    if (!m_texture.loadFromFile(s_filepath))
        throw std::runtime_error("Unable to load image " + s_filepath);
    m_texture.setSmooth(true);
}

void Player_Properties::Player::initSprite() {
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(0.25f, 0.25f);
}

void Player_Properties::Player::setPosition(const sf::Vector2<float>& position) {
    this->m_position = position;
}

sf::Vector2<float> Player_Properties::Player::getPosition() const {
    return this->m_position;
}

void Player_Properties::Player::move() {
    this->m_sprite.setPosition(this->m_position);
}

void Player_Properties::Player::update(const sf::Vector2<float>& position) {
    setPosition(position);
}

void Player_Properties::Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite);
}

