#pragma once
#include "common.h"

namespace Time
{
    void Update();
    float GetTime();
    float GetDeltaTime();
    void LimitFPS();
}