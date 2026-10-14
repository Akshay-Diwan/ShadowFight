#pragma once

#include "animation/Animation.h"

#include <string>

AnimationFrame loadInitialIdleFrame();
AnimationFrame createAbsoluteFrame(const AnimationFrame& idleFrame, const AnimationFrame& deltaFrame);
Animation loadAnimation(const std::string& filename, const AnimationFrame& idleFrame);