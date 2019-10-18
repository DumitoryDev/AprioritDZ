

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <Windows.h>
#include <future>


class thread_pool
{
public:
	using task = std::function<void()>;

	thread_pool(const thread_pool& other) = delete;
	thread_pool(thread_pool&& other) noexcept = delete;
	thread_pool& operator=(const thread_pool& other) = delete;
	thread_pool& operator=(thread_pool&& other) noexcept = delete;

	explicit thread_pool(const std::size_t size_thread) :size_thread_(size_thread)
	{
		this->start(size_thread);
	}

	~thread_pool(void)
	{
		this->stop();
	}

	template<typename T, typename... Args>
	auto enqueue(T&& task, Args&& ... args)
	{

		using ret_type = std::invoke_result_t<T&&, Args&& ...>;

		auto wrapper = std::make_shared<std::packaged_task<ret_type()>>(
			std::bind(std::forward<T>(task), std::forward<Args>(args)...)
			);

		{
			std::unique_lock<std::mutex> lock{ this->m_event_ };


			this->q_tasks_.emplace([=]
				{
					(*wrapper)();
				});

		}
		this->cv_.notify_one();

		return wrapper->get_future();
	}


	void stop_all(void)
	{
		this->stop();
	}

	void reset_state(void)
	{
		if (!this->b_stop_)
		{
			throw std::runtime_error("Thread pool now is alive!");
		}

		std::lock_guard<std::mutex> lock(this->m_event_);
		this->b_stop_ = false;
		this->v_threads_.clear();

		for (auto i = this->q_tasks_.size(); i > 0; --i)
		{
			q_tasks_.pop();
		}
		this->start(this->size_thread_);
	}

	void set_new_size(const std::size_t new_size)
	{
		if (!this->b_stop_)
		{
			throw std::runtime_error("Thread pool now is alive!");
		}

		this->size_thread_ = new_size;

	}

	std::size_t get_size(void) const noexcept
	{
		return this->size_thread_;
	}

private:
	std::vector<std::thread> v_threads_{};
	std::queue<task> q_tasks_{};
	std::condition_variable cv_;
	std::mutex m_event_{};
	std::atomic_bool b_stop_ = false;
	std::atomic_size_t size_thread_{};

	void start(const std::size_t num_thread)
	{

		for (std::size_t i{}; i < num_thread; ++i)
		{
			v_threads_.emplace_back([=]
				{
					check_task_loop();
				});
		}
	}

	void check_task_loop(void)
	{
		while (true)
		{
			task now_task;
			{
				std::unique_lock<std::mutex> lock{ this->m_event_ };
				cv_.wait(lock, [=]
					{
						return this->b_stop_ || !this->q_tasks_.empty();
					});

				if (this->b_stop_)
				{
					break;
				}
				now_task = std::move(q_tasks_.front());
				q_tasks_.pop();
			}
			now_task();
		}


	}

	void stop(void) noexcept
	{

		this->b_stop_ = true;

		this->cv_.notify_all();
		for (auto& worker : this->v_threads_)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}

	}

};

int main(void)
{
	try
	{


		auto ptr = std::make_shared<thread_pool>(1);

		auto t1 = ptr->enqueue([]()
			{

				std::cout << "task1..." << std::endl;
				Sleep(5000);

			});

		auto t2 = ptr->enqueue([]()
			{
				std::cout << "task2..." << std::endl;
				Sleep(3000);
			});

		t1.get();
		ptr->stop_all();
		ptr->set_new_size(2);
		ptr->reset_state();

		auto t4 = ptr->enqueue([]()
			{
				Sleep(5000);
				std::cout << "task4..." << std::endl;

			});

		auto t5 = ptr->enqueue([]()
			{
				std::cout << "task5..." << std::endl;
				Sleep(5000);
			});

		t4.get();
		t5.get();
		std::cout << "End\n";
	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
	}


}


