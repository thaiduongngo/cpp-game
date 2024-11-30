#pragma once
#include <time.h>
#include <string>

namespace game::lb
{
    struct LeaderboardEntry
    {
        std::string name;
        time_t timestamp;
        int score;

        bool operator<(const LeaderboardEntry &anotherEntry) const
        {
            return score > anotherEntry.score;
        }
    };
}
