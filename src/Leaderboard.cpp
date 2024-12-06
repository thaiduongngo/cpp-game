#include "Leaderboard.h"
#include <iostream>

namespace game::lb
{
    Leaderboard::Leaderboard()
    {
        entries_.reserve(LEADERBOARD_SIZE);
        loadFromFile();
    }

    void Leaderboard::loadFromFile()
    {
        std::ifstream file(LEADERBOARD_FILE);
        if (file.is_open())
        {
            Entries_t().swap(entries_);
            std::string line;
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string name, timestampStr, scoreStr;
                if (std::getline(ss, name, DELIMITER) &&
                    std::getline(ss, timestampStr, DELIMITER) &&
                    std::getline(ss, scoreStr, DELIMITER))
                {
                    try
                    {
                        time_t timestamp = std::stoll(timestampStr);
                        int score = std::stoi(scoreStr);
                        entries_.emplace_back(
                            LeaderboardEntry(name, timestamp, score));
                    }
                    catch (const std::invalid_argument &e)
                    {
                        std::cerr << "Error parsing leaderboard entry: " << line << std::endl;
                    }
                }
            }
            file.close();
            std::sort(entries_.begin(), entries_.end());
        }
    }

    void Leaderboard::saveToFile()
    {
        std::ofstream file(LEADERBOARD_FILE);
        if (file.is_open())
        {
            for (const auto &entry : entries_)
            {
                file << entry.name << DELIMITER
                     << entry.timestamp << DELIMITER
                     << entry.score << "\n";
            }
            file.close();
        }
        else
        {
            std::cerr << "Error opening leaderboard file for saving." << std::endl;
        }
    }

    const bool Leaderboard::isTopScore(const int &score) const
    {
        if (entries_.size() < LEADERBOARD_SIZE)
        {
            return true;
        }
        return score > entries_.back().score;
    }

    void Leaderboard::addEntry(const std::string &name, const int &score)
    {
        if (isTopScore(score))
        {
            std::string name_ = name;
            name_.insert(name_.end(), STRING_MAX_LENGTH - name_.size(), ' ');
            entries_.emplace_back(
                LeaderboardEntry(name_, std::time(0), score));
            std::sort(entries_.begin(), entries_.end());
            if (entries_.size() > LEADERBOARD_SIZE)
            {
                entries_.resize(LEADERBOARD_SIZE);
            }
            Entries_t(entries_).swap(entries_);
            saveToFile();
        }
    }

    const std::string Leaderboard::getLeaderboard() const
    {
        std::string strLeaderboard = "";
        for (size_t i = 0; i < entries_.size(); ++i)
        {
            const auto &entry = entries_[i];
            const size_t BUFFER_SIZE = 80;
            char *strBuffer = (char *)calloc(BUFFER_SIZE, sizeof(char *));
            if (!strBuffer)
            {
                std::cerr << "Cannot allocate memory for a string buffer" << std::endl;
                break;
            }
            std::strftime(strBuffer, BUFFER_SIZE, "%m-%d %H:%M:%S", std::localtime(&entry.timestamp));
            const std::string strTime(strBuffer);
            free(strBuffer);
            strBuffer = NULL;
            std::string strScore = std::to_string(entry.score);
            strScore.insert(strScore.begin(), STRING_MAX_LENGTH - strScore.size(), ' ');
            const std::string strRec = std::format("#{}  {}  {}  {}\n", i + 1, entry.name, strTime, strScore);
            strLeaderboard += strRec;
        }
        return strLeaderboard;
    }

    Leaderboard::~Leaderboard() {}
}