#pragma once

#include "Graph2d.h"
#include "Graph3d.h"

namespace Graph
{
struct Loudness : Graph2d
{
    explicit Loudness (const std::vector<float>& b) : Graph2d ("Time (s)", "Amplitude", { { 0.0f, 5.0f }, { -80.0f, 100.0f } }, b) {}
};

struct Energy : Graph2d
{
    explicit Energy (const std::vector<float>& b) : Graph2d ("Time (s)", "Amplitude", { { 0.0f, 5.0f }, { -80.0f, 100.0f } }, b) {}
};

struct MonophonicPitchEstimator : Graph2d
{
    explicit MonophonicPitchEstimator (const std::vector<float>& b) : Graph2d ("Time (s)", "Pitch", { { 0.0f, 5.0f }, { std::log10 (20.0f), std::log10 (44'100.0f / 2.0f) } }, b, true) {}
};

struct AmplitudeEnvelope : Graph2d
{
    explicit AmplitudeEnvelope (const std::vector<float>& b) : Graph2d ("Time (s)", "Amplitude", { { 0.0f, 44'100.0f }, { 0.0f, 1.0f } }, b) {}
};

struct Waveform : Graph2d
{
    explicit Waveform (const std::vector<float>& b) : Graph2d ("Time (s)", "Amplitude", { { 0.0f, 44'100.0f }, { -1.0f, 1.0f } }, b) {}
};

struct SpectralCentroid : Graph2d
{
    explicit SpectralCentroid (const std::vector<float>& b) : Graph2d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { std::log10 (20.0f), std::log10 (44'100.0f / 2.0f) } }, b, true) {}
};

struct SpectralRollOff : Graph2d
{
    explicit SpectralRollOff (const std::vector<float>& b) : Graph2d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { std::log10 (20.0f), std::log10 (44'100.0f / 2.0f) } }, b, true) {}
};

struct ZeroCrossingRate : Graph2d
{
    explicit ZeroCrossingRate (const std::vector<float>& b) : Graph2d ("Time (s)", "Rate", { { 0.0f, 5.0f }, { 0.0f, 1.0f } }, b) {}
};

struct HighFrequencyContent : Graph2d
{
    explicit HighFrequencyContent (const std::vector<float>& b) : Graph2d ("Time (s)", " ", { { 0.0f, 5.0f }, { 0.0f, 1'000'000.0f } }, b) {}
};

struct Mel : Graph3d<24>
{
    explicit Mel (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { 0.0f, 44'100.0f / 2.0f } }, b) {}
};

struct MFCC : Graph3d<13>
{
    explicit MFCC (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Coefficient", { { 0.0f, 5.0f }, { 0.0f, 12.0f } }, b) {}
};

struct SpectralMagnitude : Graph3d<2048 / 2 + 1>
{
    explicit SpectralMagnitude (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { std::log10 (20.0f), std::log10 (44'100.0f / 2.0f) } }, b) {}
};

struct ConstantQChromagram : Graph3d<12>
{
    explicit ConstantQChromagram (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { 0.0f, 11.0f } }, b) {}
};

struct BFCC : Graph3d<13>
{
    explicit BFCC (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { 0.0f, 12.0f } }, b) {}
};

struct Bark : Graph3d<27>
{
    explicit Bark (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { 0.0f, 26.0f } }, b) {}
};

struct SpectralFlux : Graph3d<28>
{
    explicit SpectralFlux (const std::vector<std::vector<float>>& b) : Graph3d ("Time (s)", "Frequency", { { 0.0f, 5.0f }, { 0.0f, 27.0f } }, b) {}
};
} // namespace Graph