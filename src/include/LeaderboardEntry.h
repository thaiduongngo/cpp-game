#pragma once
#include <time.h>
#include <string>

struct LeaderboardEntry
{
    time_t timestamp;
    int score;

    bool operator<(const LeaderboardEntry &anotherEntry) const
    {
        return score > anotherEntry.score;
    }
};
