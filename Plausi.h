/*
 * Plausi.h
 *
 */

#ifndef PLAUSI_H_
#define PLAUSI_H_

#include <string>
#include <deque>
#include <utility>
#include <ctime>

#include "Config.h"

class Plausi {
public:
    Plausi(const Config & config);
    bool check(const std::string & value, time_t time);
    double getCheckedValue();
    time_t getCheckedTime();
private:
    std::string queueAsString();
    std::deque<std::pair<time_t, double> > _queue;
    time_t _time;
    double _value;
    Config config;
};

#endif /* PLAUSI_H_ */
