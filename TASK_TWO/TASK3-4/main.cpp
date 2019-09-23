#include <iostream>
#include "WeakPtr.h"
//Разработайте упрощенные реализации std::shared_ptr и std::weak_ptr
int main(void)
{
	try
	{
		
	        std::cout<<std::boolalpha;
		
		WeakPtr<int> w;

		{
			std::cout<<w.Expired()<<std::endl;
			const auto ptr = MakeShared<int>(5);
			w = ptr;
			std::cout<<w.Expired()<<std::endl;
		}

	        const auto ptr = w.Lock();
		
		if (!ptr.get())
		{
			std::cout<<"Empty!"<<std::endl;
		}
		
		std::cout<<w.Expired()<<std::endl;	
				
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}

#ifdef _DEBUG
	if (_CrtDumpMemoryLeaks())
	{
		std::cout<<"Memory leak!"<<std::endl;
	}
#endif

}


