#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class F0 : public OscSender
{
public:
    void initialise()
    {
        f0 = essentia::standard::AlgorithmFactory::create ("PitchYinFFT", "frameSize", size);
        f0->input ("spectrum").set (input);
        f0->output ("pitch").set (output);
        f0->output ("pitchConfidence").set (confidence);
        spectrum.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
//        f0->configure ("sampleRate", sampleRate);
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = spectrum.processed (std::vector<float> (x.begin(), x.end()));
        f0->compute();
        const auto y {std::log10(output)};
        if (Config::is_osc_active)
        {
            sender.send ("/0/f0", y);
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
    essentia::standard::Algorithm* f0;
    std::vector<float> input;
    float output;
    float confidence;
    Spectrum spectrum;
    static inline constexpr int size { 8192 };
};