#pragma once

template <typename TimestampT = unsigned int>
class ClockTimer {
private:
    TimestampT lastUpdateTime;

public:
    ClockTimer(const TimestampT& pLastUpdateTime = 0) {
        lastUpdateTime = pLastUpdateTime;
    }

    unsigned int update(const TimestampT& pCurTime) {
        unsigned int passedTimeDiff = pCurTime - lastUpdateTime;
        lastUpdateTime = pCurTime;
        return passedTimeDiff;
    }
};