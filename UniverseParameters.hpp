#ifndef UNIVERSE_PARAMETERS_HPP_
#define UNIVERSE_PARAMETERS_HPP_

struct UniverseParameters {

    int octaveCount;
    double frequency;
    double x, y;
    double minValue;
    double stepSize;

    UniverseParameters()
    : octaveCount(2)
    , frequency(0.25)
    , x(13.0)
    , y(-19.0)
    , minValue(0.2)
    , stepSize(0.2)
    {}
};

#endif // UNIVERSE_PARAMETERS_HPP_
