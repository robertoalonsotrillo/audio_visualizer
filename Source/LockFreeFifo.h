#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

template<typename T, size_t Size>
struct LockFreeFifo
{
    auto push(T sample)
    {
        auto write {fifo.write(1)};
        if (write.blockSize1 > 0)
        {
            buffer[write.startIndex1] = sample;
        } else if (write.blockSize2 > 0)
        {
            buffer[write.startIndex2] = sample;
        }
    }

    bool pull(T &element)
    {
        auto read {fifo.read(1)};
        const std::array block_size {read.blockSize1, read.blockSize2};
        const std::array start_index {read.startIndex1, read.startIndex2};
        for (size_t i {}; i < 2; ++i)
        {
            if (block_size[i] > 0)
            {
                element = buffer[start_index[i]];
                return true;
            }
        }
        return false;
    }

    juce::AbstractFifo fifo {Size};
    std::array<T, Size> buffer {};
};