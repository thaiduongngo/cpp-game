#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <ctime>
#include <format>
#include <iostream>
#include "CtrChar.h"
#include "Pipes.h"
#include "Cloud.h"
#include "Clouds.h"
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
    constexpr auto WINDOW_WIDTH = 1024.f;
    constexpr auto WINDOW_HEIGHT = 768.f;
    constexpr auto CHAR_SIZE_HEADER = 35;
    constexpr auto CHAR_SIZE_DETAIL = 25;
    constexpr auto FPS_LIMIT = 60;

    class Game : public sf::RenderWindow
    {
    private:
        const std::set<char> BANNED_CHARS = {'\n', ' '};
        game::character::CtrChar ctrChar;
        game::pipes::Pipes pipes;
        game::cloud::Clouds clouds;
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
        int score = 0;
        float gravity = 981.f; // Acceleration due to gravity (pixels/s^2)
        void gamePlay();
        void render();

    public:
        Game();
        void reset();
        void run();
        // Events handling
        void onClose(const sf::Event &event);
        void onKeyPress(const sf::Event &event);
        void onTextEntered(const sf::Event &event);
        ~Game();
    };
}
