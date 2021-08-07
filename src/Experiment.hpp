
#pragma once

#include "Model.hpp"
#include <vector>

// Interface. Contains the callbacks that are called in the rendering loop.
class Experiment
{
public:
    virtual ~Experiment(){};
    // Called after loading the graphics libary but before starting the rendering loop.
    virtual void OnInit() = 0;
    // Called on each iteration of the rendering loop, before rendering.
    virtual void OnUpdate() = 0;
    // Called on each iteration of the rendering loop, after rendering.
    virtual void OnRender() = 0;
    // Called after the rendering loop, before destroying objects associated with the graphics library.
    virtual void OnClose() = 0;
    virtual const std::vector<Model *> &models() = 0;
};