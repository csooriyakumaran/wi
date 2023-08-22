#pragma once
#include "wipch.h"
#include "wi/core/Log.h"
#include "wi/core/Ref.h"
#include "wi/data/DataSeries.h"
#include "wi/data/DataIO.h"
#include "wi/numerics/Hash.h"
#include "wi/numerics/UUID.h"
#include "wi/timing/Timestep.h"
#include "wi/timing/Clock.h"


namespace wi
{
    struct DataSeriesSpec
    {
        uint64_t        ID = wi::UUID();
        std::string     Name="";
        Ref<DataSeries> Data = nullptr;
        Ref<DataIO>     IO = nullptr;
        size_t          Capacity = 0;
        size_t          BuffSize = 0;
        glm::vec4       Color = { 1, 1, 1, 1 };
    };

    class DataManager : public wi::RefCounted
    {
    public:
        DataManager(Ref<Clock> clock) 
            : m_Clock(clock) 
        {
            
        }
        ~DataManager() 
        {
            m_DataSet.clear();
        }
        
        Ref<DataSeries> AddSeries(DataSeriesSpec& dataSpec)
        {
            //uint32_t id = Hash::GenerateFNVHash(dataSpec.ID);
            if (m_DataSet.find(dataSpec.ID) != m_DataSet.end())
            {
                LOG_CORE_ERROR_TAG("DataManager::AddSeries", "A data series with id '{0}' has already been added.", dataSpec.ID);
                return nullptr;
            }

            dataSpec.Data->reserve(dataSpec.Capacity);
            m_DataSet[dataSpec.ID] = dataSpec;
            return dataSpec.Data;

        }

        Ref<DataSeries> AddSeries(uint64_t id, const char* displayName, DataIO* io, size_t capacity, size_t buffsize)
        {
            return AddSeries(DataSeriesSpec{ id, displayName, Ref<DataSeries>::Create(), Ref<DataIO>(io), capacity, buffsize});
        }

        Ref<DataSeries> AddSeries(uint64_t id, DataIO* io, size_t capacity, size_t buffsize)
        {
            return AddSeries(DataSeriesSpec{id, "Data Series", Ref<DataSeries>::Create(), Ref<DataIO>(io), capacity, buffsize });
        }
        
        //Ref<DataSeries> GetSeriesData(const char* strID)
        //{
        //    uint32_t id = Hash::GenerateFNVHash(strID);
        //    return GetSeriesData(id);
        //}

        Ref<DataSeries> GetSeriesData(uint64_t id)
        {
            if (m_DataSet.find(id) == m_DataSet.end())
            {
                LOG_CORE_ERROR_TAG("DataManger::GetSeries", "Could not find data series with id {0}", id);
                return nullptr;
            }
            return m_DataSet.at(id).Data;
        }

        DataSeriesSpec* GetSeriesSpec(uint64_t id)
        {
            if (m_DataSet.find(id) == m_DataSet.end())
            {
                LOG_CORE_ERROR_TAG("DataManger::GetSeries", "Could not find data series with id {0}", id);
                return nullptr;
            }
            return &m_DataSet.at(id);
        }


        // uint32_t GetSeriesID(const char* strID)
        // {
        //     uint32_t id = Hash::GenerateFNVHash(strID);
        //     // if (m_DataSet.find(id) == m_DataSet.end())
        //     // {
        //     //     LOG_CORE_ERROR_TAG("DataManger::GetSeries", "Could not find data series with id {0}", strID);
        //     //     return 0;
        //     // }
        //     return id;
        // }

        void RemoveSeries(uint64_t id)
        {
            //uint32_t id = Hash::GenerateFNVHash(strID);
            if (m_DataSet.find(id) != m_DataSet.end())
            {
                m_DataSet.erase(id);
                return;
            }
            LOG_CORE_ERROR_TAG("DataManager::RemoveSeries", "Couldn't find data series with id {0}", id);
        }

        void OnUpdate(Timestep ts) 
        {
            for (auto& [id, series] : m_DataSet)
            { 
                // should this be done on the main thread as it currently is? can maybe do this on a single thread for all data series (rather than one thread per series like the streaming is done)
                // this way, we dont bog down the renderening, but also dont needlessly use up resources
                //m_ThreadPool.emplace_back("proc thread");
                //m_ThreadPool.back().Dispatch(&DataSeries::Process, series.Data.Raw());
                //m_ThreadPool.emplace_back("dequeue thread");
                //m_ThreadPool.back().Dispatch(&DataSeries::dequeue, series.Data.Raw());
                series.Data->Process(); // process the data in the buffer, move the processed data into the ProcessedData structure
                series.Data->dequeue(); // move the buffer into the Raw data structure
            }
        } 

        void Stream(bool streamCmd)
        {
            m_Streaming = m_Clock->post(streamCmd);
            
            if (!m_Streaming)
            {
                for (auto& thread : m_ThreadPool)
                    thread.Join();
                m_ThreadPool.clear();
                return;
            }

            for (auto& [id, series] : m_DataSet)
            {
                m_ThreadPool.emplace_back(series.Name);
                m_ThreadPool.back().Dispatch(&DataSeries::Stream, series.Data.Raw(), series.IO.Raw(), std::ref(series.BuffSize), std::ref(m_Streaming));
            }
        }

        void Clear()
        {
            m_Streaming = false;
            for (auto& thread : m_ThreadPool)
                thread.Join();
            m_ThreadPool.clear();

            for (auto& [id, series] : m_DataSet)
            {
                if (!series.Data)
                    continue;
                series.Data->clear();
            }
        }

        Ref<Clock> GetClock() { return m_Clock; }

       // std::unordered_map<uint32_t, DataSeriesSpec>& GetSeriesBySource(DATASOURCE dataSource) { return m_DataSet[(size_t)dataSource]; }
       // const std::unordered_map<uint32_t, DataSeriesSpec>& GetSeriesBySource(DATASOURCE dataSource) const { return m_DataSet[(size_t)dataSource]; }
        std::unordered_map<uint64_t, DataSeriesSpec>& GetDataSet() { return m_DataSet; }
        const std::unordered_map<uint64_t, DataSeriesSpec>& GetDataSet() const { return m_DataSet; }

    private:
        std::unordered_map<uint64_t, DataSeriesSpec> m_DataSet;
        Ref<Clock> m_Clock;
        std::vector<Thread> m_ThreadPool;
        std::atomic<bool> m_Streaming;
    };

}
