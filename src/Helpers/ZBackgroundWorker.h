#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <queue>

class ZBackgroundWorker
{
public:
	ZBackgroundWorker();
	~ZBackgroundWorker();

	void Enqueue(const std::function<void()>& s_Task);

private:
	std::atomic<bool> m_bRunning{ true };
	std::thread m_WorkerThread;

	std::mutex m_QueueMutex;
	std::queue<std::function<void()>> m_qTaskQueue;

	std::condition_variable m_NewTaskCondition;

	void RunWorker();
};
