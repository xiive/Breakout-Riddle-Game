//
//  Game.hpp
//  Breakout
//
//  Created by Austin Horn on 8/19/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef _GAME__HPP
#define _GAME__HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <iostream>
#include <fstream>

#include "../src/Models.cpp"
#include "Player.hpp"
#include "UI.hpp"

///*****************************************************************************************
/// MARK: Namespace 'Game_Text'
///*****************************************************************************************
namespace Game_Text { /// Contains all in-game-text related properties

/// Stores game font [Zorque]
static sf::Font s_font;
/// Loads game font [Zorque]
static void loadGameFont() {
    if (!s_font.loadFromFile("./assets/fonts/Zorque.otf"))
        throw std::runtime_error("Unable to load font ./assets/fonts/Zorque.otf");
}
/// Stores game title [Breakout Riddle Game] text in Zorque font with a character size of 60
static sf::Text s_titleText("Breakout Riddle Game", s_font, 75);
///
static void initTitleText() { s_titleText.setPosition(173.f, 15.f); }
///
static void drawTitleText(sf::RenderTarget& target) { target.draw(s_titleText); }

} /// End of 'Game_Text' namespace









///*****************************************************************************************
/// MARK: Namespace 'Level_Properties'
///*****************************************************************************************
namespace Level_Properties { /// Properties and resources for game levels

enum class Rooms : unsigned int;


/// Abstract object representing floorplan for level
class Floorplan {
protected:
    const std::string m_filepath;
    const unsigned int m_numRooms;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    
    Floorplan(const std::string& filepath, const unsigned int numRooms) : m_filepath(filepath), m_numRooms(numRooms) {
        /// Load texture for floorplan
        initTexture();
        /// Initialize sprite with loaded texture
        initSprite();
    }
private:
    /// Function that loads in texture for sprite
    void initTexture() {
        if (!this->m_texture.loadFromFile(this->m_filepath))
            throw std::runtime_error("Unable to load image " + this->m_filepath);
        
        this->m_texture.setSmooth(true);
    }
    
    /// Function that assigns the texture that was loaded to the sprite
    void initSprite() {
        this->m_sprite.setTexture(this->m_texture);
        this->m_sprite.setPosition(sf::Vector2<float>(0.f,0.f));
    }
};




/// Abstract object containing puzzle data
class Puzzle {
protected:
    const std::string m_filepath;
    std::vector<std::pair<std::string, std::string>> m_data;
public:
    Puzzle(const std::string& filepath) : m_filepath(filepath) { }
    
    void loadPuzzleData() {
        std::ifstream inFS(Puzzle::m_filepath);
        
        if (!inFS.is_open())
            throw std::runtime_error("Unable to load data file");
        
        while (!inFS.eof()) {
            std::string title, content;
            std::getline(inFS, title);
            std::getline(inFS, content);
            this->m_data.push_back(std::make_pair(title, content));
        }
        
        inFS.close();
    }
    
    virtual std::pair<std::string, std::string> getPuzzle(unsigned int index) const {
        return this->m_data.at(index);
    }
};




/// Abstract object representing basic level structure
class Level
: protected Floorplan
, protected Puzzle {
private:
    const std::string m_floorplanFilepath;
    const unsigned int m_numRooms;
    const std::string m_puzzleFilepath;
    std::vector<Rooms> m_adjRooms;
public:
    Level(const std::string floorplanFilepath, const unsigned int numRooms, const std::string puzzleFilepath)
    : Floorplan(floorplanFilepath, numRooms)
    , Puzzle(puzzleFilepath)
    , m_floorplanFilepath(floorplanFilepath)
    , m_numRooms(numRooms)
    , m_puzzleFilepath(puzzleFilepath) { }
    
    explicit Level(const Level &level)
    : Floorplan(level.getFloorplanFilepath(), level.getNumRooms())
    , Puzzle(level.getPuzzleFilepath())
    , m_floorplanFilepath(level.getFloorplanFilepath())
    , m_numRooms(level.getNumRooms())
    , m_puzzleFilepath(level.getPuzzleFilepath()) { }
        
    const std::string getFloorplanFilepath() const { return this->m_floorplanFilepath; }
    const unsigned int getNumRooms()         const { return this->m_numRooms;          }
    const std::string getPuzzleFilepath()    const { return this->m_puzzleFilepath;    }
    std::vector<Rooms> getAdjRooms()         const { return this->m_adjRooms;          }
    
    /// Template function for assigning adjacent rooms respective to player's current location [denoted by 'room']
    template <std::size_t r, std::size_t c> void setAdjRooms(const unsigned int room, const unsigned int (&arr)[r][c]) {
        /// Empty vector for new data
        m_adjRooms.clear();
        
        /// Read adjacency matrix to determine adjacent rooms
        for (int j = 0; j < 18; j++)
            if (arr[ room ][ j ])
                m_adjRooms.push_back(static_cast<Rooms>(j));
    }
    
    /// Template function for updating level data
    template <std::size_t r, std::size_t c> void update(const unsigned int currRoom, const unsigned int (&adjMatrix)[r][c]) {
        setAdjRooms(currRoom, adjMatrix);
    }
    
};

} /// End of 'Level_Properties' namespace







