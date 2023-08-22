#pragma once

#include "wipch.h"
#include "wi/types/Types.h"

namespace wi
{
    //static std::vector<std::thread> s_ThreadPool;
    //static std::atomic<bool> running(false);
    //static std::atomic<bool> paused(false);

    struct Clock : public RefCounted
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> t0; // start time
        std::chrono::time_point<std::chrono::high_resolution_clock> t1; // last time
        std::chrono::time_point<std::chrono::high_resolution_clock> t;  // current time
        std::chrono::duration<f64_t> dt = std::chrono::high_resolution_clock::duration::zero();
      
        bool running = false;
        bool paused = false;
        
       // bool IsRunning() { return running; }
        //bool IsPaused() { return paused; }

        bool start()
        {
            paused = false;
            if (!running)
            {
                running = true;
                t = t0 = t1 = std::chrono::high_resolution_clock::now();
                dt = std::chrono::high_resolution_clock::duration::zero();
            }
            return running;
        }

        bool stop()
        {
            running = false;
            paused = false;
            return running;
        }

        bool pause()
        {
            paused = true;
            return paused;
        }

        f64_t poll()
        {
            t = std::chrono::high_resolution_clock::now();
            if (running && !paused)
            {
                dt += t - t1;
            }
            t1 = t;
            return dt.count();
        }

        f64_t pollms()
        {
            return poll() / 1000.f;
        }

        f64_t poll(f64_t fs)
        {
            f64_t d0 = 0.0, d1 = 0.0, dd = 0.0;
            d0 = poll();
            if (!running || paused)
                return 0.0;
            dd = 0.0;
            while (dd < (1.0 / fs))
            {
                if (!running) break;
                d1 = poll();
                dd = d1 - d0;
            }
             return d1;
        }

        bool post(bool cmd)
        {
            if (cmd)
                return start();
            return stop();
        }
    };

    //struct mt_Clock : public RefCounted
    //{
    //    std::chrono::time_point<std::chrono::high_resolution_clock> t0; // start time
    //    std::chrono::time_point<std::chrono::high_resolution_clock> t1; // last time
    //    std::chrono::time_point<std::chrono::high_resolution_clock> t;  // current time
    //    std::chrono::duration<f64_t> dt = std::chrono::high_resolution_clock::duration::zero();


    //    bool IsRunning() { return running; }
    //    bool IsPaused() { return paused; }

    //    Clock() {  }
    //    ~Clock()
    //    {
    //        running = false;
    //        for (uint32_t i = 0; i < s_ThreadPool.size(); i++)
    //        {
    //            s_ThreadPool[i].join();
    //            printf("threadjoined\n");
    //        }
    //        s_ThreadPool.clear();
    //    }

    //    bool start() // start or restart
    //    {
    //        paused = false;
    //        if (!running)
    //        {
    //            running = true;
    //            t = t0 = t1 = std::chrono::high_resolution_clock::now();
    //            dt = std::chrono::high_resolution_clock::duration::zero();
    //        }
    //        return running;
    //    }

    //    bool stop()
    //    {
    //        for (uint32_t i = 0; i < s_ThreadPool.size(); i++)
    //        {
    //            printf("stopping %d\n", i);
    //            s_ThreadPool[i].join();
    //            printf("threadjoined\n");
    //        }
    //        s_ThreadPool.clear();
    //        running = false;
    //        paused = false;
    //        return running;

    //    }

    //    bool pause()
    //    {
    //        paused = true;
    //        return paused;
    //    }

    //    f64_t poll()
    //    {
    //        t = std::chrono::high_resolution_clock::now();
    //        if (running && !paused)
    //        {
    //            dt += t - t1;
    //        }
    //        t1 = t;
    //        return dt.count();
    //    }

    //    f64_t pollms()
    //    {
    //        return poll() / 1000.f;
    //    }

    //    void _tick(f64_t& _freq, std::vector<f64_t>& data, std::mutex& mutex)
    //    {
    //        f64_t d0 = 0.0, d1 = 0.0, dd = 0.0;
    //        d0 = poll();
    //        while (running && !paused)
    //        {
    //            dd = 0.0;
    //            while (dd < (1.0 / _freq))
    //            {
    //                if (!running) break;
    //                d1 = poll();
    //                dd = d1 - d0;
    //            }
    //            mutex.lock();
    //            if (data.size() == 0)
    //                data.reserve(10);
    //            data.push_back(d1);
    //            mutex.unlock();
    //            d0 = poll();
    //        }
    //    }

    //    bool post(bool cmd)
    //    {
    //        if (cmd)
    //        {
    //            LOG_TRACE("Starting clock");
    //            return start();
    //        }
    //         
    //        return stop();
    //            
    //    }
    //    
    //    void tick(f64_t& _freq, std::vector<f64_t>& data, std::mutex& mutex)
    //    {
    //        s_ThreadPool.emplace_back(&Clock::_tick, this, std::ref(_freq), std::ref(data), std::ref(mutex));
    //        printf("Thread Created\n");

    //    }
    //};
}
