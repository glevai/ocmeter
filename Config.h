/*
 * Config.h
 *
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <regex>

class Config {
public:
    Config();
    void saveConfig();
    void loadConfig();
    void setConfigFilename(std::string name);


    std::string getConfigFilename() const {
        return _configFilename;
    }

    int getDigitMaxHeight() const {
        return _digitMaxHeight;
    }

    int getDigitMinHeight() const {
        return _digitMinHeight;
    }

    int getDigitYAlignment() const {
        return _digitYAlignment;
    }

    std::string getTrainingDataFilename() const {
        return _trainingDataFilename;
    }

    std::string getMeterDataFilename() const {
        return _meterDataFilename;
    }

    std::string getLogFilename() const {
        return _logFilename;
    }

    std::string getMeterValueMask() const {
        return _meterValueMask;
    }

    int getMeterValueLength() const {
        return _meterValueLength;
    }

    int getMeterValueDecimals() const {
        return _meterValueLength;
    }

    float getMeterMaxPower() const {
        return _meterMaxPower;
    }

    int getMeterWindow() const {
        return _meterWindow;
    }

    float getOcrMaxDist() const {
        return _ocrMaxDist;
    }

    int getRotationDegrees() const {
        return _rotationDegrees;
    }

    int getCannyThreshold1() const {
        return _cannyThreshold1;
    }

    int getCannyThreshold2() const {
        return _cannyThreshold2;
    }

    int getWhiteThreshold() const {
        return _whiteThreshold;
    }

private:
    std::string _configFilename;
    std::string _trainingDataFilename;
    std::string _meterDataFilename;
    std::string _logFilename;
    std::string _meterValueMask;
    int _meterValueLength;
	int _meterValueDecimals;
    float _meterMaxPower;
    int _meterWindow;
    int _rotationDegrees;
    float _ocrMaxDist;
    int _digitMinHeight;
    int _digitMaxHeight;
    int _digitYAlignment;
    int _cannyThreshold1;
    int _cannyThreshold2;
    int _whiteThreshold;
	};

#endif /* CONFIG_H_ */

extern Config config;

