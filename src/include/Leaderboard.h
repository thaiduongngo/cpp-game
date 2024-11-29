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

constexpr std::string LEADERBOARD_FILE = "leaderboard.txt";
constexpr auto LEADERBOARD_SIZE = 5;
constexpr char DELIMITER = ',';

class Leaderboard
{
private:
    std::vector<LeaderboardEntry> entries_;

public:
    Leaderboard();

    void loadFromFile();
    void saveToFile();
    bool isTopScore(const int &score) const;
    void addEntry(const int &score);
    const std::string getLeaderboard() const;
    std::vector<LeaderboardEntry> getEntries() const;
};

Leaderboard::Leaderboard()
{
    loadFromFile();
}

void Leaderboard::loadFromFile()
{
    std::ifstream file(LEADERBOARD_FILE);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string name, timestampStr, scoreStr;
            if (std::getline(ss, timestampStr, DELIMITER) &&
                std::getline(ss, scoreStr, DELIMITER))
            {
                try
                {
                    time_t timestamp = std::stoll(timestampStr);
                    int score = std::stoi(scoreStr);
                    entries_.push_back({timestamp, score});
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Error parsing leaderboard entry: " << line << std::endl;
                }
            }
        }
        file.close();
        std::sort(entries_.begin(), entries_.end());
        entries_ = std::vector<LeaderboardEntry>(std::move(entries_));
    }
}

void Leaderboard::saveToFile()
{
    std::ofstream file(LEADERBOARD_FILE);
    if (file.is_open())
    {
        for (const auto &entry : entries_)
        {
            file << entry.timestamp << DELIMITER << entry.score << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening leaderboard file for saving." << std::endl;
    }
}

bool Leaderboard::isTopScore(const int &score) const
{
    if (entries_.size() < LEADERBOARD_SIZE)
    {
        return true;
    }
    return score > entries_.back().score;
}

void Leaderboard::addEntry(const int &score)
{
    if (isTopScore(score))
    {
        entries_.push_back({std::time(0), score});
        std::sort(entries_.begin(), entries_.end());
        if (entries_.size() > LEADERBOARD_SIZE)
        {
            entries_.resize(LEADERBOARD_SIZE);
        }
        saveToFile();
    }
}

const std::string Leaderboard::getLeaderboard() const
{
    std::string strLeaderboard = "";
    for (size_t i = 0; i < entries_.size(); ++i)
    {
        const auto &entry = entries_[i];
        char strBuffer[100];
        std::strftime(strBuffer, sizeof(strBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&entry.timestamp));
        const std::string str(strBuffer);
        const std::string strRec = std::format("#{}.  {}  {}\n", i + 1, str, std::to_string(entry.score));
        strLeaderboard += strRec;
    }
    return strLeaderboard;
}

std::vector<LeaderboardEntry> Leaderboard::getEntries() const
{
    return entries_;
}
