#pragma once

#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class SpectralCentroid : public OscSender
{
public:
    void initialise()
    {
        spectralCentroid = essentia::standard::AlgorithmFactory::create ("SpectralCentroidTime");
        spectralCentroid->input ("array").set (input);
        spectralCentroid->output ("centroid").set (output);
    }

    void setSampleRate (const float sampleRate)
    {
    }

    template <class Container>
    float processed (const Container& x)
    {
        input = std::vector<float> (x.begin(), x.end());
        spectralCentroid->compute();
        const auto y {std::log10(output)};
        if (Config::is_osc_active)
        {
            sender.send ("/0/spectral_centroid", y);
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
    essentia::standard::Algorithm* spectralCentroid;
    std::vector<float> input;
    float output;
    static inline constexpr int size { 8192 };
};