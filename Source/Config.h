#pragma once

#include "juce_data_structures/juce_data_structures.h"
#include "juce_graphics/juce_graphics.h"

namespace Config
{
extern juce::Colour path;
extern juce::Colour background;
extern float pathWidth;
extern bool drawGrid;
extern float smoothness;
extern size_t windowSize;
extern float overlap;
extern bool is_osc_active;
extern int oscPort;

struct ConfigData
{
    ConfigData()
    {
        auto file_options = juce::PropertiesFile::Options();
        file_options.applicationName = "AFV"; // Access it from CMake variable
        file_options.folderName = "AFV";
        file_options.filenameSuffix = "settings";
        file_options.osxLibrarySubFolder = "Application Support";
        file_options.storageFormat = juce::PropertiesFile::StorageFormat::storeAsBinary;
        properties_file = std::make_unique<juce::PropertiesFile> (file_options);
        properties_file->setValue ("Path Colour", path.toString());
        properties_file->setValue ("Background Colour", background.toString());
        properties_file->setValue ("Path Width", pathWidth);
        properties_file->setValue ("Draw Grid", drawGrid);
        properties_file->setValue ("Smoothness", smoothness);
        properties_file->setValue ("OscPort", oscPort);
    }
    void saveState() const
    {
        properties_file->setValue ("Path Colour", path.toString());
        properties_file->setValue ("Background Colour", background.toString());
        properties_file->setValue ("Path Width", pathWidth);
        properties_file->setValue ("Draw Grid", drawGrid);
        properties_file->setValue ("Smoothness", smoothness);
        properties_file->setValue ("OscPort", oscPort);
    }

    void loadState() const
    {
        path = juce::Colour::fromString (properties_file->getValue ("Path Colour"));
        background = juce::Colour::fromString (properties_file->getValue ("Background Colour"));
        pathWidth = static_cast<float> (properties_file->getDoubleValue ("Path Width"));
        drawGrid = properties_file->getBoolValue ("Draw Grid");
        smoothness = static_cast<float> (properties_file->getDoubleValue ("Smoothness"));
        oscPort = properties_file->getIntValue ("OscPort");
    }

    std::unique_ptr<juce::PropertiesFile>
        properties_file;
};
} // namespace Config
