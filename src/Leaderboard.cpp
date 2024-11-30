#include "Leaderboard.h"

namespace game::lb
{
    Leaderboard::Leaderboard()
    {
        loadFromFile();
    }

    void Leaderboard::loadFromFile()
    {
        std::ifstream file(LEADERBOARD_FILE);
        if (file.is_open())
        {
            entries_.clear();
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
                        entries_.push_back({name, timestamp, score});
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
                file << entry.name << DELIMITER << entry.timestamp << DELIMITER << entry.score << "\n";
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

    void Leaderboard::addEntry(const std::string &name, const int &score)
    {
        if (isTopScore(score))
        {
            std::string name_ = name;
            name_.insert(name_.end(), STRING_MAX_LENGTH - name_.size(), ' ');
            entries_.push_back({name_, std::time(0), score});
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
            std::strftime(strBuffer, sizeof(strBuffer), "%m-%d %H:%M:%S", std::localtime(&entry.timestamp));
            const std::string strTime(strBuffer);
            std::string strScore = std::to_string(entry.score);
            strScore.insert(strScore.begin(), STRING_MAX_LENGTH - strScore.size(), ' ');
            const std::string strRec = std::format("#{}  {}  {}  {}\n", i + 1, entry.name, strTime, strScore);
            strLeaderboard += strRec;
        }
        return strLeaderboard;
    }

    std::vector<LeaderboardEntry> Leaderboard::getEntries() const
    {
        return entries_;
    }

    Leaderboard::~Leaderboard() {}
}