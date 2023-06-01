#include "MainComponent.h"
#include "Windows/GraphWindow.h"

using namespace essentia::standard;

MainComponent::MainComponent()
{
#if JUCE_DEBUG
    juce::UnitTestRunner runner;
    runner.runAllTests();
#endif

    addAndMakeVisible (windowContainer);
    addAndMakeVisible (offlineProcessingButton);
    addAndMakeVisible (followButton);
    addAndMakeVisible (oscButton);
    addAndMakeVisible (recordButton);

    recordButton.setClickingTogglesState (true);

    juce::ValueTree layout = { WindowIDs::Row, {}, { { WindowIDs::Column, {}, {
                                                                                  { WindowIDs::Row, {}, {
                                                                                                            //                                                                                                            { WindowIDs::Mel, {} },
                                                                                                            //                                                                                                            { WindowIDs::AmplitudeEnvelope, {} },
                                                                                                            { WindowIDs::Loudness, {} },
                                                                                                            { WindowIDs::Energy, {} },
                                                                                                        } },
                                                                                  { WindowIDs::Row, {}, {
                                                                                                            //                                                                                                            { WindowIDs::MFCC, {} },
                                                                                                            { WindowIDs::F0, {} },
                                                                                                            //                                                                                                            { WindowIDs::ConstantQChromagram, {} },
                                                                                                            //                                                                                                            { WindowIDs::FFT, {} },
                                                                                                            //                                                                                                            { WindowIDs::Scope, {} },
                                                                                                        } },
                                                                                  //                                                                                  { WindowIDs::Row, {}, {
                                                                                  //                                                                                                            { WindowIDs::BFCC, {} },
                                                                                  //                                                                                                            { WindowIDs::Bark, {} },
                                                                                  //                                                                                                            { WindowIDs::SpectralFlux, {} },
                                                                                  //                                                                                                            { WindowIDs::SpectralCentroid, {} },
                                                                                  //                                                                                                        } },
                                                                                  //                                                                                  { WindowIDs::Row, {}, {
                                                                                  //                                                                                                            { WindowIDs::SpectralRollOff, {} },
                                                                                  //                                                                                                            { WindowIDs::ZeroCrossingRate, {} },
                                                                                  //                                                                                                            { WindowIDs::HighFrequencyContent, {} },
                                                                                  //                                                                                                        } },
                                                                              } } } };
    layout.setProperty (WindowIDs::Name, "Default Layout", nullptr);
    windowLayout.copyPropertiesAndChildrenFrom (layout, nullptr);

    addAndMakeVisible (menu);

    configData.loadState();

    followButton.setToggleState (true, juce::dontSendNotification);

    menu.onFileUpload = [this] (const std::vector<float>& b)
    {
        scopeBuffer = b;
    };

    oscButton.onClick = [this]
    {
        Config::is_osc_active = oscButton.getToggleState();
    };

    recordButton.onClick = [this]
    {
        if (recordButton.getToggleState())
        {
            chain.reset();
            scopeBuffer.clear();
        }
    };

    setSize (1280, 768);

    setAudioChannels (2, 2);

    essentia::init();

    amplitudeEnvelope.initialise();
    chain.create();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (Config::background);
}

void MainComponent::resized()
{
    auto r = getLocalBounds();
    menu.setBounds (r.removeFromTop (getLookAndFeel().getDefaultMenuBarHeight()));
    auto b { r.removeFromTop (20) };
    offlineProcessingButton.setBounds (b.removeFromRight (100));
    followButton.setBounds (b.removeFromRight (100));
    oscButton.setBounds (b.removeFromRight (100));
    recordButton.setBounds (b.removeFromRight (100));
    windowContainer.setBounds (r);
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // todo essentia is trash
    chain.setSampleRate (static_cast<float> (sampleRate));
}

void MainComponent::releaseResources()
{
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    for (int i {}; i < bufferToFill.buffer->getNumSamples(); ++i)
    {
        const float random = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) * 2.0f - 1.0f);
        fifo.push (bufferToFill.buffer->getSample (0, i));
    }
    bufferToFill.buffer->clear();
}

void MainComponent::cleanupWindows()
{
    triggerAsyncUpdate();
}

std::shared_ptr<GraphWindow> MainComponent::getOrCreateWindow (const juce::ValueTree& windowTree)
{
    const auto type = windowTree.getType();
    auto& window = windows[type];
    if (window != nullptr)
    {
        return window;
    }

    return window = std::make_shared<GraphWindow> (windowLayout, windowTree.getType(), *this);
}

void MainComponent::showDragOverlayAtComponent (juce::Component& component, DragOverlay::Side side)
{
    if (dragOverlay == nullptr || dragOverlay->getTarget() != &component)
    {
        dragOverlay.reset();
        dragOverlay = std::make_unique<DragOverlay> (component);
        component.addAndMakeVisible (*dragOverlay);
    }

    dragOverlay->setSide (side);
}

void MainComponent::hideDragOverlay()
{
    dragOverlay.reset();
}

DragOverlay* MainComponent::getCurrentlyShowingDragOverlay()
{
    return dragOverlay.get();
}

void MainComponent::update()
{
    if (recordButton.getToggleState())
    {
        float val;
        static size_t accumulator {};
        const size_t samplesToPull { static_cast<size_t> ((Config::windowSize * (1.0f - Config::overlap * 0.01f))) };
        while (fifo.pull (val))
        {
            masterBuffer.push_front (val);
            masterBuffer.pop_back();
            scopeBuffer.emplace_back (val);
            masterBufferContinuous.emplace_back (val);
            if (++accumulator == samplesToPull)
            {
                chain.process (std::vector<float> (masterBuffer.begin(), masterBuffer.begin() + Config::windowSize));
                accumulator = 0;
            }
        }
        if (! offlineProcessingButton.getToggleState())
        {
            const auto a { amplitudeEnvelope.processed (masterBufferContinuous) };
            amplitudeEnvelopeBuffer.insert (amplitudeEnvelopeBuffer.end(), a.begin(), a.end());
            masterBufferContinuous.clear();
            if (followButton.getToggleState())
            {
                for (const auto& graphWindow : windows)
                {
                    if (graphWindow.second != nullptr)
                    {
                        graphWindow.second->graph->updateFollow();
                    }
                }
            }
            for (const auto& graphWindow : windows)
            {
                if (graphWindow.second != nullptr)
                {
                    if (graphWindow.first != WindowIDs::FFT)
                    {
                        graphWindow.second->graph->update();
                    }
                }
            }
        }
    }
}

void MainComponent::handleAsyncUpdate()
{
    for (auto& window : windows)
    {
        // Destroy if the use_count is 1, since it means no one is pointing to it except us
        if (window.second.use_count() == 1)
        {
            window.second.reset();
        }
    }
}