///*****************************************************************************************
/// MARK: Namespace 'Level_One_Resources'
///*****************************************************************************************
/// All Resources neccesary for gameplay of Level One
namespace Level_One_Resources {

/// Enumerated class representing all accessible rooms within the entire level
enum class Rooms : unsigned int {
    Foyer           =  0,
    Kitchen         =  1,
    LivingRoom      =  2,
    DiningRoom      =  3,
    Library         =  4,
    LaundryRoom     =  5,
    HomeOffice      =  6,
    PrimaryBedroom  =  7,
    PrimaryBathroom =  8,
    KidsBedroom     =  9,
    KidsBathroom    = 10,
    GuestBedroom    = 11,
    GuestBathroom   = 12,
    GameRoom        = 13,
    Garage          = 14,
    WineCellar      = 15,
    NHallway        = 16,
    SHallway        = 17,
    ///*************************************************************************************
    Count           = 18 /// Stays last -- denotes number of rooms withing level
};

///
static const unsigned int s_numRooms = static_cast<const unsigned int>(Rooms::Count);

#define ROWS s_numRooms // Rooms::Count
#define COLS s_numRooms // Rooms::Count

/// Enumerated dead zones where cursor content should be cleared
enum class DeadZones : unsigned int {
    DZ_1 = 0,
    DZ_2 = 1,
    DZ_3 = 2,
    DZ_4 = 3
};

/// Adjacency matrix representing connecting rooms
static const unsigned int adjMatrix [ ROWS ][ COLS ] = {
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  /// Printing method that proves matrix functions correctly:
    {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  // for (int i = 0; i < ROWS; i++)
    {1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1},  //     for (int j = 0; j < COLS; j++)
    {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  //         if (Level_One_Resources::adjMatrix[ i ][ j ])
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},  //             std::cout << roomsTypeMap.at(static_cast<Rooms>(i)) << " leads to "
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},  //                       << roomsTypeMap.at(static_cast<Rooms>(j)) << std::endl;
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0},
    {0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0}
};

///
enum class Actions : unsigned int {
    Move    = 0,
    Examine = 1
};



