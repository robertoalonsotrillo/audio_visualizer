#pragma once

#include "Graph/Graphs.h"
#include "Window.h"

class GraphWindow : public Window
{
public:
    GraphWindow (const juce::ValueTree& windowLayout_, const juce::Identifier& type_, MainComponent& mainComponent_);

    void resized() override;

    std::unique_ptr<GraphBase> graph;
};
