#include <iostream>
#include <Windows.h>
#include <functional>

//Создайте custom deleter для std::unique_ptr, освобождающий ресурс из задания 1. Продемонстрируйте его использование

int main(void)
{
	try
	{
		const auto handle = ::CreateFile(L"test.txt",
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);
				
		
	    std::unique_ptr<void, std::function<void(void*)>> u_ptr(handle,[](void * handle)
	    {
		    if (handle != INVALID_HANDLE_VALUE)
		    {
			    ::CloseHandle(handle);
		    }
	    	
	    }); 
				
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}
	
}


