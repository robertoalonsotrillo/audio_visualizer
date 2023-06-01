#pragma once

#include "OscSender.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class AmplitudeEnvelope : public OscSender
{
public:
    void initialise() {}

    void setSampleRate (const float sampleRate)
    {
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        const auto input = std::vector<float> (x.begin(), x.end());
        std::vector<float> output {};
        juce::OSCMessage message { "/0/amplitude_envelope" };
        for (float i : input)
        {
            const auto sample { std::abs (i) };
            const auto a { lastValue < sample ? attack : release };
            lastValue = (1.0f - a) * sample + a * lastValue;
            output.emplace_back (lastValue);
            if (Config::is_osc_active)
            {
                message.addFloat32 (lastValue);
            }
        }
        if (Config::is_osc_active)
        {
            sender.send (message);
        }
        return output;
    }

    std::vector<std::vector<float>> processedOffline (const std::vector<float>& x)
    {
        std::vector<std::vector<float>> y {};
        for (size_t i {}; i < x.size() / size; ++i)
        {
            y.emplace_back (processed (std::vector (x.cbegin() + i * size, x.cbegin() + (i + 1) * size)));
        }
        return y;
    }

private:
    static inline constexpr int size { 8192 };
    float lastValue {};
    const float attack { std::expf (-1.0f / (44'100.0f * 10.0f * 0.001f)) };
    const float release { std::expf (-1.0f / (44'100.0f * 1500.0f * 0.001f)) };
};