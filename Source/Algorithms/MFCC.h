#pragma once

#include "Spectrum.h"
#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class MFCC : public OscSender
{
public:
    void initialise()
    {
        mfcc = essentia::standard::AlgorithmFactory::create ("MFCC", "inputSize", size / 2 + 1);
        mfcc->input ("spectrum").set (input);
        mfcc->output ("mfcc").set (output);
        mfcc->output ("bands").set (bands);
        spectrum.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
        mfcc->configure ("sampleRate", sampleRate);
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        input = spectrum.processed (std::vector<float> (x.begin(), x.end()));
        mfcc->compute();
        //        std::transform (output.begin(), output.end(), output.begin(), [] (const auto val)
        //                        { return std::clamp (20.0f * std::log10f (val), -100.0f, 100.0f); });
        sendArray("/0/mfcc", output);
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
    essentia::standard::Algorithm* mfcc;
    std::vector<float> input;
    std::vector<float> output;
    std::vector<float> bands;
    Spectrum spectrum;
    static inline constexpr int size { 2048 };
};