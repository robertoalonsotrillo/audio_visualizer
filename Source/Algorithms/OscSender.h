#pragma once

#include "../Config.h"
#include "juce_osc/juce_osc.h"

struct OscSender
{
    OscSender()
    {
        sender.connect ("127.0.0.1", Config::oscPort);
    }

    ~OscSender()
    {
        sender.disconnect();
    }

    auto sendArray (const juce::String& address, std::vector<float>& input)
    {
        if (Config::is_osc_active)
        {
            juce::OSCMessage message { address };
            for (auto i : input)
            {
                message.addFloat32 (i);
            }
            sender.send (message);
        }
    }

    juce::OSCSender sender {};
};