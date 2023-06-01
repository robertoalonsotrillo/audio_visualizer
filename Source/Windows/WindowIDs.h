#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace WindowIDs
{
#define DECLARE_ID(name) static const juce::Identifier name (#name);

DECLARE_ID (Column)
DECLARE_ID (Row)

DECLARE_ID (FFT)
DECLARE_ID (LevelGraph)
DECLARE_ID (LevelMeter)
DECLARE_ID (Energy)
DECLARE_ID (Loudness)
DECLARE_ID (Scope)
DECLARE_ID (Tuner)
DECLARE_ID (AmplitudeEnvelope)
DECLARE_ID (F0)
DECLARE_ID (Mel)
DECLARE_ID (MFCC)
DECLARE_ID (ConstantQChromagram)
DECLARE_ID (BFCC)
DECLARE_ID (Bark)
DECLARE_ID (SpectralFlux)
DECLARE_ID (SpectralCentroid)
DECLARE_ID (SpectralRollOff)
DECLARE_ID (ZeroCrossingRate)
DECLARE_ID (HighFrequencyContent)

DECLARE_ID (size)

DECLARE_ID(Name)

#undef DECLARE_ID
} // namespace WindowIDs

inline juce::String getWindowTitle (const juce::Identifier& windowType)
{
    static std::map<juce::Identifier, juce::String> map {
        { WindowIDs::FFT, "FFT" },
        { WindowIDs::Energy, "Energy" },
        { WindowIDs::Loudness, "Loudness" },
        { WindowIDs::Scope, "Scope" },
        { WindowIDs::AmplitudeEnvelope, "Amplitude Envelope" },
        { WindowIDs::F0, "F0" },
        { WindowIDs::Mel, "Mel" },
        { WindowIDs::MFCC, "MFCC" },
        { WindowIDs::ConstantQChromagram, "Chromagram" },
        { WindowIDs::BFCC, "BFCC" },
        { WindowIDs::Bark, "Bark" },
        { WindowIDs::SpectralFlux, "Spectral Flux" },
        { WindowIDs::SpectralCentroid, "Spectral Centroid" },
        { WindowIDs::SpectralRollOff, "Spectral Roll Off" },
        { WindowIDs::ZeroCrossingRate, "Zero Crossing Rate" },
        { WindowIDs::HighFrequencyContent, "High Frequency Content" }
    };

    return map[windowType];
}
