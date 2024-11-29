#pragma once
#ifndef _GAME_H_
#define _GAME_H_

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

constexpr std::string GAME_TITLE = "Dragon Quest";
constexpr auto WINDOW_WIDTH = 1024.f;
constexpr auto WINDOW_HEIGHT = 768.f;
constexpr auto FONT_SIZE = 35;
constexpr auto FPS_LIMIT = 60;
constexpr auto PIPE_SPAWN_INTERVAL = 1.75f;
constexpr auto CLOUD_SPAWN_INTERVAL = 1.5f;

#ifndef TEXT_COLOR
#define TEXT_COLOR sf::Color::Yellow
#endif

#ifndef TEXT_OUTLINE_COLOR
#define TEXT_OUTLINE_COLOR sf::Color::Red
#endif

class Game : public sf::RenderWindow
{
private:
    CtrChar ctrChar;
    Pipes pipes;
    Clouds clouds;
    Leaderboard leaderboard;

    sf::Font font;
    sf::Text scoreText;
    sf::Text startText;
    sf::Text gameOverText;
    sf::Text leaderboardText;
    sf::RectangleShape panelLeaderboard;
    sf::Clock clock;
    sf::SoundBuffer flappingSoundBuffer;
    sf::Sound flappingSound;

    int score = 0;
    bool gameStarted = false;
    bool gameOver = false;
    float gravity = 981.f; // Acceleration due to gravity (pixels/s^2)
    float pipeSpawnTimer = 0.f;
    float cloudSpawnTimer = 0.f;
    void gamePlay();
    void render();

public:
    Game();
    void reset();
    void run();
    // Events handling
    void onClose(const sf::Event &event);
    void onKeyPress(const sf::Event &event);
    ~Game();
};

Game::Game() : RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE),
               ctrChar(),
               pipes(),
               clouds(),
               leaderboard()
{
    this->setFramerateLimit(FPS_LIMIT);
    // @todo handle error here
    font.loadFromFile("./res/FiraCode.ttf");
    flappingSoundBuffer.loadFromFile("./res/dragon.mp3");
    flappingSound.setBuffer(flappingSoundBuffer);
    // Score text
    scoreText.setString(std::format("Score: {}", score));
    scoreText.setFont(font);
    scoreText.setCharacterSize(FONT_SIZE);
    scoreText.setFillColor(TEXT_COLOR);
    scoreText.setOutlineThickness(1.f);
    scoreText.setOutlineColor(TEXT_OUTLINE_COLOR);
    scoreText.setPosition(sf::Vector2f(10.f, 0.f));
    // Start text
    startText.setString("Press Enter to Start");
    startText.setFont(font);
    startText.setCharacterSize(FONT_SIZE);
    startText.setFillColor(TEXT_COLOR);
    startText.setOutlineThickness(1.f);
    startText.setOutlineColor(TEXT_OUTLINE_COLOR);
    startText.setOrigin(startText.getLocalBounds().width / 2, startText.getLocalBounds().height / 2);
    startText.setPosition(WINDOW_WIDTH / 2, 250);
    // Gameover text
    gameOverText.setString("Game Over!");
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(FONT_SIZE);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineThickness(1.f);
    gameOverText.setOutlineColor(sf::Color::Yellow);
    gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
    gameOverText.setPosition(WINDOW_WIDTH / 2, 300);
    // Leaderboard
    panelLeaderboard = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.42));
    panelLeaderboard.setOutlineThickness(1.f);
    panelLeaderboard.setOutlineColor(sf::Color::Yellow);
    panelLeaderboard.setFillColor(sf::Color::Black);
    panelLeaderboard.setPosition(WINDOW_WIDTH * 0.1, 350);
    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(FONT_SIZE);
    leaderboardText.setFillColor(sf::Color::Yellow);
    leaderboardText.setOutlineThickness(1.f);
    leaderboardText.setOutlineColor(sf::Color::Black);
    leaderboardText.setOrigin(leaderboardText.getLocalBounds().width / 2, leaderboardText.getLocalBounds().height / 2);
    leaderboardText.setPosition(WINDOW_WIDTH * 0.2, 400);
    reset();
};

