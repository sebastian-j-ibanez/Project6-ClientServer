#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include <queue>
#include <functional>

class Thread_Pool {
public:
	void Start(const uint32_t thread_count) {
		for (uint32_t i = 0; i < thread_count; i++) {
			thread_list.emplace_back(std::thread(&Thread_Pool::threadLoop,this));
		}
	};
	/// <summary>
	/// Post a new job to the queue
	/// </summary>
	/// <typeparam name="func"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="job">Function Pointer to the function to be executed</param>
	/// <param name="...args">List of paramaters to be passed to the given function</param>
	template <typename func, typename... Args>
	void PostJob(func job, Args&&... args) {
		{
			std::unique_lock<std::mutex> lock(queue_lock);
			job_queue.push([=]() { job(args...); });
		}
		cv.notify_one();
	}

	/// <summary>
	/// The thread pool will finish all of its queued tasks and then quit gracefully.
	/// </summary>
	void Stop() {
		while (this->Busy()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		{
			std::unique_lock<std::mutex> lock(queue_lock);
			terminate = true;
		}
		cv.notify_all();
		for (std::thread& t : thread_list) {
			t.join();
		}
		thread_list.clear();
	}
	/// <summary>
	/// Forces the thread pool to stop immediately regardless of the number of queued jobs.
	/// </summary>
	void Stop_Force() {
		while (this->Busy()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		{
			std::unique_lock<std::mutex> lock(queue_lock);
			terminate = true;
		}
		cv.notify_all();
		for (std::thread& t : thread_list) {
			t.join();
		}
		thread_list.clear();
	}
	bool Busy() {
		bool busy;
		{
			std::unique_lock<std::mutex> lock(queue_lock);
			busy = !job_queue.empty();
		}
		return busy;
	}
private:
	void threadLoop() {
		while (true) {
			std::function<void()> cur_job;
			{
				std::unique_lock<std::mutex> lock(queue_lock);
				cv.wait(lock, [this] {
					return !job_queue.empty() || terminate;
					});
				if (terminate)
					return;
				cur_job = job_queue.front();
				job_queue.pop();
			}
			cur_job();
		}
	}

	bool terminate = false;
	std::mutex queue_lock;
	std::condition_variable cv;
	std::vector<std::thread> thread_list;
	std::queue<std::function<void()>> job_queue;
};