#pragma once
#include "juce_audio_basics/juce_audio_basics.h"
#include <juce_gui_basics/juce_gui_basics.h>

struct Limit
{
    float min;
    float max;
};

struct Limit2d
{
    Limit x;
    Limit y;
};

class Grid : public juce::Component
{
public:
    enum class Direction
    {
        Horizontal,
        Vertical
    };

    bool draw_notes {};

    void draw (juce::Graphics& g, Limit limit, Direction direction)
    {
        g.setFont (10.0);
        const auto [y0, y1] { limit };
        const auto diff { y1 - y0 };
        const auto exponent { static_cast<int> (floor (std::log10 (diff / 5.0))) };
        const auto b { std::pow (10.0f, static_cast<float> (exponent)) };
        const auto c { static_cast<int> (floor ((sqrt ((1.0 / (5.0 * b))) * sqrt ((diff - 5.0 * b))))) };
        const auto d { static_cast<float> ((c * c)) + 1.0f };
        const auto steps { b * d };
        const auto padding { fmod (y1, steps) };
        juce::Path path {};
        for (int i { y1 > 0.0f ? 0 : 1 }; i < 12; ++i)
        {
            const auto y { y1 - padding - static_cast<float> (i) * steps };
            if (y < y0)
            {
                break;
            }
            const auto width { static_cast<float> (getWidth()) };
            const auto height { static_cast<float> (getHeight()) };
            if (direction == Direction::Horizontal)
            {
                const auto a { juce::jmap (y, y1, y0, height * 0.1f, height * 0.9f) };
                const auto start { juce::Point { width * 0.1f, a } };
                const auto end { juce::Point { width * 0.9f, a } };
                g.setColour (juce::Colours::white.withAlpha (0.2f));
                g.drawLine ({ start, end });
                g.setColour (juce::Colours::white.withAlpha (1.0f));
                const auto midiNumber { juce::jlimit (0.0, 128.0, std::log2 (std::pow (10.0, y) / 440.0) * 12.0 + 69.0) };
                const auto midiNoteName { juce::MidiMessage::getMidiNoteName (static_cast<int> (midiNumber), true, true, 4) };
                const auto cents { juce::jmap (midiNumber, std::floor (midiNumber), std::floor (midiNumber) + 1.0, 0.0, 100.0) };
                std::stringstream stream;
                stream << std::fixed << std::setprecision (0) << cents;
                std::string centsString = stream.str();
                g.drawFittedText (draw_notes ? midiNoteName + " " + centsString : juce::String { y }, juce::Rectangle { 0.0f, a - 5.0f, width * 0.1f, 10.0f }.toNearestInt(), juce::Justification::Flags::centred, 2);
            }
            else
            {
                const auto a { juce::jmap (y, y0, y1, width * 0.1f, width * 0.9f) };
                const auto start { juce::Point { a, height * 0.1f } };
                const auto end { juce::Point { a, height * 0.9f } };
                g.setColour (juce::Colours::white.withAlpha (0.2f));
                g.drawLine ({ start, end });
                g.setColour (juce::Colours::white.withAlpha (1.0f));
                g.drawText (juce::String { y }, juce::Rectangle { a - 30.0f, 0.0f, 60.0f, height * 0.1f }, juce::Justification::Flags::centred);
            }
        }
        g.strokePath (path, juce::PathStrokeType { 1.0f });
    }
};