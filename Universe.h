#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include <noise.h>

#include "UniverseParameters.hpp"

class Universe {
private:
    /// 1000C  #ff0015
    /// 2000C  #ff4121
    /// 3000C  #ff8445
    /// 4000C  #ffbd84
    /// 5000C  #ffecd4
    /// 6000C  #d8e9fe
    /// 8000C  #86b4fb
    /// 10000C #629afa
    /// 20000C #2e75f9

    // 0.7645     M orange-red      (<0.7645) #ff1718
    // 0.121      K orange-yellow   (<0.8855) #ff8445
    // 0.076      G yellow          (<0.9615) #ffa261
    // 0.03       F yellow-white    (<0.9915) #ffd6aa
    // 0.006      A white           (<0.9975) #ffffff
    // 0.0013     B blue-white      (<0.9988) #d8e9fe
    // 0.00000003 O blue            (<0.9996) #629afa
    // --         Neutron Star      (<0.9999) #2050e0
    // --         Black Hole      (otherwise) #000000, #800080
    enum {
        e_NUM_CATEGORIES = 9,
        e_PROB_INDEX = 0,
        e_COLOR_INDEX = 1,
    };
    double star_categories_[e_NUM_CATEGORIES][4] = {
        {0.7230 /*0.7645*/, 1.000, 0.090, 0.094},
        {0.8676 /*0.8855*/, 1.000, 0.518, 0.271},
        {0.9544 /*0.9615*/, 1.000, 0.635, 0.380},
        {0.9905 /*0.9915*/, 1.000, 0.839, 0.667},
        {0.9977 /*0.9975*/, 1.000, 1.000, 1.000},
        {0.9992 /*0.9988*/, 0.847, 0.914, 0.996},
        {0.9997 /*0.9996*/, 0.384, 0.604, 0.980},
        {0.9999 /*0.9999*/, 0.126, 0.314, 0.878},
        {1.0000 /*1.0000*/, 0.500, 0.000, 0.500}
    };
public:
    struct StarInfo {
        double x, y;
        double dx, dy;
        float value;
        double *color_ptr;
    };

public: // setters
    void setOctaveCount(const double & value);
    void setFrequency(const double & value);
    void setStepSize(const double & value);
    void setXPosition(const double & value);
    void setYPosition(const double & value);
    void setMinValue(const double & value);

public:
    Universe();
    ~Universe();
    void GetParameters(UniverseParameters & params) {
        params = m_params;
    }
    void GetStars(double centerX, double centerY, std::vector<StarInfo> & stars);
    void SetOctaveCount();

    UniverseParameters * getUniverseParams() {
        return &m_params;
    }

private:
    int GetCategoryIndex(double value);

private:
    noise::module::Perlin m_noise;
    // noise::module::Billow m_noise;
    // noise::module::RidgedMulti m_noise;
    std::vector<StarInfo> m_stars;
    // Input parameters
    UniverseParameters m_params;
};

#endif // UNIVERSE_H
