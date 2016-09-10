#pragma once


// Represent immutable set of options for CvModelBuilder
class CvModelBuilderOptions
{
public:
    CvModelBuilderOptions(bool _boostContrast = true);

    bool boostContrast() const {
        return _boostContrast;
    }

    double cannyThreshold1() const {
        return 100;
    }

    double cannyThreshold2() const {
        return 300;
    }

private:
    bool _boostContrast;
};

