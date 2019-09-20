#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <list>
#include <functional>

#ifdef _WIN32
#include <Windows.h>
#endif

//Напишите пример кода с использованием unique_ptr и shared_ptr.
//Должна присутствовать передача их в качестве параметра функций.
//Работа с массивами. Использование с контейнерами.



namespace unique_ptr_int
{
	using vector_unique_array = std::vector<std::unique_ptr<int[]>>;
	using vector_unique_ptr = std::vector<std::unique_ptr<int>>;

	auto make_unique(int val)
	{
		return std::make_unique<int>(val);
	}

	auto make_array_unique(const int val)
	{
		return std::make_unique<int[]>(val);
	}

	void print_unique(const std::unique_ptr<int>& ptr)
	{
		std::cout << *ptr << " ";
	}

	auto make_vec_array_unique_ptr(const std::size_t size)
	{
		vector_unique_array vec_u_ptr(size);

		for (std::size_t i = 0; i < size; ++i)
		{
			vec_u_ptr[i] = make_array_unique(size);

			for (std::size_t j = 0; j < size; ++j)
			{
				vec_u_ptr[i][j] = j + 1;
			}
		}

		return vec_u_ptr;

	}

	auto make_vec_unique_ptr(const std::size_t size)
	{
		vector_unique_ptr vec_u_ptr(size);

		for (std::size_t i = 0; i < size; ++i)
		{
			vec_u_ptr[i] = make_unique(i + 1);
		}

		return vec_u_ptr;
	}

	auto print_vec_unique_array(const vector_unique_array& data)
	{
		std::cout << "One-dimensional vector unique_ptr<int[]>:" << std::endl;
		const auto size = data.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			for (std::size_t j = 0; j < size; ++j)
			{
				std::cout << data[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	auto print_vec_unique_ptr(const vector_unique_ptr& data)
	{
		std::cout << "One-dimensional vector unique_ptr<int>:" << std::endl;
		std::for_each(data.cbegin(), data.cend(), print_unique);
	}


	void run_test(void)
	{
		std::cout << "---UNIQUE_PTR TEST---" << std::endl;

		print_vec_unique_ptr(make_vec_unique_ptr(10));
		std::cout << std::endl;
		print_vec_unique_array(make_vec_array_unique_ptr(10));
	}
}


namespace shared_ptr_int
{
	using vector_shared_ptr = std::vector<std::shared_ptr<int>>;

	auto make_shared(const int val)
	{
		return std::make_shared<int>(val);
	}

	auto print_shared(const std::shared_ptr<int>& shared)
	{
		std::cout << *shared << " ";
	}

	auto make_vector_shared_ptr(const std::size_t size)
	{

		vector_shared_ptr vec_sh_ptr(size);

		for (std::size_t i = 0; i < size; ++i)
		{
			vec_sh_ptr[i] = make_shared(i + 1);
		}

		return vec_sh_ptr;

	}

	auto make_list_of_vector(const vector_shared_ptr& data)
	{
		return std::list(data.cbegin(), data.cend());
	}

	template<typename T>
	void print_for_all_shared(const T& data)
	{
		std::for_each(data.cbegin(), data.cend(), print_shared);
		std::cout << std::endl;
	}

	void run_test(void)
	{
		std::cout << "---SHARED_PTR TEST---" << std::endl;

		auto vec_shared = make_vector_shared_ptr(10);
		std::cout << "Vector shared_ptr:" << std::endl;
		print_for_all_shared(vec_shared);

		const auto list_shared = make_list_of_vector(vec_shared);
		std::cout << "List(copy of vector) shared_ptr:" << std::endl;
		print_for_all_shared(list_shared);

		std::cout << "Change vector element:" << std::endl;

		*vec_shared[0] = (std::numeric_limits<int>::max)();

		print_for_all_shared(vec_shared);

		std::cout << "And list also changed:" << std::endl;
		print_for_all_shared(list_shared);

	}


}



namespace practical_use
{


	using smart_handle = std::unique_ptr<void, std::function<void(void*)>>;

	void test(void)
	{
		const auto ptr1 = ::CreateFile(L"test.txt",
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);


		const smart_handle ptr(ptr1, [](void* ptr)
			{
				if (ptr != INVALID_HANDLE_VALUE)
				{
					if (::CloseHandle(ptr) == 0)
					{
						std::cerr << "Failed CloseHandle! - " << GetLastError() << std::endl;
					}

				}
			});


		if (reinterpret_cast<void*>(ptr.get()) == INVALID_HANDLE_VALUE)
		{

			std::cerr << "Failed open file! - " << GetLastError() << std::endl;
			return;

		}

		std::cout << "Ok!" << std::endl;

	}



}

int main(void)
{
	try
	{
		unique_ptr_int::run_test();
		std::cout << std::endl;
		shared_ptr_int::run_test();
		std::cout << std::endl;
		practical_use::test();

	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
	catch (...)
	{
	}


}


