#pragma once

#include "../Grid.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ZoomRectangle : public juce::Component
{
public:
    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::yellow);
        g.drawRect (getLocalBounds());
    }

    void update (juce::Rectangle<int> bounds)
    {
        setBounds (bounds);
    }
};