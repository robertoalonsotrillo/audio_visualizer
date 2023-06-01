#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class ZeroCrossingRate : public OscSender
{
public:
    void initialise()
    {
        zeroCrossingRate = essentia::standard::AlgorithmFactory::create ("ZeroCrossingRate");
        zeroCrossingRate->input ("signal").set (input);
        zeroCrossingRate->output ("zeroCrossingRate").set (output);
    }

    void setSampleRate (const float sampleRate)
    {
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = std::vector<float> (x.begin(), x.end());
        zeroCrossingRate->compute();
        const auto y {output};
        if (Config::is_osc_active)
        {
            sender.send ("/0/zero_crossing_rate", y);
        }
        return y;
    }

    std::vector<float> processedOffline (const std::vector<float>& x)
    {
        std::vector<float> y {};
        for (size_t i {}; i < input.size() / size; ++i)
        {
            y.emplace_back (processed (std::vector (input.cbegin() + i * size, input.cbegin() + (i + 1) * size)));
        }
        return y;
    }

private:
    essentia::standard::Algorithm* zeroCrossingRate;
    std::vector<float> input;
    float output;
    static inline constexpr int size { 8192 };
};