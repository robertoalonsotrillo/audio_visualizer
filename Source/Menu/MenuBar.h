#pragma once

#include "../CustomLookAndFeel.h"
#include "../GraphSettingsView.h"
#include "../LayoutManager.h"
#include "../Windows/Window.h"
#include "ActiveGraphs.h"
#include "LayoutChooser.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_gui_extra/juce_gui_extra.h"

class MenuBar : public juce::Component,
                public juce::ApplicationCommandTarget,
                public juce::MenuBarModel
{
public:
    std::function<void (std::vector<float>)> onFileUpload;

    enum CommandIDs
    {
        UploadFile = 1,
        ImportData,
        ExportData,
        SaveLayout,
        LoadLayout,
        Audio,
        Graph
    };

    MenuBar (juce::AudioDeviceManager& a, juce::ValueTree& w, AlgorithmChain& chain, Config::ConfigData& configData) : audioDeviceManager (a), algorithmChain (chain), layoutManager (w), configData (configData)
    {
        menuComponent = std::make_unique<juce::MenuBarComponent> (this);
        addAndMakeVisible (menuComponent.get());
        setApplicationCommandManagerToWatch (&commandManager);
        commandManager.registerAllCommandsForTarget (this);
        commandManager.setFirstCommandTarget (this);
        addKeyListener (commandManager.getKeyMappings());

        setWantsKeyboardFocus (true);

#if JUCE_MAC
        MenuBarModel::setMacMainMenu (this);
#endif
    }
    ~MenuBar() override
    {
#if JUCE_MAC
        MenuBarModel::setMacMainMenu (nullptr);
#endif
        commandManager.setFirstCommandTarget (nullptr);
    }

    juce::StringArray getMenuBarNames() override
    {
        return { "Menu", "Layout", "Settings" };
    }

    juce::PopupMenu getMenuForIndex (int index, const juce::String& name) override
    {
        juce::PopupMenu menu;
        if (index == 0)
        {
            menu.addCommandItem (&commandManager, CommandIDs::UploadFile);
            menu.addCommandItem (&commandManager, CommandIDs::ImportData);
            menu.addCommandItem (&commandManager, CommandIDs::ExportData);
        }
        else if (index == 1)
        {
            menu.addCommandItem (&commandManager, CommandIDs::SaveLayout);
            menu.addSubMenu ("Load Layout", Menu::LayoutChooser { layoutManager });
            menu.addSubMenu ("Active Graphs", Menu::ActiveGraphs { layoutManager.windowLayout });
        }
        else if (index == 2)
        {
            menu.addCommandItem (&commandManager, CommandIDs::Audio);
            menu.addCommandItem (&commandManager, CommandIDs::Graph);
        }
        return menu;
    }

    void menuItemSelected (int menuID, int index) override
    {
        if (menuID == CommandIDs::UploadFile)
        {
            chooser = std::make_unique<juce::FileChooser> ("Select Mp3", juce::File::getSpecialLocation (juce::File::userHomeDirectory), "*.mp3");
            auto chooserFlags = juce::FileBrowserComponent::canSelectFiles;
            chooser->launchAsync (chooserFlags, [&] (const juce::FileChooser& fc)
                                  {
                                      const auto file { fc.getResult() };
                                      juce::AudioFormatManager formatManager;
                                      formatManager.registerFormat (new juce::MP3AudioFormat {}, true);
                                      const std::unique_ptr<juce::AudioFormatReader> reader { formatManager.createReaderFor (file) };
                                      if (reader != nullptr)
                                      {
                                          juce::AudioBuffer<float> offlineBuffer {};
                                          offlineBuffer.setSize (static_cast<int> (reader->numChannels), static_cast<int> (reader->lengthInSamples));
                                          reader->read (&offlineBuffer, 0, static_cast<int> (reader->lengthInSamples), 0, true, false);
                                          if (onFileUpload)
                                          {
                                              const auto leftChannelBuffer {offlineBuffer.getWritePointer(0)};
                                              onFileUpload({ leftChannelBuffer, leftChannelBuffer + offlineBuffer.getNumSamples() });
                                          }
                                      } });
        }
        else if (menuID == CommandIDs::ExportData)
        {
            chooser = std::make_unique<juce::FileChooser> ("Select a folder to export the data", juce::File::getSpecialLocation (juce::File::userHomeDirectory));
            auto chooser_flags { juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting };
            chooser->launchAsync (chooser_flags, [&] (const juce::FileChooser& fc)
                                  {
                                      auto file { fc.getResult() };
                                      file.deleteFile();
                                      const juce::String jsonString{algorithmChain.to_json().dump(4)};
                                      file.replaceWithText(jsonString); });
        }
        else if (menuID == CommandIDs::Audio)
        {
            auto contentComponent = std::make_unique<juce::AudioDeviceSelectorComponent> (audioDeviceManager, 1, 2, 1, 2, false, false, false, false);
            contentComponent->setSize (500, contentComponent->getHeight());
            auto* const topLevelComponent = getTopLevelComponent();
            juce::CallOutBox::launchAsynchronously (std::move (contentComponent), topLevelComponent->getLocalArea (this, menuComponent->getLocalBounds()), topLevelComponent);
        }
        else if (menuID == CommandIDs::Graph)
        {
            auto contentComponent = std::make_unique<GraphSettingsView> (configData, algorithmChain);
            auto* const topLevelComponent = getTopLevelComponent();
            juce::CallOutBox::launchAsynchronously (std::move (contentComponent), topLevelComponent->getLocalArea (this, menuComponent->getLocalBounds()), topLevelComponent);
        }
        else if (menuID == CommandIDs::SaveLayout)
        {
            juce::AlertWindow saveDialog { "Save Layout", "", juce::MessageBoxIconType::NoIcon };
            saveDialog.addTextEditor ("Layout Name", "");
            saveDialog.addButton ("Save", 1);
            saveDialog.addButton ("Cancel", 0);
            if (saveDialog.runModalLoop() == 1)
            {
                const auto layoutName { saveDialog.getTextEditor ("Layout Name")->getText() };
                layoutManager.saveLayout (layoutName);
            }
        }
    }

    ApplicationCommandTarget* getNextCommandTarget() override
    {
        return this;
    }

    void getAllCommands (juce::Array<juce::CommandID>& c) override
    {
        juce::Array<juce::CommandID> commands {
            CommandIDs::UploadFile,
            CommandIDs::ImportData,
            CommandIDs::ExportData,
            CommandIDs::SaveLayout,
            CommandIDs::LoadLayout,
            CommandIDs::Audio,
            CommandIDs::Graph
        };
        c.addArray (commands);
    }

    void getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result) override
    {
        switch (commandID)
        {
            case CommandIDs::UploadFile:
                result.setInfo ("Upload File", "Upload File", "Upload", 0);
                break;
            case CommandIDs::ImportData:
                result.setInfo ("Import Data", "Import Data", "Import", 0);
                break;
            case CommandIDs::ExportData:
                result.setInfo ("Export Data", "Export Data", "Export", 0);
                break;
            case CommandIDs::SaveLayout:
                result.setInfo ("Save Layout", "Save Layout", "Layout", 0);
                break;
            case CommandIDs::LoadLayout:
                result.setInfo ("Load Layout", "Load Layout", "Layout", 0);
                break;
            case CommandIDs::Audio:
                result.setInfo ("Audio", "Audio Settings", "Settings", 0);
                break;
            case CommandIDs::Graph:
                result.setInfo ("Graph", "Graph Settings", "Settings", 0);
                break;
        }
    }
    bool perform (const InvocationInfo& info) override
    {
        return true;
    }

    void resized() override { menuComponent->setBounds (getLocalBounds()); }

private:
#if JUCE_DEMO_RUNNER
    juce::ApplicationCommandManager& commandManager = getGlobalCommandManager();
#else
    juce::ApplicationCommandManager commandManager {};
#endif
    std::unique_ptr<juce::MenuBarComponent> menuComponent {};
    juce::AudioDeviceManager& audioDeviceManager;
    AlgorithmChain& algorithmChain;
    Config::ConfigData& configData;
    LayoutManager layoutManager;
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBar)
};