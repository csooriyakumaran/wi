#pragma once
#include "wipch.h"
#include "wi/core/Ref.h"
#include "wi/core/Log.h"
#include "wi/data/DataBuffer.h"
#include "wi/data/DataIO.h"
#include "wi/timing/Timestep.h"

namespace wi
{
    class DataSeries : public RefCounted
    {
    public:
        DataSeries() {}
        ~DataSeries() {}

        Ref<DataBuffer>        RawData = Ref<DataBuffer>::Create(); // rawData better name?
        Ref<DataBuffer>        ReducedData = Ref<DataBuffer>::Create(); // reducedData better name? could be fft, or filtered, or the output of a chain of operations. 
        std::queue<DataBuffer> BufferQueue;
        // should we add a stack of processing functions? or an ordered map? the order of processing is important, but we may want the use to dynamically add new steps in the data reduction chain
        //std::orderedmap<uint32_t, std::function<void()>> ProcessingStack;
        std::mutex BufferQueueMutex;
        uint32_t offset = 0;

        void reserve(const size_t size)
        {
            RawData->reserve(size);
            ReducedData->reserve(size);
        }

        void Process(){} // process the data, maybe loop through processing function pipeline, but process the data in the queue and then 

        void Stream(DataIO* io, size_t buffsize, std::atomic<bool>& streaming)
        {
            if (!io)
                return;
            while (streaming)
            {
                DataBuffer buff = io->Load(streaming, buffsize);
                enqueue(buff);
            }
        }

        void InsertProcessingFunction(uint32_t pos, const std::function<void()>& fn)
        {
        }

        void RemoveProcessingFunction(uint32_t pos)
        {
        }

        void enqueue(DataBuffer& buff)
        {
            std::scoped_lock<std::mutex> lock(BufferQueueMutex);
            BufferQueue.push(std::move(buff));
            // LOG_TRACE_TAG("DataSeries::enqueue", "Size of BufferQueue: {0}", BufferQueue.size());
        }

        void dequeue()
        {
            while (!BufferQueue.empty())
            {
                // do we add mutex locking here? each data series manages it's own 
                std::scoped_lock<std::mutex> lock(BufferQueueMutex);
                RawData->append(BufferQueue.front());
                BufferQueue.pop();
                // LOG_TRACE_TAG("DataSeires::dequeue", "Length of DataSeries: {0}", RawData->x.size());
                // LOG_TRACE_TAG("DataSeries::dequeue", "Size of BufferQueue: {0}", BufferQueue.size());

            }
        }

        void clear()
        {
            RawData->clear();
            ReducedData->clear();

            while (!BufferQueue.empty())
            {
                BufferQueue.front().clear();
                BufferQueue.pop();
            }
        }
    };
}
