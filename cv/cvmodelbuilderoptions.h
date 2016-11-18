#pragma once


// Represent set of options for CvModelBuilder
struct CvModelBuilderOptions
{
    CvModelBuilderOptions();

    bool boostContrast;
    double cannyThreshold1;
    double cannyThreshold2;
};
