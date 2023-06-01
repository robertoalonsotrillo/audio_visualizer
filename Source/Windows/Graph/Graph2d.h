#pragma once

#include "../../Config.h"
#include "GraphBase.h"

class Graph2d : public GraphBase
{
public:
    Graph2d (const juce::String& xLabelText, const juce::String& yLabelText, const Limit2d l, const std::vector<float>& b, bool draw_notes = false) : GraphBase (xLabelText, yLabelText, l), buffer (b)
    {
        grid.draw_notes = draw_notes;
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
        if (startIndex < endIndex && !buffer.empty())
        {
            const std::vector<float> buffer1 { buffer.begin() + static_cast<int> (startIndex), buffer.begin() + static_cast<int> (endIndex) };
            const size_t samplePerPixel { buffer1.size() / static_cast<size_t> (plotBounds.getWidth()) };
            if (samplePerPixel > 5)
            {
                for (size_t i {}; i < static_cast<int> (plotBounds.getWidth()); ++i)
                {
                    if (buffer1.size() >= (i + 1) * samplePerPixel)
                    {
                        const auto x { juce::jmap (static_cast<float> (i),
                                                   0.0f,
                                                   plotBounds.getWidth(),
                                                   juce::jmap (static_cast<float> (startIndex), startIndexFract, endIndexFract, plotBounds.getX(), plotBounds.getRight()),
                                                   juce::jmap (static_cast<float> (endIndex), startIndexFract, endIndexFract, plotBounds.getX(), plotBounds.getRight())) };
                        const std::vector<float> bufferSlice { buffer1.begin() + i * samplePerPixel, buffer1.begin() + (i + 1) * samplePerPixel };
                        auto yMax { *std::max_element (bufferSlice.begin(), bufferSlice.end()) };
                        auto yMin { *std::min_element (bufferSlice.begin(), bufferSlice.end()) };
                        yMax = juce::jmap (yMax, limits.y.min, limits.y.max, plotBounds.getBottom(), plotBounds.getY());
                        yMin = juce::jmap (yMin, limits.y.min, limits.y.max, plotBounds.getBottom(), plotBounds.getY());
                        path.addRectangle (x, yMax, 1, yMin - yMax);
                    }
                }
            }
            else
            {
                // prepare iir state
                // this should be omitted once we use window overlapping
                if (startIndex > 100)
                {
                    for (size_t k {}; k < 101; ++k)
                    {
                        lastY = juce::jmap (buffer[startIndex - 100 + k], limits.y.min, limits.y.max, plotBounds.getBottom(), plotBounds.getY());
                    }
                }
                for (size_t i { startIndex }; i < endIndex; ++i)
                {
                    const float index { static_cast<float> (i) };
                    juce::Point<float> point { juce::jmap (index, startIndexFract, endIndexFract, plotBounds.getX(), plotBounds.getRight()),
                                               juce::jmap (buffer[i], limits.y.min, limits.y.max, plotBounds.getBottom(), plotBounds.getY()) };
                    point.y = lastY + Config::smoothness * (point.y - lastY);
                    lastY = point.y;
                    i == startIndex ? path.startNewSubPath (point) : path.lineTo (point);
                }
            }
        }
    }

    void buildLowResLine() {}

    void buildHighResLine (const std::vector<float>& buffer)
    {
    }

    void paintGraph (juce::Graphics& g) override
    {
        g.setColour (Config::path);
        g.strokePath (path, juce::PathStrokeType (Config::pathWidth));
    }

    const std::vector<float>& buffer;
    juce::Path path;
    float lastY {};
};
