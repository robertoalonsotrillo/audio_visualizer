#pragma once

#include <algorithm.h>
#include <algorithmfactory.h>
#include <cmath>
#include <numeric>
#include <vector>

class ConstantQChromagram : public OscSender
{
public:
    void initialise()
    {
        chromagram = essentia::standard::AlgorithmFactory::create ("Chromagram");
        chromagram->input ("frame").set (input);
        chromagram->output ("chromagram").set (output);
    }

    void setSampleRate (const float sampleRate)
    {
//        std::cout << "sampleRate" << "\n";
//        chromagram->configure ("sampleRate", sampleRate);
    }

    template <class Container>
    std::vector<float> processed (const Container& x)
    {
        input = std::vector<float> (x.begin(), x.end());
        input.resize (32768);
        chromagram->compute();
        std::transform (output.begin(), output.end(), output.begin(), [] (const auto val)
                        { return std::clamp (20.0f * std::log10f (val), -100.0f, 100.0f); });
        sendArray("/0/constant_q", output);
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
    essentia::standard::Algorithm* chromagram;
    std::vector<float> input;
    std::vector<float> output;
    static inline constexpr int size { 2048 };
};