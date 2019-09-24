#include <iostream>
#include <Windows.h>

//Разработайте guard, управляющий дескриптором, полученным в результате вызова функции Win32 API CreateFile. Продемонстрируйте его использование.
class smart_handle
{

public:

	smart_handle(const smart_handle& other) = delete;
	smart_handle(smart_handle&& other) = delete;
	smart_handle& operator=(const smart_handle& other) = delete;
	smart_handle& operator=(smart_handle&& other) noexcept = delete;
	
	explicit smart_handle(HANDLE handle = INVALID_HANDLE_VALUE):handle_(handle)
	{
		
	}

	auto get(void) const noexcept
	{
		return this->handle_;
	}
	
	auto & operator *(void) noexcept
	{
		return this->handle_;
	}  

	void reset(HANDLE handle = INVALID_HANDLE_VALUE)
	{
		this->handle_ = handle;
	}

	void swap(smart_handle & other) noexcept
	{
		std::swap(this->handle_, other.handle_);
	}
	
	~smart_handle(void) 
	{
		if (this->handle_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(this->handle_);
		}
					
	}
		
private:
	HANDLE handle_{};
	
};

int main()
{
	try
	{
		const auto ptr1 = ::CreateFile(L"test.txt",
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);
				   	
			   		
		smart_handle guard(ptr1);
		
	}
	catch (const std::exception & error)
	{
		std::cerr<<error.what()<<std::endl;
	}
	catch (...)
	{
		std::cout<<"Error!"<<std::endl;
	}
    
}


