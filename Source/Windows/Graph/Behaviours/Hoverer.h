#pragma once

#include "../Grid.h"
#include "juce_gui_basics/juce_gui_basics.h"

class Hovering : public juce::Label
{
public:
    void update (juce::Point<float> mousePosition, Limit2d graphLimit, Limit2d windowLimit, bool draw_notes)
    {
        if (const auto [mouseX, mouseY] { mousePosition }; mouseX < windowLimit.x.max && mouseX > windowLimit.x.min && mouseY < windowLimit.y.max && mouseY > windowLimit.y.min)
        {
            setVisible (true);
            const auto x { juce::jmap (mouseX, windowLimit.x.min, windowLimit.x.max, graphLimit.x.min, graphLimit.x.max) };
            const auto y { juce::jmap (mouseY, windowLimit.y.max, windowLimit.y.min, graphLimit.y.min, graphLimit.y.max) };
            if (draw_notes)
            {
                const auto midiNumber { std::log2 (std::pow (10.0, y) / 440.0) * 12.0 + 69.0 };
                const auto midiNoteName { juce::MidiMessage::getMidiNoteName (static_cast<int> (midiNumber), true, true, 4) };
                const auto cents { juce::jmap (midiNumber, std::floor (midiNumber), std::floor (midiNumber) + 1.0, 0.0, 100.0) };
                std::stringstream stream;
                stream << std::fixed << std::setprecision (0) << cents;
                std::string centsString = stream.str();
                setText ("x = " + juce::String { x } + "\n" + "y = " + midiNoteName + " " + centsString, juce::NotificationType::dontSendNotification);
            }
            else
            {
                setText ("x = " + juce::String { x } + "\n" + "y = " + juce::String { y }, juce::NotificationType::dontSendNotification);
            }
            setSize (getFont().getStringWidth (getText()), static_cast<int> (getFont().getHeight()) * 2);
            setTopLeftPosition (mousePosition.toInt().translated (10, 10));
        }
        else
        {
            setVisible (false);
        }
    }
};