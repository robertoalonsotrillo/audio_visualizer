#pragma once

#include "../Windows/Window.h"
#include "../Windows/WindowIDs.h"

namespace Menu
{
struct ActiveGraphs : public juce::PopupMenu
{
    explicit ActiveGraphs (juce::ValueTree& windowLayout)
    {
        const auto addWindowItem = [&] (const auto& id)
        {
            const auto toggled { ::Window::findWindow (windowLayout, id).isValid() };
            addItem (getWindowTitle (id), true, toggled, [id = id, toggled, &windowLayout]
                     {
                                      if (toggled)
                                      {
                                          for (auto child = ::Window::findWindow(windowLayout, id); child.isValid() && child.getNumChildren() == 0;)
                                          {
                                              auto parent = child.getParent();
                                              parent.removeChild(child, nullptr);
                                              child = parent;
                                          }
                                          return;
                                      }
                                      auto totalWindowSizes = 0.0f;
                                      for (const auto window : windowLayout.getRoot())
                                      {
                                          totalWindowSizes += (float) window.getProperty(WindowIDs::size, 1.0f);
                                      }
                                      const auto newSize = (totalWindowSizes == 0.0f) ? 1.0f : (totalWindowSizes / (float) windowLayout.getRoot().getNumChildren());
                                      windowLayout.getRoot().appendChild({ id, {{ WindowIDs::size, newSize }}}, nullptr); });
        };
        addSectionHeader ("2d Graphs");
        addWindowItem (WindowIDs::Energy);
        addWindowItem (WindowIDs::Loudness);
        addWindowItem (WindowIDs::Scope);
        addWindowItem (WindowIDs::HighFrequencyContent);
        addWindowItem (WindowIDs::SpectralRollOff);
        addWindowItem (WindowIDs::ZeroCrossingRate);
        addWindowItem (WindowIDs::SpectralCentroid);
        addWindowItem (WindowIDs::AmplitudeEnvelope);
        addWindowItem (WindowIDs::F0);
        addSectionHeader ("3d Graphs");
        addWindowItem (WindowIDs::Mel);
        addWindowItem (WindowIDs::MFCC);
        addWindowItem (WindowIDs::ConstantQChromagram);
        addWindowItem (WindowIDs::BFCC);
        addWindowItem (WindowIDs::Bark);
        addWindowItem (WindowIDs::SpectralFlux);
        addWindowItem (WindowIDs::FFT);
    }
};
} // namespace Menu