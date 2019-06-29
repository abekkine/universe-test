// Copyright [2019] A.Bekkine
#ifndef PARAMETER_CONTROL_HPP_
#define PARAMETER_CONTROL_HPP_

#include "UniverseParameters.hpp"
#include "UdpSocket.h"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <GL/glut.h>

class ParameterControl {
public:
    ParameterControl() {
    }
    ~ParameterControl() {}
    void SetParameters(UniverseParameters * params) {
        params_ = params;
    }
    void Init() {
        sock_.LocalPort(3201);
        sock_.Init();
    }

    void Update() {
        if (sock_.Read((uint8_t *)&(params_->rawParams), sizeof(Params)) > 0) {
            params_->Update();
        }
    }

private:
    UniverseParameters * params_;
    UdpSocket sock_;
};

#endif // PARAMETER_CONTROL_HPP_
