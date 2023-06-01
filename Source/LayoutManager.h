#pragma once

#include "Windows/WindowIDs.h"

class LayoutManager
{
public:
    explicit LayoutManager (juce::ValueTree& w) : windowLayout (w)
    {
        if (! layoutDirectory.exists())
        {
            layoutDirectory.createDirectory();
        }
        if (w.hasProperty (WindowIDs::Name))
        {
            layoutName = w.getPropertyAsValue (WindowIDs::Name, nullptr).toString();
        }
    }

    [[nodiscard]] static juce::StringArray getAllLayoutNames()
    {
        const auto files { layoutDirectory.findChildFiles (juce::File::TypesOfFileToFind::findFiles, false, "*.layout") };
        juce::StringArray presets {};
        for (const auto& file : files)
        {
            presets.add (file.getFileNameWithoutExtension());
        }
        return presets;
    }

    void saveLayout (const juce::String& name)
    {
        const auto layoutFile { layoutDirectory.getChildFile (juce::String { name + ".layout" }) };
        const auto xml { windowLayout.createXml() };
        xml->writeTo (layoutFile);
        layoutName = name;
    }

    void loadLayout (const juce::String& name)
    {
        const auto layoutFile { layoutDirectory.getChildFile (juce::String { name + ".layout" }) };
        if (layoutFile.existsAsFile())
        {
            juce::XmlDocument xmlDocument { layoutFile };
            const auto xmlElement { xmlDocument.getDocumentElement() };
            windowLayout.copyPropertiesAndChildrenFrom (juce::ValueTree::fromXml (*xmlElement), nullptr);
            layoutName = name;
        }
    }

    void deleteLayout (const juce::String&)
    {
    }

    [[nodiscard]] juce::String getCurrentLayoutName() const { return layoutName; }

    void setCurrentLayoutName (const juce::String& name) { layoutName = name; }

    //private:
    juce::ValueTree& windowLayout;
    juce::String layoutName {};
    static inline juce::File layoutDirectory { juce::File::getSpecialLocation (juce::File::SpecialLocationType::commonDocumentsDirectory).getChildFile ("AFV") };
};