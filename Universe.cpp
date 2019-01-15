#include "Universe.h"

#include <math.h>

#include <iostream>

Universe::Universe() {

    param_octave_count_ = 2;
    param_frequency_ = 0.25;
    param_x_position_ = 13.0;
    param_y_position_ = -19.0;
    param_min_value_ = 0.2;
    param_step_size_ = 0.2;

    SetOctaveCount();
    SetFrequency();

    // m_noise.SetPersistence(0.02);
}

Universe::~Universe() {}

void Universe::SetOctaveCount() {
    m_noise.SetOctaveCount(param_octave_count_);
}

void Universe::SetFrequency() {
    m_noise.SetFrequency(param_frequency_);
}

void Universe::ParamAddXPosition(double value) {
    param_x_position_ += value;
}
void Universe::ParamAddYPosition(double value) {
    param_y_position_ += value;
}
void Universe::ParamAddMinValue(double value) {
    param_min_value_ += value;
    if (param_min_value_ < 0.0) param_min_value_ = 0.0;
    else if (param_min_value_ > 0.9) param_min_value_ = 0.9;
}
void Universe::ParamAddStepSize(double value) {
    param_step_size_ += value;
    if (param_step_size_ < 0.1) param_step_size_ = 0.1;
    else if (param_step_size_ > 2.0) param_step_size_ = 2.0;
}
void Universe::ParamAddFrequency(double value) {
    param_frequency_ += value;
    if (param_frequency_ < 0.1) param_frequency_ = 0.1;
    else if (param_frequency_ > 16.0) param_frequency_ = 16.0;
    SetFrequency();
}
void Universe::ParamAddOctaveCount(int value) {
    param_octave_count_ += value;
    if (param_octave_count_ < 1) param_octave_count_ = 1;
    else if (param_octave_count_ > 8) param_octave_count_ = 8;
    SetOctaveCount();
}

void Universe::GetStars(double centerX, double centerY, std::vector<StarInfo> & stars) {

    const double ds = param_step_size_; // 0.2;//1.0;
    const double frame_size = 15.0;

    m_stars.clear();
    int category_index;
    for (double dx=-frame_size; dx<frame_size; dx+=ds) {
        for (double dy=-frame_size; dy<frame_size; dy+=ds) {
            double x = floor(centerX/ds)*ds + dx;
            double y = floor(centerY/ds)*ds + dy;
            // double value = 0.5 * (1.0 + m_noise.GetValue(x, y, 0.0));
            double value = 0.5 + m_noise.GetValue(x, y, 0.0);
            {
                static double min_v = 1000.0;
                static double max_v = -1000.0;
                if (value > max_v) {
                    max_v = value;
                    // std::cout << "MAX(" << max_v << "), ";
                    // std::cout << "MIN(" << min_v << ")\n";
                }
                else if (value < min_v) {
                    min_v = value;
                    // std::cout << "MAX(" << max_v << "), ";
                    // std::cout << "MIN(" << min_v << ")\n";
                }
            }
            if (value > param_min_value_) { // 0.2) {
                StarInfo p;
                p.x = x;
                p.y = y;
                // DEBUG
                p.value = value;
                // p.value = (value - 0.5) * 2.0;
                category_index = GetCategoryIndex(p.value);
                p.color_ptr = &star_categories_[category_index][e_COLOR_INDEX];
                p.dx = ds * m_noise.GetValue(p.x, p.y, param_x_position_);
                p.dy = ds * m_noise.GetValue(p.x, p.y, param_y_position_);
                m_stars.push_back(p);
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
