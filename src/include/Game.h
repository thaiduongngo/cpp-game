#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <ctime>
#include <format>
#include <iostream>
#include <set>
#include "CtrChar.h"
#include "Pipes.h"
#include "Clouds.h"
#include "Mounts.h"
#include "Leaderboard.h"
#include "GameState.h"

#ifndef TEXT_COLOR
#define TEXT_COLOR sf::Color::Yellow
#endif

#ifndef TEXT_OUTLINE_COLOR
#define TEXT_OUTLINE_COLOR sf::Color::Red
#endif

namespace game
{
    constexpr std::string GAME_TITLE = "Dragon Quest";
    constexpr u_int WINDOW_WIDTH = 1024;
    constexpr u_int WINDOW_HEIGHT = 768;
    constexpr u_int CHAR_SIZE_HEADER = 35;
    constexpr u_int CHAR_SIZE_DETAIL = 25;
    constexpr u_int FPS_LIMIT = 60;
    constexpr std::string FONT_FILE = "./res/FiraCode.ttf";
    constexpr std::string CHAR_IMAGE = "./res/dragon.png";
    constexpr std::string FLAPPING_SOUND = "./res/dragon.mp3";
    constexpr std::string GAMEOVER_SOUND = "./res/gameover.mp3";

    class Game : public sf::RenderWindow
    {
    private:
        const std::set<char> BANNED_CHARS = {'\n', ' '};
        sf::Texture charTexture_;
        game::character::CtrChar ctrChar;
        game::pipes::Pipes pipes;
        game::cloud::Clouds clouds;
        game::mount::Mounts mounts;
        game::lb::Leaderboard leaderboard;

        sf::Font font;
        sf::Text scoreText;
        sf::Text startText;
        sf::Text gameOverText;
        sf::Text leaderboardText;
        sf::Text highScore;
        sf::Text playerName;
        sf::String playerString;
        sf::RectangleShape panelLeaderboard;
        sf::Clock clock;
        sf::SoundBuffer flappingSoundBuffer;
        sf::Sound flappingSound;
        sf::SoundBuffer gameOverSoundBuffer;
        sf::Sound gameOverSound;
        GameState gameState = GameState::NOT_STARTED;
        u_int score = 0;
        float gravity = 981.f; // Acceleration due to gravity (pixels/s^2)
        void gamePlay();
        void render();

    public:
        Game();
        void reset();
        void run();
        // Events handling
        void onClose(const std::optional<sf::Event> &event);
        void onKeyPress(const std::optional<sf::Event> &event);
        void onTextEntered(const std::optional<sf::Event> &event);
        ~Game();
    };
}
