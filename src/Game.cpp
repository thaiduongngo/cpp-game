#include "Game.h"

namespace game
{
    Game::Game() : RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                                GAME_TITLE),
                   font("./res/FiraCode.ttf"),
                   charTexture_("./res/dragon.png"),
                   ctrChar(charTexture_),
                   pipes(),
                   clouds(),
                   mounts(),
                   scoreText(font),
                   startText(font),
                   gameOverText(font),
                   leaderboardText(font),
                   highScore(font),
                   playerName(font),
                   leaderboard(),
                   flappingSoundBuffer("./res/dragon.mp3"),
                   flappingSound(flappingSoundBuffer),
                   gameOverSoundBuffer("./res/gameover.mp3"),
                   gameOverSound(gameOverSoundBuffer)
    {
        this->setFramerateLimit(FPS_LIMIT);
        leaderboard.loadFromFile();
        //  Score text
        scoreText.setString(std::format("Score: {}", score));
        scoreText.setCharacterSize(CHAR_SIZE_HEADER);
        scoreText.setFillColor(TEXT_COLOR);
        scoreText.setOutlineThickness(1.f);
        scoreText.setOutlineColor(TEXT_OUTLINE_COLOR);
        scoreText.setPosition(sf::Vector2f(10.f, 0.f));
        // Start text
        startText.setString("Press Enter to Start");
        startText.setCharacterSize(CHAR_SIZE_HEADER);
        startText.setFillColor(TEXT_COLOR);
        startText.setOutlineThickness(1.f);
        startText.setOutlineColor(TEXT_OUTLINE_COLOR);
        startText.setOrigin(sf::Vector2f(startText.getLocalBounds().size.x / 2,
                                         startText.getLocalBounds().size.y / 2));
        startText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 250.f));
        // Gameover text
        gameOverText.setString("Game Over!");
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(CHAR_SIZE_HEADER);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOutlineThickness(1.f);
        gameOverText.setOutlineColor(sf::Color::Yellow);
        gameOverText.setOrigin(sf::Vector2f(gameOverText.getLocalBounds().size.x / 2,
                                            gameOverText.getLocalBounds().size.y / 2));
        gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, 300.f));
        // Leaderboard
        panelLeaderboard = sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.45));
        panelLeaderboard.setOutlineThickness(1.f);
        panelLeaderboard.setOutlineColor(sf::Color::Yellow);
        panelLeaderboard.setFillColor(sf::Color::Black);
        panelLeaderboard.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.1f, 350.f));
        leaderboardText.setFont(font);
        leaderboardText.setCharacterSize(CHAR_SIZE_HEADER);
        leaderboardText.setFillColor(sf::Color::Yellow);
        leaderboardText.setOutlineThickness(1.f);
        leaderboardText.setOutlineColor(sf::Color::Black);
        leaderboardText.setOrigin(sf::Vector2f(leaderboardText.getLocalBounds().size.x / 2,
                                               leaderboardText.getLocalBounds().size.y / 2));
        leaderboardText.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.15f, 400.f));
        // High score
        highScore.setString("High score! Enter your name: ");
        highScore.setFont(font);
        highScore.setCharacterSize(CHAR_SIZE_DETAIL);
        highScore.setFillColor(sf::Color::White);
        highScore.setOutlineThickness(1.f);
        highScore.setOutlineColor(sf::Color::Blue);
        highScore.setOrigin(sf::Vector2f(highScore.getLocalBounds().size.x / 2,
                                         highScore.getLocalBounds().size.y / 2));
        highScore.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.15f, 640.f));
        // Player name
        playerString = "";
        playerName.setFont(font);
        playerName.setCharacterSize(CHAR_SIZE_DETAIL);
        playerName.setFillColor(sf::Color::White);
        playerName.setOutlineThickness(1.f);
        playerName.setOutlineColor(sf::Color::Blue);
        playerName.setStyle(sf::Text::Bold | sf::Text::Italic);
        playerName.setOrigin(sf::Vector2f(playerName.getLocalBounds().size.x / 2,
                                          playerName.getLocalBounds().size.y / 2));
        playerName.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.57f, 640.f));
        reset();
    };

    void Game::reset()
    {
        score = 0;
        ctrChar.reset();
        scoreText.setString(std::format("Score: {}", score));
        pipes.reset();
        clouds.reset();
        mounts.reset();
    }

    void Game::gamePlay()
    {
        float deltaTime = clock.restart().asSeconds();

        if (gameState == GameState::PLAYING) // Game is playing
        {
            // Bird movement
            ctrChar.moveAndFall(gravity, deltaTime);

            // Cloud spawning
            clouds.spawnCloud(WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT), deltaTime);
            // Move clouds
            clouds.moveCloud(deltaTime);
            clouds.eraseOffScreenCloud();

            // Mount spawning
            mounts.spawnMount(WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT), deltaTime);
            // Move mounts
            mounts.moveMount(deltaTime);
            mounts.eraseOffScreenMount();

            // Pipe spawning
            pipes.spawnPipe(WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT * 0.66), deltaTime);

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
            while (const std::optional event = pollEvent())
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

    void Game::onClose(const std::optional<sf::Event> &event)
    {
        if (event->is<sf::Event::Closed>())
        {
            close();
        }
    }

    void Game::onKeyPress(const std::optional<sf::Event> &event)
    {
        if (event->is<sf::Event::KeyPressed>())
        {
            switch (event->getIf<sf::Event::KeyPressed>()->code)
            {
            case sf::Keyboard::Key::Space:
                if (gameState == GameState::PLAYING)
                {
                    // While game playing
                    ctrChar.jump();
                    flappingSound.play();
                }
                break;
            case sf::Keyboard::Key::Enter:
                if (gameState == GameState::TOP_GAMEOVER)
                {
                    if (playerString.getSize() > 0)
                    {
                        leaderboard.addEntry(playerString, score);
                        leaderboardText.setString(leaderboard.getLeaderboard());
                        playerString = "";
                        playerName.setString(playerString);
                        gameState = GameState::GAMEOVER;
                    }
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

    void Game::onTextEntered(const std::optional<sf::Event> &event)
    {
        const auto chr_ = std::find(BANNED_CHARS.begin(), BANNED_CHARS.end(), event->getIf<sf::Event::TextEntered>()->unicode);
        if (event->is<sf::Event::TextEntered>() && chr_ == BANNED_CHARS.end())
        {
            if (event->getIf<sf::Event::TextEntered>()->unicode == '\b')
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
                    playerString += event->getIf<sf::Event::TextEntered>()->unicode;
                    playerName.setString(playerString);
                }
            }
        }
    }

    void Game::render()
    {
        clear();
        sf::VertexArray wnd_background(sf::PrimitiveType::TriangleStrip, 4);
        // define it as a rectangle, located at (10, 10) and with size 100x100
        wnd_background[0].position = sf::Vector2f(0.f, 0.f);
        wnd_background[1].position = sf::Vector2f(0.f, WINDOW_HEIGHT);
        wnd_background[2].position = sf::Vector2f(WINDOW_WIDTH, 0.f);
        wnd_background[3].position = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        wnd_background[0].color = sf::Color::Cyan;
        wnd_background[1].color = sf::Color::Blue;
        wnd_background[2].color = sf::Color::Cyan;
        wnd_background[3].color = sf::Color::Blue;

        draw(wnd_background);

        for (const auto &mount_ : mounts.getMounts())
        {
            draw(*mount_);
        }

        for (const auto &cloud_ : clouds.getClouds())
        {
            draw(*cloud_);
        }

        for (const auto &pairPipes_ : pipes.getPipes())
        {
            draw(*pairPipes_.top);
            draw(*pairPipes_.bottom);
        }

        draw(ctrChar);
        draw(scoreText);

        if (gameState == GameState::GAMEOVER || gameState == GameState::NOT_STARTED)
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
