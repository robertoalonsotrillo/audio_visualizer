#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class SpectralRollOff : public OscSender
{
public:
    void initialise()
    {
        spectralRollOff = essentia::standard::AlgorithmFactory::create ("RollOff");
        spectralRollOff->input ("spectrum").set (input);
        spectralRollOff->output ("rollOff").set (output);
        spectrum.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = spectrum.processed (std::vector<float> (x.begin(), x.end()));
        spectralRollOff->compute();
        const auto y {std::log10(output)};
        if (Config::is_osc_active)
        {
            sender.send ("/0/spectral_roll_off", y);
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
    essentia::standard::Algorithm* spectralRollOff;
    std::vector<float> input;
    float output;
    Spectrum spectrum;
    static inline constexpr int size { 8192 };
};