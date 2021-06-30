#pragma once

class Experiment
{
public:
virtual ~Experiment() {};
virtual void OnInit() =0;
virtual void OnUpdate() =0;
virtual void OnRender() =0;
virtual void OnClose() =0;
};