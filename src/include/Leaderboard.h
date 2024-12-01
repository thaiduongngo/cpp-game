#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "LeaderboardEntry.h"

#ifndef LEADERBOARD_SIZE
#define LEADERBOARD_SIZE 5
#endif

namespace game::lb
{
    constexpr std::string LEADERBOARD_FILE = "leaderboard.dat";
    constexpr char DELIMITER = ',';
    constexpr int STRING_MAX_LENGTH = 6;

    class Leaderboard
    {
    private:
        std::vector<LeaderboardEntry> entries_;

    public:
        Leaderboard();

        void loadFromFile();
        void saveToFile();
        bool isTopScore(const int &score) const;
        void addEntry(const std::string &name, const int &score);
        const std::string getLeaderboard() const;
        std::vector<LeaderboardEntry> getEntries() const;

        ~Leaderboard();
    };
}