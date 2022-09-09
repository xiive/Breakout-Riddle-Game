//
//  UI.hpp
//  Breakout
//
//  Created by Austin Horn on 8/20/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef _UI__HPP
#define _UI__HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////
/// MARK: Namespace 'UI'
///////////////////////////////////////////////////////////////////////////////////
namespace UI { /// User Interface Properties -- Clickable and Interactive Objects --

///////////////////////////////////////////////////////////////////////////////////
/// MARK: Namespace 'Cursor'
///////////////////////////////////////////////////////////////////////////////////
namespace Cursor {

class CursorContent
: public sf::Drawable {
private:
    sf::Vector2<int> m_position;
    sf::Font m_font;
    sf::Text m_text;
public:
    CursorContent() {
        if (!this->m_font.loadFromFile("./assets/fonts/Zorque.otf"))
            throw std::runtime_error("Unable to load font ./assets/fonts/Zorque.otf");
        
        this->m_text = sf::Text("", this->m_font, 30);
    }
    
    //*******************************************************************
    /// Mutators
    void setPosition(const sf::Vector2<int>& position) {
        this->m_position = position;
    }
    void setString(const std::string& str) {
        this->m_text.setString(str);
    }
    
    //*******************************************************************
    // Accessors
    sf::Vector2i getPosition() const {
        return this->m_position;
    }
    std::string getString() const {
        return this->m_text.getString();
    }
    
    //*******************************************************************
    // Update Functions
    void update(const sf::Vector2<int>& mousePos) {
        updatePosition(mousePos);
    }
    void updatePosition(const sf::Vector2<int>& mousePos) {
        const sf::Vector2<float> &position = sf::Vector2<float>(mousePos.x+10, mousePos.y+10);
        this->m_text.setPosition(position);
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(this->m_text);
    }
};
} /// End of 'Cursor' namespace

///////////////////////////////////////////////////////////////////////////////////
/// MARK: Namespace 'Button_Properties'
///////////////////////////////////////////////////////////////////////////////////
namespace Button_Properties { /// Contains properties for clickable buttons




/// Represents buttons available at the Main Menu
enum class MenuButtons : unsigned int {
    NewGame  = 0,
    LoadGame = 1,
    Controls = 2,
    Quit     = 3,
    Resume   = 4,
    Exit     = 5,
};

static const std::string GB_FILEPATH = "./assets/buttons/game-buttons-sheet.png";
static sf::Texture s_gbTexture;

static void initGameButtons() {
    if (!s_gbTexture.loadFromFile(GB_FILEPATH))
        throw std::runtime_error("Unable to load " + GB_FILEPATH);
}





/// Rectangle UI Button Object (Base Button Class)
template <typename T> class UIButton {
protected:
    sf::IntRect m_rect;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2<float> m_position;
    bool m_clicked;
    bool m_mouseover;
    const T m_button;
public:
    UIButton(T type, sf::IntRect rect)
    : m_rect(rect)
    , m_clicked(false)
    , m_button(type) { }
    
    // Pure virtual functions for initialization purposes
    virtual void init() = 0;
    virtual void initTexture() = 0;
    virtual void initSprite() = 0;
    
    // Mutators
    void setClicked(const bool state)                  { this->m_clicked = state;     }
    void setPosition(const sf::Vector2<float>& position) { this->m_position = position; }
    
    // Accessors
    bool getClicked()                               const { return this->m_clicked;                                                   }
    const T getButton()                             const { return this->m_button;                                                    }
    bool contains(const sf::Vector2<int>& mousePos) const { return this->m_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y); }
    
    // Update Functions
    void updateState(const sf::Vector2<int>& mousePos, const bool clicked) {
        if (clicked  &&  this->contains(mousePos))
            setClicked(true);
    }
    
    void updateSprite(const sf::Vector2<int>& mousePos) {
        this->m_rect.left = (this->contains(mousePos)) ? 300 : 0;
        this->m_sprite.setTextureRect(this->m_rect);
    }
    
    void update(const sf::Vector2<int>& mousePos, const bool clicked = false) {
        updateState(mousePos, clicked);
        updateSprite(mousePos);
    }
};






class MenuButton
: public UIButton<MenuButtons>
, public sf::Drawable {
public:
    MenuButton(const MenuButtons button) : UIButton<MenuButtons>(button, {0,60 * static_cast<int>(button),300,60}) {
        
        MenuButton::init();
        MenuButton::initTexture();
        MenuButton::initSprite();
        
        switch (this->m_button) {
            case MenuButtons::NewGame:
            case MenuButtons::LoadGame:
            case MenuButtons::Controls:
            case MenuButtons::Quit:
                this->m_position = sf::Vector2<float>(475.f, 330.f + (70.f * static_cast<int>(this->m_button)));
                break;
            case MenuButtons::Resume:
            case MenuButtons::Exit:
                this->m_position = sf::Vector2<float>(475.f, 330.f + (70.f * (static_cast<int>(this->m_button) - 4)));
                break;
        }
        
        this->m_sprite.setPosition(this->m_position);
    }
    
    void init() override {
        UI::Button_Properties::initGameButtons();
    }
    
    void initTexture() override {
        this->m_texture = s_gbTexture;
    }
    
    void initSprite() override {
        this->m_sprite.setTextureRect(this->m_rect);
        this->m_sprite.setTexture(this->m_texture);
    }
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(this->m_sprite);
    }
};
} // End of Button_Properties








}




#endif /* _UI__HPP */
