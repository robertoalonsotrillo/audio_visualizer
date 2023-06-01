#pragma once

#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class Loudness : public OscSender
{
public:
    void initialise()
    {
        loudness = essentia::standard::AlgorithmFactory::create ("Loudness");
        loudness->input ("signal").set (input);
        loudness->output ("loudness").set (output);
    }

    void setSampleRate (const float sampleRate)
    {
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = std::vector<float> (x.begin(), x.end());
        loudness->compute();
        const auto y {std::clamp (20.0f * std::log10f (output), -100.0f, 100.0f)};
        if (Config::is_osc_active)
        {
            sender.send ("/0/loudness", y);
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
    essentia::standard::Algorithm* loudness;
    std::vector<float> input;
    float output;
    static inline constexpr int size { 8192 };
};