#include "GraphWindow.h"
#include "../MainComponent.h"

GraphWindow::GraphWindow (const juce::ValueTree& windowLayout_, const juce::Identifier& type_, MainComponent& mainComponent_) : Window (windowLayout_, type_, mainComponent_)
{
    if (type_ == WindowIDs::FFT)
    {
        //        graph = std::make_unique<Graph::SpectralMagnitude> (mainComponent_.getFFTBuffer());
    }
    else if (type_ == WindowIDs::Energy)
    {
        graph = std::make_unique<Graph::Energy> (std::get<Algorithm<Energy, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::Loudness)
    {
        graph = std::make_unique<Graph::Loudness> (std::get<Algorithm<Loudness, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::Scope)
    {
        graph = std::make_unique<Graph::Waveform> (mainComponent_.getScopeBuffer());
    }
    else if (type_ == WindowIDs::AmplitudeEnvelope)
    {
        graph = std::make_unique<Graph::AmplitudeEnvelope> (mainComponent_.getAmplitudeEnvelopeBuffer());
    }
    else if (type_ == WindowIDs::F0)
    {
        graph = std::make_unique<Graph::MonophonicPitchEstimator> (std::get<Algorithm<F0, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::Mel)
    {
        graph = std::make_unique<Graph::Mel> (std::get<Algorithm<Mel, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::MFCC)
    {
        graph = std::make_unique<Graph::MFCC> (std::get<Algorithm<MFCC, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::ConstantQChromagram)
    {
        graph = std::make_unique<Graph::ConstantQChromagram> (std::get<Algorithm<ConstantQChromagram, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::BFCC)
    {
        graph = std::make_unique<Graph::BFCC> (std::get<Algorithm<BFCC, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::Bark)
    {
        graph = std::make_unique<Graph::Bark> (std::get<Algorithm<Bark, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::SpectralFlux)
    {
        graph = std::make_unique<Graph::SpectralFlux> (std::get<Algorithm<SpectralFlux, AlgorithmChain::vector2d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::SpectralCentroid)
    {
        graph = std::make_unique<Graph::SpectralCentroid> (std::get<Algorithm<SpectralCentroid, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::SpectralRollOff)
    {
        graph = std::make_unique<Graph::SpectralRollOff> (std::get<Algorithm<SpectralRollOff, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::ZeroCrossingRate)
    {
        graph = std::make_unique<Graph::ZeroCrossingRate> (std::get<Algorithm<ZeroCrossingRate, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    else if (type_ == WindowIDs::HighFrequencyContent)
    {
        graph = std::make_unique<Graph::HighFrequencyContent> (std::get<Algorithm<HighFrequencyContent, AlgorithmChain::vector1d>> (mainComponent_.chain.algorithms).container);
    }
    addAndMakeVisible (graph.get());
}

void GraphWindow::resized()
{
    Window::resized();
    if (graph)
    {
        graph->setBounds (getContentBounds());
    }
}
