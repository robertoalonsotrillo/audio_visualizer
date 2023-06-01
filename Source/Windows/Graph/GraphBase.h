#pragma once

#include "Behaviours/Hoverer.h"
#include "Behaviours/Zoomer.h"
#include "Grid.h"
#include <juce_gui_basics/juce_gui_basics.h>

class GraphBase : public juce::Component
{
public:
    GraphBase (const juce::String& xLabelText, const juce::String& yLabelText, const Limit2d l) : limits (l), fixedLimits (l)
    {
        setOpaque (true);
        addAndMakeVisible (hovering);
        addAndMakeVisible (xAxisLabel);
        addAndMakeVisible (yAxisLabel);
        addAndMakeVisible (zoomRectangle);
        xAxisLabel.setText (xLabelText, juce::dontSendNotification);
        yAxisLabel.setText (yLabelText, juce::dontSendNotification);
        xAxisLabel.setJustificationType (juce::Justification::centred);
        yAxisLabel.setJustificationType (juce::Justification::centred);
    }

    virtual void paintGraph (juce::Graphics& g) = 0;

    virtual void buildPath() = 0;

    virtual void updateFollow() = 0;

    void update()
    {
        buildPath();
        repaint();
    }

private:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (Config::background);
        g.setColour (juce::Colours::white.withAlpha (0.5f));
        if (Config::drawGrid)
        {
            grid.draw (g, limits.y, Grid::Direction::Horizontal);
            grid.draw (g, limits.x, Grid::Direction::Vertical);
        }
        paintGraph (g);
    }

    void mouseMove (const juce::MouseEvent& event) override
    {
        setMouseCursor (topGridLabelBounds.contains (event.position) ? juce::MouseCursor::LeftRightResizeCursor : juce::MouseCursor::NormalCursor);
        hovering.update (getMouseXYRelative().toFloat(), limits, { { plotBounds.getX(), plotBounds.getRight() }, { plotBounds.getY(), plotBounds.getBottom() } }, grid.draw_notes);
    }

    void mouseExit (const juce::MouseEvent& event) override
    {
        hovering.setVisible (false);
    }

    void mouseDown (const juce::MouseEvent& event) override
    {
        if (event.mods.isLeftButtonDown())
        {
            leftMouseDownPosition = event.position;
            isLeftMouseButtonDown = true;
        }
        else if (event.mods.isRightButtonDown())
        {
            rightMouseDownPosition = event.position;
        }
    }

    void mouseDrag (const juce::MouseEvent& event) override
    {
        if (event.mods.isLeftButtonDown() && plotBounds.contains (leftMouseDownPosition))
        {
            zoomRectangle.update ({ leftMouseDownPosition.toInt(), event.getPosition() });
        }
        hovering.update (getMouseXYRelative().toFloat(), limits, { { plotBounds.getX(), plotBounds.getRight() }, { plotBounds.getY(), plotBounds.getBottom() } }, grid.draw_notes);
        if (event.mods.isRightButtonDown() && plotBounds.contains (rightMouseDownPosition))
        {
            const auto x { juce::jmap (event.position.x - rightMouseDownPosition.x, 0.0f, static_cast<float> (getWidth()), 0.0f, limits.x.max - limits.x.min) };
            limits.x.min -= x;
            limits.x.max -= x;
            rightMouseDownPosition = event.position;
            update();
        }
        else if (event.mods.isLeftButtonDown() && topGridLabelBounds.contains (leftMouseDownPosition))
        {
            const auto x { juce::jmap (event.position.x - leftMouseDownPosition.x, 0.0f, static_cast<float> (getWidth()), 0.0f, limits.x.max - limits.x.min) };
            limits.x.max -= x;
            leftMouseDownPosition = event.position;
            update();
        }
    }

    void mouseUp (const juce::MouseEvent& event) override
    {
        zoomRectangle.update ({});
        hovering.setVisible (false);
        if (event.mouseWasDraggedSinceMouseDown() && isLeftMouseButtonDown && plotBounds.contains (leftMouseDownPosition))
        {
            const auto [mouseX1, mouseY1] { leftMouseDownPosition };
            const auto [mouseX2, mouseY2] { event.position };
            limits = { { juce::jmap (std::min (mouseX1, mouseX2), plotBounds.getX(), plotBounds.getRight(), limits.x.min, limits.x.max),
                         juce::jmap (std::max (mouseX1, mouseX2), plotBounds.getX(), plotBounds.getRight(), limits.x.min, limits.x.max) },
                       { juce::jmap (std::max (mouseY1, mouseY2), plotBounds.getBottom(), plotBounds.getY(), limits.y.min, limits.y.max),
                         juce::jmap (std::min (mouseY1, mouseY2), plotBounds.getBottom(), plotBounds.getY(), limits.y.min, limits.y.max) } };
            update();
        }
        isLeftMouseButtonDown = false;
    }

    void mouseDoubleClick ([[maybe_unused]] const juce::MouseEvent& event) override
    {
        limits = fixedLimits;
        isLeftMouseButtonDown = false;
        update();
    }

    void resized() override
    {
        grid.setBounds (getLocalBounds());
        plotBounds.setBounds (getWidth() * 0.1f, getHeight() * 0.1f, getWidth() * 0.8f, getHeight() * 0.8f);
        topGridLabelBounds.setBounds (getWidth() * 0.1f, 0.0f, getWidth() * 0.8f, getHeight() * 0.1f);
        xAxisLabel.setBounds (getWidth() * 0.1f, getHeight() * 0.9f, getWidth() * 0.8f, getHeight() * 0.1f);
        yAxisLabel.setBounds (getWidth(), getHeight() * 0.1f, getHeight() * 0.8f, getWidth() * 0.1f);
        yAxisLabel.setTransform (juce::AffineTransform::rotation (juce::MathConstants<float>::halfPi, yAxisLabel.getX(), yAxisLabel.getY()));
    }

public:
    juce::Label xAxisLabel;
    juce::Label yAxisLabel;

    juce::Point<float> leftMouseDownPosition {};
    juce::Point<float> rightMouseDownPosition {};
    juce::Rectangle<float> plotBounds {};
    juce::Rectangle<float> topGridLabelBounds {};
    bool isLeftMouseButtonDown {};

public:
    Limit2d limits;
    const Limit2d fixedLimits;
    Grid grid;
    Hovering hovering;
    ZoomRectangle zoomRectangle;
    bool follow {};
};