#include "Game.h"

namespace game
{
    Game::Game() : RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE),
                   ctrChar(),
                   pipes(),
                   clouds(),
                   leaderboard()
    {
        this->setFramerateLimit(FPS_LIMIT);
        leaderboard.loadFromFile();
        // @todo handle error here
        font.loadFromFile("./res/FiraCode.ttf");
        flappingSoundBuffer.loadFromFile("./res/dragon.mp3");
        flappingSound.setBuffer(flappingSoundBuffer);
        gameOverSoundBuffer.loadFromFile("./res/gameover.mp3");
        gameOverSound.setBuffer(gameOverSoundBuffer);
        // Score text
        scoreText.setString(std::format("Score: {}", score));
        scoreText.setFont(font);
        scoreText.setCharacterSize(CHAR_SIZE_HEADER);
        scoreText.setFillColor(TEXT_COLOR);
        scoreText.setOutlineThickness(1.f);
        scoreText.setOutlineColor(TEXT_OUTLINE_COLOR);
        scoreText.setPosition(sf::Vector2f(10.f, 0.f));
        // Start text
        startText.setString("Press Enter to Start");
        startText.setFont(font);
        startText.setCharacterSize(CHAR_SIZE_HEADER);
        startText.setFillColor(TEXT_COLOR);
        startText.setOutlineThickness(1.f);
        startText.setOutlineColor(TEXT_OUTLINE_COLOR);
        startText.setOrigin(startText.getLocalBounds().width / 2, startText.getLocalBounds().height / 2);
        startText.setPosition(WINDOW_WIDTH / 2, 250);
        // Gameover text
        gameOverText.setString("Game Over!");
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(CHAR_SIZE_HEADER);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineThickness(1.f);
        gameOverText.setOutlineColor(sf::Color::Yellow);
        gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
        gameOverText.setPosition(WINDOW_WIDTH / 2, 300);
        // Leaderboard
        panelLeaderboard = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.45));
        panelLeaderboard.setOutlineThickness(1.f);
        panelLeaderboard.setOutlineColor(sf::Color::Yellow);
        panelLeaderboard.setFillColor(sf::Color::Black);
        panelLeaderboard.setPosition(WINDOW_WIDTH * 0.1, 350);
        leaderboardText.setFont(font);
        leaderboardText.setCharacterSize(CHAR_SIZE_HEADER);
        leaderboardText.setFillColor(sf::Color::Yellow);
        leaderboardText.setOutlineThickness(1.f);
        leaderboardText.setOutlineColor(sf::Color::Black);
        leaderboardText.setOrigin(leaderboardText.getLocalBounds().width / 2, leaderboardText.getLocalBounds().height / 2);
        leaderboardText.setPosition(WINDOW_WIDTH * 0.15, 400);
        // High score
        highScore.setString("High score! Enter your name: ");
        highScore.setFont(font);
        highScore.setCharacterSize(CHAR_SIZE_DETAIL);
        highScore.setFillColor(sf::Color::White);
        highScore.setOutlineThickness(1.f);
        highScore.setOutlineColor(sf::Color::Blue);
        highScore.setOrigin(playerName.getLocalBounds().width / 2, playerName.getLocalBounds().height / 2);
        highScore.setPosition(WINDOW_WIDTH * 0.15, 640);
        // Player name
        playerName.setFont(font);
        playerName.setCharacterSize(CHAR_SIZE_DETAIL);
        playerName.setFillColor(sf::Color::White);
        playerName.setOutlineThickness(1.f);
        playerName.setOutlineColor(sf::Color::Blue);
        playerName.setStyle(sf::Text::Bold);
        playerName.setOrigin(playerName.getLocalBounds().width / 2, playerName.getLocalBounds().height / 2);
        playerName.setPosition(WINDOW_WIDTH * 0.57, 640);
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

        if (gameState == GameState::PLAYING) // Game is playing
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
            if (ctrChar.collidedWEdge(WINDOW_HEIGHT, 0))
            {
                gameState = GameState::GAMEOVER;
            }

            const auto &pipes_ = pipes.getPipes();
            for (int i = 0; i < pipes_.size(); i++)
            {
                // Pipe collision detection
                if (gameState != GameState::GAMEOVER && gameState != GameState::TOP_GAMEOVER)
                {
                    if (ctrChar.collidedWPipe(pipes_[i]))
                    {
                        gameState = GameState::GAMEOVER;
                    }

                    if (ctrChar.passedPipe(pipes_[i], pipes.getPipeSpeed(), deltaTime))
                    {
                        // Score increment
                        score++;
                        scoreText.setString(std::format("Score: {}", score));
                    }
                }

                pipes.eraseOffScreenPipe(i);
            }

            // Gameover
            if (gameState == GameState::GAMEOVER)
            {
                gameOverSound.play();

                if (leaderboard.isTopScore(score))
                {
                    gameState = GameState::TOP_GAMEOVER;
                }
                leaderboardText.setString(leaderboard.getLeaderboard());
            }
        }
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
                if (gameState == GameState::TOP_GAMEOVER)
                    onTextEntered(event);
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
                if (gameState == GameState::PLAYING)
                {
                    // While game playing
                    ctrChar.jump();
                    flappingSound.play();
                }
                break;
            case sf::Keyboard::Enter:
                if (gameState == GameState::TOP_GAMEOVER)
                {
                    leaderboard.addEntry(playerString, score);
                    leaderboardText.setString(leaderboard.getLeaderboard());
                    playerString = "";
                    playerName.setString(playerString);
                    gameState = GameState::GAMEOVER;
                }
                else if (gameState != GameState::PLAYING)
                {
                    // Restart a new game
                    gameState = GameState::PLAYING;
                    reset();
                }
                break;
            default:
                break;
            }
        }
    }

    void Game::onTextEntered(const sf::Event &event)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == '\b')
            {
                if (playerString.getSize() > 0)
                {
                    playerString.erase(playerString.getSize() - 1, 1);
                    playerName.setString(playerString);
                }
            }
            else
            {
                if (playerString.getSize() < game::lb::STRING_MAX_LENGTH)
                {
                    playerString += event.text.unicode;
                    playerName.setString(playerString);
                }
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

        if (gameState != GameState::PLAYING)
        {
            draw(startText);
        }

        if (gameState == GameState::GAMEOVER || gameState == GameState::TOP_GAMEOVER)
        {
            draw(gameOverText);
            draw(panelLeaderboard);
            draw(leaderboardText);
            if (gameState == GameState::TOP_GAMEOVER)
            {
                draw(highScore);
                draw(playerName);
            }
        }

        display();
    }

    Game::~Game() {}
}
