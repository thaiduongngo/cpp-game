#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <format>
#include <iostream>
#include "CtrChar.h"
#include "Pipes.h"
#include "Cloud.h"
#include "Clouds.h"

#ifndef GAME_TITLE
#define GAME_TITLE "Dragon Quest"
#endif

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1024
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 768
#endif

#ifndef FONT_SIZE
#define FONT_SIZE 35
#endif

#ifndef FPS_LIMIT
#define FPS_LIMIT 60
#endif

#ifndef PIPE_SPAWN_INTERVAL
#define PIPE_SPAWN_INTERVAL 1.6f
#endif

#ifndef CLOUD_SPAWN_INTERVAL
#define CLOUD_SPAWN_INTERVAL 1.5f
#endif

#ifndef TEXT_COLOR
#define TEXT_COLOR sf::Color::Yellow
#endif

#ifndef TEXT_OUTLINE_COLOR
#define TEXT_OUTLINE_COLOR sf::Color::Red
#endif

class Game
{
private:
    sf::RenderWindow window;
    CtrChar ctrChar;
    Pipes pipes;
    Clouds clouds;
    sf::Font font;
    sf::Text scoreText;
    sf::Text startText;
    sf::Text gameOverText;
    sf::Clock clock;
    sf::Vertex background[4] = {
        sf::Vertex(sf::Vector2f(0, 0), sf::Color::Cyan),
        sf::Vertex(sf::Vector2f(0, WINDOW_HEIGHT), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(WINDOW_WIDTH, 0), sf::Color::Cyan),
    };
    sf::SoundBuffer flappingSoundBuffer;
    sf::Sound flappingSound;

    int score = 0;
    bool gameStarted = false;
    bool gameOver = false;
    float gravity = 981.f; // Acceleration due to gravity (pixels/s^2)
    float pipeSpawnTimer = 0.f;
    float cloudSpawnTimer = 0.f;
    void render();

public:
    Game();
    void reset();
    void run();
    ~Game();
};

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE),
               ctrChar(),
               pipes(),
               clouds(),
               font(),
               scoreText(),
               startText(),
               gameOverText()
{
    window.setFramerateLimit(FPS_LIMIT);

    ctrChar.setOriginalPosition(sf::Vector2f(100.f, 300.f));
    ctrChar.moveToOriginalPosition();

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
    startText.setPosition(WINDOW_WIDTH / 2, 300);
    // Gameover text
    gameOverText.setString("Game Over!");
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(FONT_SIZE);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineThickness(1.f);
    gameOverText.setOutlineColor(sf::Color::Yellow);
    gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
    gameOverText.setPosition(WINDOW_WIDTH / 2, 400);
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

void Game::render()
{
    window.clear();

    window.draw(background, 4, sf::Quads);

    for (const auto &cloud : (*clouds.getClouds()))
    {
        window.draw(*(cloud));
    }

    for (const auto &pipePair : pipes.getPipes())
    {
        window.draw(pipePair.first);
        window.draw(pipePair.second);
    }

    window.draw(ctrChar);
    window.draw(scoreText);

    if (!gameStarted)
    {
        window.draw(startText);
    }

    if (gameOver)
    {
        window.draw(gameOverText);
    }

    window.display();
}

void Game::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // While pressing Spacebar
            if (event.key.code == sf::Keyboard::Space)
            {
                if (gameStarted)
                {
                    ctrChar.jump();
                    flappingSound.play();
                }
            }

            if (event.key.code == sf::Keyboard::Enter)
            {
                if (!gameStarted)
                {
                    gameOver = false;
                    gameStarted = true;
                    reset();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (gameStarted && !gameOver)
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

            const auto &_clouds = clouds.getClouds();
            for (int i = 0; i < (*_clouds).size(); i++)
            {
                // Remove off-screen clouds
                if (clouds.offScreen((*(*_clouds)[i])))
                {
                    clouds.eraseCloud(i);
                    break;
                }
            }

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

            const auto _pipes = pipes.getPipes();
            for (int i = 0; i < _pipes.size(); i++)
            {
                // Pipe collision detection
                if (!gameOver)
                {
                    gameOver = ctrChar.collidedWPipe(_pipes[i]);
                }

                // Score increment
                if (!gameOver && ctrChar.passedPipe(_pipes[i], pipes.getPipeSpeed(), deltaTime))
                {
                    score++;
                    scoreText.setString(std::format("Score: {}", score));
                }

                // Remove off-screen pipes
                if (pipes.offScreen(_pipes[i]))
                {
                    pipes.erasePipe(i);
                    break;
                }
            }

            if (gameOver)
                gameStarted = false;
        }

        // Rendering
        render();
    }
}

Game::~Game() {};