/// Map each accessible room with its title
static std::map<Rooms, const char *> roomsTypeMap;
/// Map each room to a rectangle representing the dimensions (in pixels)
static std::map<Rooms, sf::IntRect> roomsRectBounds;
/// Map each room to a rectangle representing the dimensions (in pixels)
static std::map<DeadZones, sf::IntRect> deadZones;
/// Function that maps enumerated object with a string for naming purposes
static void initRoomsMap() {
    map_init(roomsTypeMap)
        (Rooms::Foyer,           "foyer")
        (Rooms::Kitchen,         "kitchen")
        (Rooms::LivingRoom,      "living room")
        (Rooms::DiningRoom,      "dining room")
        (Rooms::Library,         "library")
        (Rooms::LaundryRoom,     "laundry room")
        (Rooms::HomeOffice,      "home office")
        (Rooms::PrimaryBedroom,  "primary bedroom")
        (Rooms::PrimaryBathroom, "primary bathroom")
        (Rooms::KidsBedroom,     "kids bedroom")
        (Rooms::KidsBathroom,    "kids bathroom")
        (Rooms::GuestBedroom,    "guest bedroom")
        (Rooms::GuestBathroom,   "guest bathroom")
        (Rooms::GameRoom,        "game room")
        (Rooms::Garage,          "garage")
        (Rooms::WineCellar,      "wine cellar")
        (Rooms::NHallway,        "n. hallway")
        (Rooms::SHallway,        "s. hallway")
    ;
}
/// Function that maps enumerated object with rectangle bounds relative to window
static void initRoomsRectBounds() {
    map_init(roomsRectBounds)
        (Rooms::Foyer,           {{  1, 431}, {139,  32}})
        (Rooms::Kitchen,         {{269, 409}, { 88, 113}})
        (Rooms::LivingRoom,      {{141, 324}, {112, 198}})
        (Rooms::DiningRoom,      {{258, 324}, { 99,  80}})
        (Rooms::Library,         {{137, 596}, {103,  93}})
        (Rooms::LaundryRoom,     {{ 22, 715}, { 99,  58}})
        (Rooms::HomeOffice,      {{ 66, 323}, { 69,  81}})
        (Rooms::PrimaryBedroom,  {{ 66,   6}, {212, 141}})
        (Rooms::PrimaryBathroom, {{283,   6}, { 73, 141}})
        (Rooms::KidsBedroom,     {{ 66, 223}, { 98,  83}})
        (Rooms::KidsBathroom,    {{ 66, 179}, { 98,  39}})
        (Rooms::GuestBedroom,    {{233, 223}, { 98,  83}})
        (Rooms::GuestBathroom,   {{233, 179}, { 98,  39}})
        (Rooms::GameRoom,        {{257, 539}, {100, 150}})
        (Rooms::Garage,          {{ 17, 494}, {105, 210}})
        (Rooms::WineCellar,      {{137, 705}, {220,  68}})
        (Rooms::NHallway,        {{183, 149}, { 32, 176}})
        (Rooms::SHallway,        {{123, 555}, {133,  26}})
    ;
}
/// Function that maps enumerated object with rectangle bounds relative to window
static void initDeadZones() {
    map_init(deadZones)
        (DeadZones::DZ_1, {{  0,   0}, {59,  414}})
        (DeadZones::DZ_2, {{ 59, 155}, {116,  17}})
        (DeadZones::DZ_3, {{223, 155}, {141,  17}})
        (DeadZones::DZ_3, {{339, 172}, { 25, 144}})
    ;
}

/// Path to room buttons texture sheet
static const std::string s_roomsButtonsFilepath = "./assets/levels/level-one/rooms-sheet.png";
static sf::Texture s_rbTexture;
static sf::Sprite s_rbSprite;

/// Path to text box image
static const std::string s_textBoxFilepath = "./assets/levels/level-one/text-box.png";
static sf::Texture s_tbTexture;
static sf::Sprite s_tbSprite;

/// Path to action box image
static const std::string s_actionBoxFilepath = "./assets/levels/level-one/action-box.png";
static sf::Texture s_abTexture;
static sf::Sprite s_abSprite;

/// Path to notepad image
static const std::string s_notepadFilepath = "./assets/levels/level-one/notepad.png";
static sf::Texture s_npTexture;
static sf::Sprite s_npSprite;

/// Path to floorplan image
static const std::string s_floorplanFilepath = "./assets/levels/level-one/floorplan.png";
static sf::Texture s_fpTexture;
static sf::Sprite s_fpSprite;

/// Path to puzzles data file
static const std::string s_puzzleFilepath = "./assets/levels/level-one/puzzles.txt";

/// Transparent box representing bounds for text box
static sf::RectangleShape s_textBox;
/// Transparent box representing bounds for action box
static sf::RectangleShape s_actionBox;
/// Transparent box representing bounds for notepad box
static sf::RectangleShape s_notepadBox;

///
static void initRoomButtons() {
    ///
    if (!s_rbTexture.loadFromFile(s_roomsButtonsFilepath))
        throw std::runtime_error("Unable to load " + s_roomsButtonsFilepath);
    
    s_rbSprite.setTexture(s_rbTexture);
}

