#pragma once

#include "GraphBase.h"

template <size_t BandCount>
class Graph3d : public GraphBase
{
public:
    Graph3d (const juce::String& xLabelText, const juce::String& yLabelText, const Limit2d l, const std::vector<std::vector<float>>& b) : GraphBase (xLabelText, yLabelText, l), buffer (b)
    {
        gradient.addColour (0.0, juce::Colours::transparentBlack);
        gradient.addColour (1.0f * 1.0f / 6.0f, juce::Colours::blue);
        gradient.addColour (2.0f * 1.0f / 6.0f, juce::Colours::pink);
        gradient.addColour (3.0f * 1.0f / 6.0f, juce::Colours::red);
        gradient.addColour (4.0f * 1.0f / 6.0f, juce::Colours::orange);
        gradient.addColour (5.0f * 1.0f / 6.0f, juce::Colours::yellow);
        gradient.addColour (1.0f, juce::Colours::white);
    }

    void updateFollow() override
    {
        const auto rangeLength { limits.x.max - limits.x.min };
        const auto max { (static_cast<float> (buffer.size() * (Config::windowSize * (1.0f - Config::overlap * 0.01f))) / 44'100.0f) + rangeLength / 2.0f };
        limits.x = { max - rangeLength, max };
    }

private:
    void buildPath() override
    {
        path.clear();
        const auto startIndexFract = (limits.x.min * 44'100.0f) / (Config::windowSize * (1.0f - Config::overlap * 0.01f));
        const auto endIndexFract = (limits.x.max * 44'100.0f) / (Config::windowSize * (1.0f - Config::overlap * 0.01f));
        const auto startIndex = (size_t) std::max (0.0f, std::floor (startIndexFract));
        const auto endIndex = (size_t) std::min<float> (buffer.size() - 1, std::ceil (endIndexFract));

        for (size_t i { startIndex }; i <= endIndex; ++i)
        {
            const auto x { juce::jmap (static_cast<float> (i), startIndexFract, endIndexFract, plotBounds.getX(), plotBounds.getRight()) };
            for (size_t j {}; j < buffer[i].size(); ++j)
            {
                const auto y { juce::jmap (static_cast<float> ((j + 1) * limits.y.max / BandCount), limits.y.min, limits.y.max, plotBounds.getBottom(), plotBounds.getY()) };
                path.push_back ({ { x, y }, gradient.getColourAtPosition (juce::jmap (std::clamp (buffer[i][j], -80.0f, 0.0f), -80.0f, 0.0f, 0.0f, 1.0f)) });
            }
        }
    }

    void paintGraph (juce::Graphics& g) override
    {
        const auto rectWidth { path[BandCount].pos.x - path[0].pos.x };
        const auto rectHeight { path[1].pos.y - path[0].pos.y };
        for (auto& pos : path)
        {
            static float last_y {};
            if (static_cast<int> (pos.pos.y) == static_cast<int> (last_y))
            {
                continue;
            }
            g.setColour (pos.colours);
            g.fillRect (pos.pos.x, pos.pos.y, rectWidth, rectHeight);
            last_y = pos.pos.y;
        }
    }

    const std::vector<std::vector<float>>& buffer;
    struct op
    {
        juce::Point<float> pos;
        juce::Colour colours;
    };
    std::vector<op> path;
    juce::ColourGradient gradient;
};
