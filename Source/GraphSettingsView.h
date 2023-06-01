#pragma once

#include "Config.h"
#include "juce_gui_extra/juce_gui_extra.h"

class GraphSettingsView : public juce::Component, public juce::ChangeListener
{
public:
    GraphSettingsView (Config::ConfigData& configData, AlgorithmChain& chain) : configData (configData)
    {
        addAndMakeVisible (backgroundColourLabel);
        addAndMakeVisible (pathColourLabel);
        addAndMakeVisible (pathWidthLabel);
        addAndMakeVisible (drawGridLabel);
        addAndMakeVisible (smoothnessLabel);
        addAndMakeVisible (windowSizeLabel);
        addAndMakeVisible (overlapLabel);
        addAndMakeVisible (backgroundColourButton);
        addAndMakeVisible (pathColourButton);
        addAndMakeVisible (pathWidthSlider);
        addAndMakeVisible (drawGridButton);
        addAndMakeVisible (smoothnessSlider);
        //        addAndMakeVisible (windowSizeComboBox);
        //        addAndMakeVisible (overlapComboBox);
        addAndMakeVisible (oscPortNumberLabel);
        addAndMakeVisible (oscPortEditor);

        backgroundColourButton.setColour (juce::TextButton::ColourIds::buttonColourId, Config::background);
        pathColourButton.setColour (juce::TextButton::ColourIds::buttonColourId, Config::path);
        pathWidthSlider.setRange (1.0, 5.0);
        pathWidthSlider.setValue (Config::pathWidth);
        drawGridButton.setToggleState (Config::drawGrid, juce::dontSendNotification);
        smoothnessSlider.setRange (0.2, 1.0);
        smoothnessSlider.setValue (Config::smoothness);
        windowSizeComboBox.setText (juce::String { Config::windowSize });
        overlapComboBox.setText (juce::String { Config::overlap } + "%");
        oscPortEditor.setEditable (true);

        oscPortEditor.setText (juce::String { Config::oscPort }, juce::dontSendNotification);

        windowSizeComboBox.addItemList ({ "128", "256", "512", "1024", "2048", "4096", "8192", "16384" }, 1);
        overlapComboBox.addItemList ({ "0%", "50%", "75%", "87.5%", "93.75%" }, 1);

        backgroundColourButton.onClick = [&]
        {
            colourSelector = std::make_unique<juce::ColourSelector> (juce::ColourSelector::ColourSelectorOptions::showColourspace, 0, 0);
            colourSelector->setName ("Background Colour");
            colourSelector->setSize (200, 200);
            colourSelector->addChangeListener (this);
            colourSelector->setCurrentColour (Config::background);
            juce::CallOutBox::launchAsynchronously (std::move (colourSelector), getTopLevelComponent()->getLocalArea (this, backgroundColourButton.getLocalBounds()), getTopLevelComponent());
        };

        pathColourButton.onClick = [&]
        {
            colourSelector = std::make_unique<juce::ColourSelector> (juce::ColourSelector::ColourSelectorOptions::showColourspace, 0, 0);
            colourSelector->setName ("Path Colour");
            colourSelector->setSize (200, 200);
            colourSelector->setCurrentColour (Config::path);
            colourSelector->addChangeListener (this);
            juce::CallOutBox::launchAsynchronously (std::move (colourSelector), getTopLevelComponent()->getLocalArea (this, pathColourButton.getLocalBounds()), getTopLevelComponent());
        };

        pathWidthSlider.onValueChange = [&]
        {
            Config::pathWidth = static_cast<float> (pathWidthSlider.getValue());
            configData.saveState();
        };

        drawGridButton.onClick = [&]
        {
            Config::drawGrid = drawGridButton.getToggleState();
            configData.saveState();
        };

        smoothnessSlider.onValueChange = [&]
        {
            Config::smoothness = static_cast<float> (smoothnessSlider.getValue());
            configData.saveState();
        };

        windowSizeComboBox.onChange = [&]
        {
            Config::windowSize = static_cast<size_t> (windowSizeComboBox.getText().getIntValue());
        };

        overlapComboBox.onChange = [&]
        {
            auto overlapValueText { overlapComboBox.getText() };
            overlapValueText.dropLastCharacters (1);
            Config::overlap = overlapValueText.getFloatValue();
        };

        oscPortEditor.onEditorHide = [&]
        {
            Config::oscPort = oscPortEditor.getText().getIntValue();
            configData.saveState();
            chain.setOscPort (Config::oscPort);
        };

        setSize (240, 160);
    }

private:
    void resized() override
    {
        backgroundColourLabel.setBounds (0, 0, 130, 20);
        pathColourLabel.setBounds (0, 20, 130, 20);
        pathWidthLabel.setBounds (0, 40, 130, 20);
        drawGridLabel.setBounds (0, 60, 130, 20);
        smoothnessLabel.setBounds (0, 80, 130, 20);
        windowSizeLabel.setBounds (0, 100, 130, 20);
        overlapLabel.setBounds (0, 120, 130, 20);
        oscPortNumberLabel.setBounds (0, 140, 130, 20);

        backgroundColourButton.setBounds (140, 0, 20, 20);
        pathColourButton.setBounds (140, 20, 20, 20);
        pathWidthSlider.setBounds (140, 40, 100, 20);
        drawGridButton.setBounds (140, 60, 20, 20);
        smoothnessSlider.setBounds (140, 80, 100, 20);
        windowSizeComboBox.setBounds (140, 100, 100, 20);
        overlapComboBox.setBounds (140, 120, 100, 20);
        oscPortEditor.setBounds (140, 140, 100, 20);
    }

    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
        if (auto* cs = dynamic_cast<juce::ColourSelector*> (source))
        {
            if (cs->getName() == "Background Colour")
            {
                Config::background = cs->getCurrentColour();
                backgroundColourButton.setColour (juce::TextButton::ColourIds::buttonColourId, Config::background);
                configData.saveState();
            }
            else
            {
                Config::path = cs->getCurrentColour();
                pathColourButton.setColour (juce::TextButton::ColourIds::buttonColourId, Config::path);
                configData.saveState();
            }
        }
    }

    juce::Label backgroundColourLabel { "Background Colour", "Background Colour" };
    juce::Label pathColourLabel { "Path Colour", "Path Colour" };
    juce::Label pathWidthLabel { "Path Width", "Path Width" };
    juce::Label drawGridLabel { "Draw Grid", "Draw Grid" };
    juce::Label smoothnessLabel { "Smoothness", "Smoothness" };
    juce::Label windowSizeLabel { "Window Size", "Window Size" };
    juce::Label overlapLabel { "Overlap", "Overlap" };
    juce::Label oscPortNumberLabel { "Osc Port", "Osc Port" };

    juce::TextButton backgroundColourButton {};
    juce::TextButton pathColourButton {};
    juce::Slider pathWidthSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::NoTextBox };
    juce::ToggleButton drawGridButton {};
    juce::Slider smoothnessSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::NoTextBox };
    juce::ComboBox windowSizeComboBox {};
    juce::ComboBox overlapComboBox {};
    std::unique_ptr<juce::ColourSelector> colourSelector {};
    juce::Label oscPortEditor {};
    Config::ConfigData& configData;
};