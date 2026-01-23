#include "ZBackgroundWorker.h"


ZBackgroundWorker::ZBackgroundWorker()
{
	m_WorkerThread = std::thread(&ZBackgroundWorker::RunWorker, this);
}

ZBackgroundWorker::~ZBackgroundWorker()
{
	m_bRunning = false;
	m_NewTaskCondition.notify_all();

	if (m_WorkerThread.joinable())
	{
		m_WorkerThread.join();
	}
}

void ZBackgroundWorker::Enqueue(const std::function<void()>& s_Task)
{
	{
		std::lock_guard<std::mutex> lock(m_QueueMutex);
		m_qTaskQueue.push(s_Task);
	}

	m_NewTaskCondition.notify_one();
}


void ZBackgroundWorker::RunWorker()
{
	while (m_bRunning)
	{
		std::function<void()> s_Task;
		{
			std::unique_lock<std::mutex> s_Lock(m_QueueMutex);
			m_NewTaskCondition.wait(s_Lock, [this]() { return !m_qTaskQueue.empty() || !m_bRunning; });

			if (!m_bRunning && m_qTaskQueue.empty())
			{
				return;
			}

			s_Task = m_qTaskQueue.front();
			m_qTaskQueue.pop();
		}

		if (s_Task)
		{
			s_Task();
		}
	}
}
