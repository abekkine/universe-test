#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include <noise.h>

#include "UniverseParameters.hpp"

class Universe {
private:
    // 0.7645     M orange-red      (<0.7645) #ff1718
    // 0.121      K orange-yellow   (<0.8855) #ff8445
    // 0.076      G yellow          (<0.9615) #ffa261
    // 0.03       F yellow-white    (<0.9915) #ffd6aa
    // 0.006      A white           (<0.9975) #ffffff
    // 0.0013     B blue-white      (<0.9988) #d8e9fe
    // 0.00000003 O blue            (<0.9996) #629afa
    // --         Neutron Star      (<0.9999) #2050e0
    // --         Black Hole      (otherwise) #000040, #800080
    enum {
        e_NUM_CATEGORIES = 9,
        e_PROB_INDEX = 0,
        e_COLOR_INDEX = 1,
    };
    double star_categories_[e_NUM_CATEGORIES][4] = {
        {0.2, 1.000, 0.090, 0.094}, // #ff1718
        {0.3, 1.000, 0.518, 0.271}, // #ff8445
        {0.4, 1.000, 0.635, 0.380}, // #ffa261
        {0.5, 1.000, 0.839, 0.667}, // #ffd6aa
        {0.6, 1.000, 1.000, 1.000}, // #ffffff
        {0.7, 0.847, 0.914, 0.996}, // #d8e9fe
        {0.8, 0.384, 0.604, 0.980}, // #629afa
        {0.9, 0.126, 0.314, 0.878}, // #2050e0
        {1.0, 0.000, 0.000, 0.250}  // #000040
    };
public:
    struct StarInfo {
        double x, y;
        double dx, dy;
        float value;
        float color_dev;
        float size;
        double *color_ptr;
    };

public: // setters
    void setOctaveCount(const double & value);
    double getOctaveCount();
    void setFrequency(const double & value);
    double getFrequency();
    void setStepSize(const double & value);
    double getStepSize();
    void setXPosition(const double & value);
    double getXPosition();
    void setYPosition(const double & value);
    double getYPosition();
    void setMinValue(const double & value);
    double getMinValue();
    void setZIndex(const double & value);
    double getZIndex();

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
    void UpdateCategoryIndex();
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
