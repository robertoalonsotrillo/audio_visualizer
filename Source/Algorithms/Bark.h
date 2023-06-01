#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class Bark : public OscSender
{
public:
    void initialise()
    {
        bark = essentia::standard::AlgorithmFactory::create ("BarkBands");
        bark->input ("spectrum").set (input);
        bark->output ("bands").set (output);
        spectrum.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
        bark->configure ("sampleRate", sampleRate);
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        input = spectrum.processed (std::vector<float> (x.begin(), x.end()));
        bark->compute();
        std::transform (output.begin(), output.end(), output.begin(), [] (const auto val)
                        { return std::clamp (20.0f * std::log10f (val), -100.0f, 100.0f); });
        sendArray("/0/bark", output);
        return output;
    }

    std::vector<std::vector<float>> processedOffline (const std::vector<float>& x)
    {
        std::vector<std::vector<float>> y {};
        for (size_t i {}; i < input.size() / size; ++i)
        {
            y.emplace_back (processed (std::vector (input.cbegin() + i * size, input.cbegin() + (i + 1) * size)));
        }
        return y;
    }

private:
    essentia::standard::Algorithm* bark;
    std::vector<float> input;
    std::vector<float> output;
    Spectrum spectrum;
    static inline constexpr int size { 8192 };
};