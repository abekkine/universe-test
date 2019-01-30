#include "Universe.h"

#include <math.h>

#include <iostream>

Universe::Universe() {

    m_noise.SetOctaveCount(m_params.octaveCount);
    m_noise.SetFrequency(m_params.frequency);
    // m_noise.SetPersistence(0.02);

    UpdateCategoryIndex();
}

Universe::~Universe() {}

void Universe::setOctaveCount(const double & value) {
    m_params.octaveCount = 1 + static_cast<int>(7.0 * value);
    m_noise.SetOctaveCount(m_params.octaveCount);
}
double Universe::getOctaveCount() {
    return ((double)m_params.octaveCount - 1.0) / 7.0;
}

void Universe::setFrequency(const double & value) {
    m_params.frequency = (value * 15.9) + 0.1;
    m_noise.SetFrequency(m_params.frequency);
}
double Universe::getFrequency() {
    return ((m_params.frequency - 0.1) / 15.9);
}

void Universe::setStepSize(const double & value) {
    m_params.stepSize = 0.05 + value * 19.95;
}
double Universe::getStepSize() {
    return ((m_params.stepSize - 0.05) / 19.95);
}

void Universe::setXPosition(const double & value) {
    m_params.x = value * 40.0 - 20.0;
}
double Universe::getXPosition() {
    return ((m_params.x + 20.0) / 40.0);
}

void Universe::setYPosition(const double & value) {
    m_params.y = value * 40.0 - 20.0;
}
double Universe::getYPosition() {
    return ((m_params.y + 20.0) / 40.0);
}

void Universe::setMinValue(const double & value) {
    m_params.minValue = value * 0.9;
}
double Universe::getMinValue() {
    return (m_params.minValue / 0.9);
}
void Universe::setZIndex(const double & value) {
    m_params.zIndex = value * 20.0 - 10.0;
}
double Universe::getZIndex() {
    return ((m_params.zIndex + 10.0)/20.0);
}

void Universe::UpdateCategoryIndex() {
    double totalNums = m_params.numBHs +
        m_params.numNSs +
        m_params.numOs +
        m_params.numBs +
        m_params.numAs +
        m_params.numFs +
        m_params.numGs +
        m_params.numKs +
        m_params.numMs;

    star_categories_[0][0] = m_params.numMs;
    star_categories_[1][0] = m_params.numKs;
    star_categories_[2][0] = m_params.numGs;
    star_categories_[3][0] = m_params.numFs;
    star_categories_[4][0] = m_params.numAs;
    star_categories_[5][0] = m_params.numBs;
    star_categories_[6][0] = m_params.numOs;
    star_categories_[7][0] = m_params.numNSs;
    star_categories_[8][0] = m_params.numBHs;

    for (int i=1; i<e_NUM_CATEGORIES; ++i){
        star_categories_[i][0] += star_categories_[i-1][0];

    }
    for (int i=0; i<e_NUM_CATEGORIES; ++i) {
        star_categories_[i][0] /= totalNums;
        std::cout << i << " : " << star_categories_[i][0] << '\n';
    }
}

void Universe::GetStars(const double & centerX, const double & centerY, const double & size, std::vector<StarInfo> & stars) {

    const double ds = m_params.stepSize;
    const double frame_size = ds * floor(0.5 * size / ds);

    m_stars.clear();
    int category_index;
    for (double dx=-frame_size; dx<frame_size; dx+=ds) {
        for (double dy=-frame_size; dy<frame_size; dy+=ds) {
            double x = floor(centerX/ds)*ds + dx;
            double y = floor(centerY/ds)*ds + dy;
            double value = m_noise.GetValue(x, y, m_params.zIndex);
            if (value >= -0.5 && value <= 0.5) {
                value += 0.5;
                if (value > m_params.minValue) {
                    float color_deviation = m_noise.GetValue(x, y, 19.781) * 0.2;
                    float size = fabs(m_noise.GetValue(x, y, -4.904)) + 0.4;
                    value = (value - m_params.minValue) / (1.0 - m_params.minValue);
                    StarInfo p;
                    p.x = x;
                    p.y = y;
                    p.value = value;
                    category_index = GetCategoryIndex(p.value);
                    p.color_ptr = &star_categories_[category_index][e_COLOR_INDEX];
                    p.color_dev = color_deviation;
                    p.size = size;
                    p.dx = ds * m_noise.GetValue(p.x, p.y, m_params.x);
                    p.dy = ds * m_noise.GetValue(p.x, p.y, m_params.y);
                    m_stars.push_back(p);
                }
            }
        }
    }

    stars = m_stars;
}

int Universe::GetCategoryIndex(double value) {
    int ix = e_NUM_CATEGORIES-1;
    for (int i=0; i<e_NUM_CATEGORIES-1; ++i) {
        if (value < star_categories_[i][e_PROB_INDEX]) {
            ix = i;
            break;
        }
    }
    return ix;
}
