#include "Config.h"

namespace Config
{
juce::Colour path = juce::Colours::green;
juce::Colour background = juce::Colour::greyLevel (0.1f);
float pathWidth = 1.0f;
bool drawGrid = true;
float smoothness = 1.0f;
size_t windowSize = 2048;
float overlap = 50.0f;
bool is_osc_active = false;
int oscPort = 9001;
} // namespace Config