void Game::reset()
{
    score = 0;
    pipeSpawnTimer = 0;
    cloudSpawnTimer = 0;
    ctrChar.reset();
    scoreText.setString(std::format("Score: {}", score));
    pipes.reset();
    clouds.reset();
}

void Game::gamePlay()
{
    float deltaTime = clock.restart().asSeconds();

    if (gameStarted && !gameOver) // Game is playing
    {
        // Bird movement
        ctrChar.moveAndFall(gravity, deltaTime);

        // Cloud spawning
        cloudSpawnTimer += deltaTime;
        if (cloudSpawnTimer >= CLOUD_SPAWN_INTERVAL)
        {
            cloudSpawnTimer = 0;
            clouds.spawnCloud(WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT));
        }

        // Move clouds
        clouds.moveCloud(deltaTime);
        clouds.eraseOffScreenCloud();

        // Pipe spawning
        pipeSpawnTimer += deltaTime;
        if (pipeSpawnTimer >= PIPE_SPAWN_INTERVAL)
        {
            pipeSpawnTimer = 0;
            pipes.spawnPipe(WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT * 2 / 3));
        }

        // Move pipes
        pipes.movePipes(deltaTime);

        // Window edges collision detection
        gameOver = ctrChar.collidedWEdge(WINDOW_HEIGHT, 0);

        const auto &pipes_ = pipes.getPipes();
        for (int i = 0; i < pipes_.size(); i++)
        {
            // Pipe collision detection
            if (!gameOver)
            {
                gameOver = ctrChar.collidedWPipe(pipes_[i]);
            }

            // Score increment
            if (!gameOver && ctrChar.passedPipe(pipes_[i], pipes.getPipeSpeed(), deltaTime))
            {
                score++;
                scoreText.setString(std::format("Score: {}", score));
            }

            pipes.eraseOffScreenPipe(i);
        }

        if (gameOver)
        {
            gameStarted = false;
            leaderboard.addEntry(score);
            leaderboard.saveToFile();
            leaderboardText.setString(leaderboard.getLeaderboard());
        }
    }
}

void Game::render()
{
    clear();
    const sf::Vertex wnd_background[4] = {
        sf::Vertex(sf::Vector2f(0, 0), sf::Color::Cyan),
        sf::Vertex(sf::Vector2f(0, WINDOW_HEIGHT), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(WINDOW_WIDTH, 0), sf::Color::Cyan),
    };

    draw(wnd_background, 4, sf::PrimitiveType::Quads);

    for (const auto &cloud : clouds.getClouds())
    {
        draw(cloud);
    }

    for (const auto &pipePair : pipes.getPipes())
    {
        draw((pipePair).first);
        draw((pipePair).second);
    }

    draw(ctrChar);
    draw(scoreText);

    if (!gameStarted)
    {
        draw(startText);
    }

    if (gameOver)
    {
        draw(gameOverText);
        draw(panelLeaderboard);
        draw(leaderboardText);
    }

    display();
}

void Game::run()
{
    while (isOpen())
    {
        // Events handling
        sf::Event event;
        while (pollEvent(event))
        {
            onClose(event);
            onKeyPress(event);
        }

        // Game logic
        gamePlay();

        // Render components
        render();
    }
}

void Game::onClose(const sf::Event &event)
{
    if (event.type == sf::Event::Closed)
    {
        close();
    }
}

void Game::onKeyPress(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Space:
            if (gameStarted)
            {
                // WHile game playing
                ctrChar.jump();
                flappingSound.play();
            }
            break;
        case sf::Keyboard::Enter:
            if (!gameStarted)
            {
                // Restart a new game
                gameOver = false;
                gameStarted = true;
                reset();
            }
            break;
        default:
            break;
        }
    }
}

Game::~Game() {}
#endif
