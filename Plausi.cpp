/*
 * Plausi.cpp
 *
 */

#include <string>
#include <deque>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <regex>

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#include "Plausi.h"
#include "Config.h"

Plausi::Plausi(const Config & config) :
        config(config),_value(-1.), _time(0) {
}

bool Plausi::check(const std::string& value, time_t time) {
    log4cpp::Category& rlog = log4cpp::Category::getRoot();
    if (rlog.isInfoEnabled()) {
        rlog.info("Plausi check: %s of %s", value.c_str(), ctime(&time));
    }

    if (!std::regex_match (value,std::regex(config.getMeterValueMask()))) {
        rlog.info("Plausi rejected: value (%s) does not match with regex",value.c_str());
        return false;
    }

    double dval = (config.getMeterValueDecimals()>0)?atof(value.substr(0,config.getMeterValueLength()).c_str())/(config.getMeterValueDecimals()*10):atof(value.substr(0,config.getMeterValueLength()).c_str());
    _queue.push_back(std::make_pair(time, dval));

    if (_queue.size() < config.getMeterWindow()) {
        rlog.info("Plausi rejected: not enough values: %d", _queue.size());
        return false;
    }
    if (_queue.size() > config.getMeterWindow()) {
        _queue.pop_front();
    }

    // iterate through queue and check that all values are ascending
    // and consumption of energy is less than limit
    std::deque<std::pair<time_t, double> >::const_iterator it = _queue.begin();
    time = it->first;
    dval = it->second;
    ++it;
    while (it != _queue.end()) {
        if (it->second < dval) {
            // value must be >= previous value
            rlog.info("Plausi rejected: value must be >= previous value");
            return false;
        }
        double power = (it->second - dval) / (it->first - time) * 3600.;
        if (power > config.getMeterMaxPower()) {
            // consumption of energy must not be greater than limit
            rlog.info("Plausi rejected: consumption of energy %.3f must not be greater than limit %.3f", power, config.getMeterMaxPower());
            return false;
        }
        time = it->first;
        dval = it->second;
        ++it;
    }

    // values in queue are ok: use the center value as candidate, but test again with latest checked value
    if (rlog.isDebugEnabled()) {
        rlog.debug(queueAsString());
    }
    time_t candTime = _queue.at(config.getMeterWindow()/2).first;
    double candValue = _queue.at(config.getMeterWindow()/2).second;
    if (candValue < _value) {
        rlog.info("Plausi rejected: value must be >= previous checked value");
        return false;
    }
    double power = (candValue - _value) / (candTime - _time) * 3600.;
    if (power > config.getMeterMaxPower()) {
        rlog.info("Plausi rejected: consumption of energy (checked value) %.3f must not be greater than limit %.3f", power, config.getMeterMaxPower());
        return false;
    }

    // everything is OK -> use the candidate value
    _time = candTime;
    _value = candValue;
    if (rlog.isInfoEnabled()) {
        rlog.info("Plausi accepted: %.*f of %s", config.getMeterValueDecimals(), _value, ctime(&_time));
    }
    return true;
}

double Plausi::getCheckedValue() {
    return _value;
}

time_t Plausi::getCheckedTime() {
    return _time;
}

std::string Plausi::queueAsString() {
    std::string str;
    char buf[20];
    str += "[";
    std::deque<std::pair<time_t, double> >::const_iterator it = _queue.begin();
    for (; it != _queue.end(); ++it) {
        sprintf(buf, "%.*f", config.getMeterValueDecimals(), it->second); // %.2f
        str += buf;
        str += ", ";
    }
    str += "]";
    return str;
}

