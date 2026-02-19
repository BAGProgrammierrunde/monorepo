#pragma once

namespace pa {
    template <typename TimestampT = unsigned int>
    class IntervalTimer {
    private:
        TimestampT lastUpdateTime;
        unsigned int intervalUnits;
    
    public:
        IntervalTimer(unsigned int pIntervalUnits = 0) {
            intervalUnits = pIntervalUnits;
            lastUpdateTime = 0;
        }
    
        unsigned int getIntervalUnits() {
            return intervalUnits;
        }
    
        void setIntervalUnits(unsigned int pIntervalUnits) {
            intervalUnits = pIntervalUnits;
        }
    
        bool tryUpdate(const TimestampT& pCurTime) {
            if (pCurTime - lastUpdateTime >= intervalUnits)
            {
                lastUpdateTime = pCurTime;
                return true;
            }
            return false;
        }
    };
}