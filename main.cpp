#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <thread>
#include <future>
#include <memory>
#include "sync.hpp"


class worker
{
public:

    thread_sync::gate m_gate{};
    std::thread m_work_thread{};
    uint64_t m_last_total_time {0};
    uint64_t m_iterations{};
    uint64_t m_worker_id{};

    explicit worker(uint64_t iterations, uint64_t id) : m_iterations(iterations), m_worker_id(id)
    {
        //std::cout << "Create worker: " << m_worker_id << " starting workload: " << m_iterations << std::endl;
    }

    ~worker()
    {
        m_gate.open_gate();
        thread_sync::threading::join_thread(m_work_thread);
    }

    void start()
    {
        m_work_thread = std::thread([=](){ return thread_func(m_iterations); });
        m_gate.open_gate();
    }

    uint64_t get()
    {
        thread_sync::threading::join_thread(m_work_thread);
        return m_last_total_time;
    }

    uint64_t thread_func(uint64_t interations)
    {
        m_last_total_time = 0;

//        std::cout << "worker: " << m_worker_id << " ready" << std::endl;
        m_gate.wait_at_gate();
//        std::cout << "worker: " << m_worker_id << " working" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        double value = 1.2;
        for (uint64_t i = 0u; (i < interations); i++)
        {
            value = std::atan(value);
        }
        (void) value; // Avoid optimisation
        m_last_total_time = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count());
//        std::cout << "worker: " << m_worker_id << " done " << m_last_total_time << "ms" << std::endl;
        return m_last_total_time;
    }
};

int main(int argc, char *argv[])
{
    size_t total_work = 100'000'000;
    size_t num_threads = 6;
    size_t num_iterations = 3;
    std::cout << "argc: " << argc << std::endl;
    if (argc > 1)
    {
        std::stringstream ss;
        ss << argv[1];
        ss >> num_threads;
    }

    std::cout << "Start work test: total_work: " << total_work << ", threads: " << num_threads << "\n";
    std::vector<std::unique_ptr<worker>> workers;
    workers.reserve(num_threads);

    // Create workers
    for (uint64_t worker_id = 1; worker_id <= num_threads; worker_id++)
    {
        workers.emplace_back(std::make_unique<worker>(total_work / num_threads, worker_id));
    }

    // Get start time
    for (size_t i = 1; i <= num_iterations; i++)
    {
        std::cout << "============ ITER: " << i << " ==============\n";
        auto overall_start = std::chrono::high_resolution_clock::now();
        // Start workers
        for (auto &worker : workers)
        {
            worker->start();
        }
        // Wait for workers to finish and get results
        uint64_t accumulated_time = 0;
        for (auto &worker : workers)
        {
            accumulated_time += worker->get();
        }
        auto overall_total_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - overall_start).count();
        std::cout << "actual time taken: " << overall_total_time << "ms" << std::endl;
        std::cout << "accumulated time:  " << accumulated_time << "ms" << std::endl;
    }
    std::cout << "===================================\n";
    return 0;
}
