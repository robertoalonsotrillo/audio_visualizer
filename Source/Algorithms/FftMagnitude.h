#pragma once

#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class FftMagnitude
{
public:
    void initialise()
    {
        spectrum.initialise();
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        auto output { spectrum.processed (std::vector<float> (x.begin(), x.end())) };
        std::transform (output.begin(), output.end(), output.begin(), [] (const auto val)
                        { return std::clamp (20.0f * std::log10f (val), -100.0f, 100.0f); });
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
    essentia::standard::Algorithm* fftMagnitude;
    Spectrum spectrum;
    static inline constexpr int size { 8192 };
};