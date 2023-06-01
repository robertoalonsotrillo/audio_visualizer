#pragma once

#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class Spectrum
{
public:
    void initialise()
    {
        spectrum = essentia::standard::AlgorithmFactory::create ("Spectrum", "size", size);
        spectrum->input ("frame").set (input);
        spectrum->output ("spectrum").set (output);
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        input = std::vector<float> (x.begin(), x.end());
        spectrum->compute();
        return output;
    }

    //    std::vector<std::vector<std::complex<float>>> processedOffline (const std::vector<float>& x)
    //    {
    //        std::vector<std::vector<std::complex<float>>> y {};
    //        for (size_t i {}; i < input.size() / size; ++i)
    //        {
    //            y.emplace_back (processed (std::vector (input.cbegin() + i * size, input.cbegin() + (i + 1) * size)));
    //        }
    //        return y;
    //    }

private:
    essentia::standard::Algorithm* spectrum;
    std::vector<float> input;
    std::vector<float> output;
    static inline constexpr int size { 2048 };
};