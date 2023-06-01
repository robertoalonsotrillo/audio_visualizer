#pragma once

#include "Algorithms/AlgorithmChain.h"
#include "Algorithms/AmplitudeEnvelope.h"
#include "CustomLookAndFeel.h"
#include "DragOverlay.h"
#include "LockFreeFifo.h"
#include "Menu/MenuBar.h"
#include "Windows/GraphWindow.h"
#include "Windows/WindowContainer.h"
#include "Windows/WindowIDs.h"
#include <essentia.h> // TODO Disable warnings from essentia
#include <juce_audio_utils/juce_audio_utils.h>

class MainComponent : public juce::AudioAppComponent, private juce::AsyncUpdater
{
public:
    juce::VBlankAttachment vBlankAttachment { this, [this]
                                              { update(); } };
    MainComponent();

    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    void cleanupWindows();
    std::shared_ptr<GraphWindow> getOrCreateWindow (const juce::ValueTree& windowTree);

    void showDragOverlayAtComponent (juce::Component& component, DragOverlay::Side side);
    void hideDragOverlay();
    DragOverlay* getCurrentlyShowingDragOverlay();

    const std::vector<float>& getScopeBuffer() const { return scopeBuffer; }
    const std::vector<float>& getAmplitudeEnvelopeBuffer() const { return amplitudeEnvelopeBuffer; }

    AlgorithmChain chain;

private:
    juce::ValueTree windowLayout { WindowIDs::Row };
    std::map<juce::Identifier, std::shared_ptr<GraphWindow>> windows;
    WindowContainer windowContainer { *this, windowLayout };
    std::unique_ptr<DragOverlay> dragOverlay;
    Config::ConfigData configData {};
    MenuBar menu { deviceManager, windowLayout, chain, configData };

    LockFreeFifo<float, 16384> fifo;

    std::vector<float> scopeBuffer;
    std::vector<float> amplitudeEnvelopeBuffer;

    juce::ToggleButton offlineProcessingButton { "Offline" };
    juce::ToggleButton followButton { "Follow" };
    juce::ToggleButton oscButton {"OSC"};

    juce::TextButton recordButton {"Record"};

    std::deque<float> masterBuffer = std::deque<float> (16384);
    std::vector<float> masterBufferContinuous {};
    AmplitudeEnvelope amplitudeEnvelope;

    void update();

    void handleAsyncUpdate() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
