#pragma once

#include "../LayoutManager.h"
#include "../Windows/Window.h"
#include "../Windows/WindowIDs.h"

namespace Menu
{
struct LayoutChooser : public juce::PopupMenu
{
    explicit LayoutChooser (LayoutManager& layoutManager)
    {
        const auto layoutNames { LayoutManager::getAllLayoutNames() };
        for (const auto& layoutName : layoutNames)
        {
            addItem (layoutName, [layoutName, &layoutManager]
                     { layoutManager.loadLayout (layoutName); });
        }
    }
};
} // namespace Menu