#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class HighFrequencyContent : public OscSender
{
public:
    void initialise()
    {
        highFrequenctContent = essentia::standard::AlgorithmFactory::create ("HFC");
        highFrequenctContent->input ("spectrum").set (input);
        highFrequenctContent->output ("hfc").set (output);
        spectrum.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
//        highFrequenctContent->configure ("sampleRate", sampleRate);
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = spectrum.processed (std::vector<float> (x.begin(), x.end()));
        highFrequenctContent->compute();
        if (Config::is_osc_active)
        {
            sender.send ("/0/hfc", output);
        }
        return output;
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
    essentia::standard::Algorithm* highFrequenctContent;
    std::vector<float> input;
    float output;
    Spectrum spectrum;
    static inline constexpr int size { 8192 };
};