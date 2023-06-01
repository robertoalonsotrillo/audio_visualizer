#pragma once

#include "AmplitudeEnvelope.h"
#include "BFCC.h"
#include "Bark.h"
#include "ConstantQChromagram.h"
#include "Energy.h"
#include "F0.h"
#include "FftMagnitude.h"
#include "HighFrequencyContent.h"
#include "Loudness.h"
#include "MFCC.h"
#include "Mel.h"
#include "Rms.h"
#include "SpectralCentroid.h"
#include "SpectralFlux.h"
#include "SpectralRollOff.h"
#include "Spectrum.h"
#include "ZeroCrossingRate.h"
#include "nlohmann/json.hpp"
#include <tuple>

template <class Alg, typename Container>
struct Algorithm
{
    Alg alg;
    Container container;

    void create()
    {
        alg.initialise();
    }

    void setSampleRate (const float sampleRate)
    {
        //        alg.setSampleRate (sampleRate);
    }

    void process (const std::vector<float>& input)
    {
        const auto output { alg.processed (input) };
        if constexpr (! std::is_same<Alg, AmplitudeEnvelope>())
        {
            container.emplace_back (output);
        }
    }

    void processOffline (const std::vector<float>& input)
    {
        container = alg.processedOffline (input);
    }

    void setOscPort (const int portNumber)
    {
        alg.sender.disconnect();
        alg.sender.connect ("127.0.0.1", portNumber);
    }

    void reset()
    {
        container.clear();
    }
};

struct AlgorithmChain
{
    using vector1d = std::vector<float>;
    using vector2d = std::vector<std::vector<float>>;
    std::tuple<Algorithm<AmplitudeEnvelope, vector1d>, Algorithm<Energy, vector1d>, Algorithm<Loudness, vector1d>, Algorithm<F0, vector1d>, Algorithm<Rms, vector1d>, Algorithm<SpectralCentroid, vector1d>, Algorithm<SpectralRollOff, vector1d>, Algorithm<ZeroCrossingRate, vector1d>, Algorithm<HighFrequencyContent, vector1d>, Algorithm<ConstantQChromagram, vector2d>, Algorithm<Mel, vector2d>, Algorithm<MFCC, vector2d>, Algorithm<BFCC, vector2d>, Algorithm<Bark, vector2d>, Algorithm<SpectralFlux, vector2d>> algorithms;

    void create()
    {
        std::apply ([] (auto&&... x)
                    { (x.create(), ...); },
                    algorithms);
    }

    void setSampleRate (const float sampleRate)
    {
        std::apply ([sampleRate] (auto&&... x)
                    { (x.setSampleRate (sampleRate), ...); },
                    algorithms);
    }

    void process (const std::vector<float>& input)
    {
        std::apply ([&input] (auto&&... x)
                    { (x.process (input), ...); },
                    algorithms);
    }

    nlohmann::json to_json()
    {
        nlohmann::json json;
        json["Window Size"] = 2048;
        json["Overlap %"] = 50.0;
        json["Sample Rate"] = 44'100.0;
        json["Energy"] = std::get<Algorithm<Energy, vector1d>> (algorithms).container;
        json["Loudness"] = std::get<Algorithm<Loudness, vector1d>> (algorithms).container;
        json["F0"] = std::get<Algorithm<F0, vector1d>> (algorithms).container;
        json["Rms"] = std::get<Algorithm<Rms, vector1d>> (algorithms).container;
        json["Spectral Centroid"] = std::get<Algorithm<SpectralCentroid, vector1d>> (algorithms).container;
        json["Spectral Roll Off"] = std::get<Algorithm<SpectralRollOff, vector1d>> (algorithms).container;
        json["Zero Crossing Rate"] = std::get<Algorithm<ZeroCrossingRate, vector1d>> (algorithms).container;
        json["High Frequency Content"] = std::get<Algorithm<HighFrequencyContent, vector1d>> (algorithms).container;
        json["Constant Q Chromagram"] = std::get<Algorithm<ConstantQChromagram, vector2d>> (algorithms).container;
        json["Mel"] = std::get<Algorithm<Mel, vector2d>> (algorithms).container;
        json["MFCC"] = std::get<Algorithm<MFCC, vector2d>> (algorithms).container;
        json["BFCC"] = std::get<Algorithm<BFCC, vector2d>> (algorithms).container;
        json["Bark"] = std::get<Algorithm<Bark, vector2d>> (algorithms).container;
        json["Spectral Flux"] = std::get<Algorithm<SpectralFlux, vector2d>> (algorithms).container;
        return json;
    }

    //    void processOffline (const std::vector<float>& input)
    //    {
    //        std::apply ([&input] (auto&&... x)
    //                    { (x.processOffline (input), ...); },
    //                    algorithms);
    //    }

    void setOscPort (const int portNumber)
    {
        std::apply ([portNumber] (auto&&... x)
                    { (x.setOscPort (portNumber), ...); },
                    algorithms);
    }

    void reset()
    {
        std::apply ([] (auto&&... x)
                    { (x.reset(), ...); },
                    algorithms);
    }
};