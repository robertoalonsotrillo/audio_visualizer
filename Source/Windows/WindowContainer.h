#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include "WindowIDs.h"

class MainComponent;

class WindowContainer : public juce::Component,
                        private juce::ValueTree::Listener
{
public:
    WindowContainer(MainComponent& mainComponent_, juce::ValueTree layoutTree_);

    ~WindowContainer() override;

    void resized() override;

private:
    MainComponent& mainComponent;
    juce::ValueTree layoutTree;
    const bool isVertical = layoutTree.hasType(WindowIDs::Column);

    std::vector<std::shared_ptr<juce::Component>> childComponents;

    class ResizeBar;

    std::vector<std::unique_ptr<ResizeBar>> resizeBars;

    int totalWindowSize = 0;

    void componentAddedOrRemoved();

    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override;

    void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;

    void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int) override;

    void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowContainer)
};
