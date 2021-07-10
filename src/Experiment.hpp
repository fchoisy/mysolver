#pragma once

#include "Model.hpp"
#include <vector>
class Experiment
{
public:
    virtual ~Experiment(){};
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnClose() = 0;
    virtual const std::vector<Model*> &models() = 0;
};