/*
 * ImageInput.cpp
 *
 */

#include <ctime>
#include <string>
#include <list>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>

#include "ImageInput.h"

ImageInput::~ImageInput() {
}

cv::Mat& ImageInput::getImage() {
    return _img;
}

time_t ImageInput::getTime() {
    return _time;
}

void ImageInput::setOutputDir(const std::string & outDir) {
    _outDir = outDir;
}

void ImageInput::saveImage() {
    struct tm date;
    localtime_r(&_time, &date);
    char filename[PATH_MAX];
    strftime(filename, PATH_MAX, "/%Y%m%d-%H%M%S.png", &date);
    std::string path = _outDir + filename;
    if (cv::imwrite(path, _img)) {
        log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "Image saved to " + path;
    }
}

DirectoryInput::DirectoryInput(const Directory& directory) :
        _directory(directory) {
    _filenameList = _directory.list();
    _filenameList.sort();
    _itFilename = _filenameList.begin();
}

bool DirectoryInput::nextImage() {
    if (_itFilename == _filenameList.end()) {
        return false;
    }
    std::string path = _directory.fullpath(*_itFilename);

    _img = cv::imread(path.c_str());

    // read time from file name
    struct tm date;
    memset(&date, 0, sizeof(date));
    date.tm_year = atoi(_itFilename->substr(0, 4).c_str()) - 1900;
    date.tm_mon = atoi(_itFilename->substr(4, 2).c_str()) - 1;
    date.tm_mday = atoi(_itFilename->substr(6, 2).c_str());
    date.tm_hour = atoi(_itFilename->substr(9, 2).c_str());
    date.tm_min = atoi(_itFilename->substr(11, 2).c_str());
    date.tm_sec = atoi(_itFilename->substr(13, 2).c_str());
    _time = mktime(&date);

    log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "Processing " << *_itFilename << " of " << ctime(&_time);

	std::cout << "Processing " << *_itFilename << std::endl;
	
    // save copy of image if requested
    if (!_outDir.empty()) {
        saveImage();
    }

    _itFilename++;
    return true;
}

CameraInput::CameraInput(int device) {
    _capture.open(device);
}

CameraInput::CameraInput(std::string url) {
	std::cout << "Opening IP cam stream: " << url << std::endl;
	log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "Opening IP cam stream: " << url;
    _capture.open(url);
	if (!_capture.isOpened())  // if not success, exit program
    {
        log4cpp::Category::getRoot() << log4cpp::Priority::ERROR << "Cannot open the video cam " << url;
	    std::cout << "Cannot open IP cam stream: " << url << std::endl;
    }else{
		log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "IP Cam stream opened! ";
		std::cout << "IP cam stream opened!" << std::endl;
    }
}

bool CameraInput::nextImage() {
    time(&_time);
    // read image from camera
    bool success = _capture.read(_img);

    log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "Image captured: " << success;

    // save copy of image if requested
    if (success && !_outDir.empty()) {
        saveImage();
    }

    return success;
}

URLInput::URLInput(std::string url) {
	std::cout << "URL access to picture: " << url << std::endl;
	log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "URL access to picture: " << url;
    _urlImageSource = url;
}

bool URLInput::nextImage() {
    int ret;
	time(&_time);
    // download image from url
    ret = system("wget -q -O meter.jpg http://192.168.0.163:8080/photoaf.jpg"); // + _urlImageSource);

	// load image
    _img = cv::imread("meter.jpg");
    log4cpp::Category::getRoot() << log4cpp::Priority::INFO << "Image captured: " << ret;

	ret = system("rm meter.jpg");

    // save copy of image if requested
    if (!_outDir.empty()) {
        saveImage();
    }

    return true;
}
