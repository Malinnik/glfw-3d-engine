#include "ChunkGenerator.h"

#include <loguru.hpp>
#include "fmt/format.h"
#include "world/generation.h"

ChunkGenerator::ChunkGenerator()
{
    m_thread = std::thread(&ChunkGenerator::workerLoop, this);
}

ChunkGenerator::~ChunkGenerator()
{
    shutdown();
}

void ChunkGenerator::requestChunk(int x, int y, int z)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_taskQueue.push({x, y, z});
    }
    m_condition.notify_one();
}

void ChunkGenerator::fetchReadyChunks(std::vector<ChunkResult> &outResults)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_resultQueue.empty()) {
        outResults.push_back(std::move(m_resultQueue.front()));
        m_resultQueue.pop();
    }
}

void ChunkGenerator::shutdown()
{
    m_running = false;
    m_condition.notify_all();
    if (m_thread.joinable())
        m_thread.join();
}

void ChunkGenerator::workerLoop()
{
    while(m_running)
    {
        ChunkTask task;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this] {return !m_taskQueue.empty() || !m_running;});
            if (!m_running) break;
            task = m_taskQueue.front();
            m_taskQueue.pop();
        }

        ChunkResult result;
        result.task = task;
        // LOG_F(INFO, fmt::format("CHUNK_GEN: At {} {} {}", task.x, task.y, task.z).c_str());
        WorldGenerator::generate(result.blocks, task.x, task.y, task.z);

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_resultQueue.push(std::move(result));
        }
    }
}
