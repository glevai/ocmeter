/*
 * Config.cpp
 *
 */

#include <opencv2/highgui/highgui.hpp>
#include <regex>

#include "Config.h"

Config::Config() :
    _configFilename("config.yml"),
    _trainingDataFilename("trainctr.yml"),
    _meterDataFilename("emeter.txt"),
    _logFilename("emeter.log"),
    _meterValueMask("[0-9]{7}"),
    _meterValueLength(7),
	_meterValueDecimals(1),
    _meterMaxPower(50000.),
    _meterWindow(13),
    _rotationDegrees(0),
    _ocrMaxDist(5e5),
    _digitMinHeight(20),
    _digitMaxHeight(90),
    _digitYAlignment(10),
    _cannyThreshold1(100),
    _cannyThreshold2(200),
    _whiteThreshold(90) {
}

void Config::saveConfig() {
    cv::FileStorage fs(_configFilename, cv::FileStorage::WRITE);
    fs << "trainingDataFilename" << _trainingDataFilename;
    fs << "meterDataFilename" << _meterDataFilename;
    fs << "logFilename" << _logFilename;
    fs << "meterValueMask" << _meterValueMask;
    fs << "meterValueLength" << _meterValueLength;
    fs << "meterValueDecimals" << _meterValueDecimals;
    fs << "meterMaxPower" << _meterMaxPower;
    fs << "meterWindow" << _meterWindow;
    fs << "rotationDegrees" << _rotationDegrees;
    fs << "cannyThreshold1" << _cannyThreshold1;
    fs << "cannyThreshold2" << _cannyThreshold2;
    fs << "digitMinHeight" << _digitMinHeight;
    fs << "digitMaxHeight" << _digitMaxHeight;
    fs << "digitYAlignment" << _digitYAlignment;
    fs << "ocrMaxDist" << _ocrMaxDist;
    fs << "whiteTreshold" << _whiteThreshold;
    fs.release();
}

void Config::loadConfig() {
    cv::FileStorage fs(_configFilename, cv::FileStorage::READ);
    if (fs.isOpened()) {
        fs["trainingDataFilename"] >> _trainingDataFilename;
        fs["meterDataFilename"] >> _meterDataFilename;
        fs["logFilename"] >> _logFilename;
        fs["meterValueMask"] >> _meterValueMask;
        fs["meterValueLength"] >> _meterValueLength;
        fs["meterValueDecimals"] >> _meterValueDecimals;
        fs["meterMaxPower"] >> _meterMaxPower;
        fs["meterWindow"] >> _meterWindow;
        fs["rotationDegrees"] >> _rotationDegrees;
        fs["cannyThreshold1"] >> _cannyThreshold1;
        fs["cannyThreshold2"] >> _cannyThreshold2;
        fs["digitMinHeight"] >> _digitMinHeight;
        fs["digitMaxHeight"] >> _digitMaxHeight;
        fs["digitYAlignment"] >> _digitYAlignment;
        fs["ocrMaxDist"] >> _ocrMaxDist;
        fs["whiteTreshold"] >> _whiteThreshold;
        fs.release();
    } else {
        // no config file - create an initial one with default values
        saveConfig();
    }
}

void Config::setConfigFilename(std::string name) {
    _configFilename=name;
}

