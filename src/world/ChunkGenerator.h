#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "world/chunk.h"

struct ChunkTask {
    int x, y, z;
};

struct ChunkResult {
    ChunkTask task;
    unsigned int blocks[CHUNK_W * CHUNK_H * CHUNK_D];
};


class ChunkGenerator {
    public:
        ChunkGenerator();
        ~ChunkGenerator();

        void requestChunk(int x, int y, int z);

        void fetchReadyChunks(std::vector<ChunkResult>& outResults);

        void shutdown();

    private:
        void workerLoop();

        std::thread m_thread;
        std::queue<ChunkTask> m_taskQueue;
        std::queue<ChunkResult> m_resultQueue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_running{true};
};