///
static void initTextBox() {
    ///
    s_textBox.setSize(sf::Vector2f(585.f, 390.f));
    s_textBox.setPosition(sf::Vector2f(365.f, 390.f));
    s_textBox.setFillColor(sf::Color(0,0,0,0));
    if (!s_tbTexture.loadFromFile(s_textBoxFilepath))
        throw std::runtime_error("Unable to load image file " + s_textBoxFilepath);
    
    s_tbSprite.setTexture(s_tbTexture);
    s_tbSprite.setPosition(sf::Vector2f(365.f, 390.f));
}

///
static void initActionBox() {
    s_actionBox.setSize(sf::Vector2f(300.f, 390.f));
    s_actionBox.setPosition(sf::Vector2f(950.f, 390.f));
    s_actionBox.setFillColor(sf::Color(0,0,0,0));
    if (!s_abTexture.loadFromFile(s_actionBoxFilepath))
        throw std::runtime_error("Unable to load image file " + s_actionBoxFilepath);
    
    s_abSprite.setTexture(s_abTexture);
    s_abSprite.setPosition(sf::Vector2f(950.f, 390.f));
}

///
static void initNotepadBox() {
    s_notepadBox.setSize(sf::Vector2f(885.f, 390.f));
    s_notepadBox.setPosition(sf::Vector2f(365.f, 0.f));
    s_notepadBox.setFillColor(sf::Color(0,0,0,0));
    if (!s_npTexture.loadFromFile(s_notepadFilepath))
        throw std::runtime_error("Unable to load image file " + s_notepadFilepath);
    
    s_npSprite.setTexture(s_npTexture);
    s_npSprite.setPosition(sf::Vector2f(365.f, 0.f));
}






///
class RoomButton
: public UI::Button_Properties::UIButton<Rooms>
, public sf::Drawable {
public:
    RoomButton(const Rooms button) : UIButton<Rooms>(button, {0,60 * static_cast<int>(button),300,60}) { }
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_sprite);
    }
};






///
class ActionButton
: public UI::Button_Properties::UIButton<Actions>
, public sf::Drawable {
public:
    ActionButton(const Actions button) : UIButton<Actions>(button, {0,60 * (static_cast<int>(button)+6),300,60}) {
        ActionButton::init();
        ActionButton::initTexture();
        ActionButton::initSprite();
    }
    
    void init() override {
        UI::Button_Properties::initGameButtons();
    }
    
    void initTexture() override {
        this->m_texture = UI::Button_Properties::s_gbTexture;
    }
    
    void initSprite() override {
        this->m_sprite.setTextureRect(this->m_rect);
        this->m_sprite.setTexture(this->m_texture);
        this->m_position = sf::Vector2<float>(950.f, 395.f + (70.f * static_cast<int>(this->m_button)));
        this->m_sprite.setPosition(this->m_position);
    }
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        if (!getClicked())
            target.draw(m_sprite);
    }
};






///
class LevelOne
: public Level_Properties::Level
, public sf::Drawable {
public:
    LevelOne()
    : Level_Properties::Level(s_floorplanFilepath, s_numRooms, s_puzzleFilepath) {
        /// Initialize and create rooms map
        initRoomsMap();
        ///
        initRoomsRectBounds();
        ///
        initRoomButtons();
        ///
        initDeadZones();
        ///
        initTextBox();
        ///
        initActionBox();
        ///
        initNotepadBox();
        ///
        loadPuzzleData();
    }
    
    std::pair<std::string, std::string> getPuzzle(unsigned int index) const override {
        return this->Puzzle::m_data.at(index);
    }
    

    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        /// Draws floorplan to target
        target.draw(Floorplan::m_sprite);
        /// Draws text box to target
        target.draw(s_tbSprite);
        /// Draws action nox to target
        target.draw(s_abSprite);
        /// Draws notepad box to target
        target.draw(s_npSprite);
    }
};
}
















namespace Game_Properties {



///
class Game
: public Level_Properties::Level
, public sf::Drawable {
private:
    Player_Properties::Player m_player;
    Level_Properties::Level m_level;
public:
    Game(const Level_Properties::Level& level) : Level(level), m_level(level) { }
    
    Player_Properties::Player getPlayer() const { return this->m_player; }
    
    void update(const sf::Vector2<int>& position) {
        
    }
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_player);
    }
};




} /// End of 'Game_Properties' namespace

#endif // _GAME__HPP //
