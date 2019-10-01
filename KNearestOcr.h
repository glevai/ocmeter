/*
 * KNearestOcr.h
 *
 */

#ifndef KNEARESTOCR_H_
#define KNEARESTOCR_H_

#include <vector>
#include <list>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

class KNearestOcr {
public:
    KNearestOcr();
    virtual ~KNearestOcr();

    int learn(const cv::Mat & img);
    int learn(const std::vector<cv::Mat> & images);
    void saveTrainingData();
    bool loadTrainingData();

    char recognize(const cv::Mat & img);
    std::string recognize(const std::vector<cv::Mat> & images);

    cv::Mat _samples;
    cv::Mat _responses;

	private:
    cv::Mat prepareSample(const cv::Mat & img);
    void initModel();

    CvKNearest* _pModel;
};

#endif /* KNEARESTOCR_H_ */
