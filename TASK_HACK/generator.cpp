#include <iostream>
#include <random>
#include <numeric>

std::string generate(void)
{
	std::string password(6,'0');
	
	std::random_device dev;
  	std::mt19937 gen(dev());
	const std::uniform_int_distribution<unsigned int> uid(97, 126); //range of values for input (so that there are no obscure characters)

	int orig_byte[] = {222,209,212}; //sum element
	
	size_t index_begin = 0;
	size_t index_end = 5;
	
	for (auto i : orig_byte)
	{
	   const char out_rand = uid(gen);
	   const auto out_rand_two = std::abs((i - out_rand));
	   password[index_begin] = out_rand;
	   password[index_end] = out_rand_two;

	   ++index_begin;
	   --index_end;
	}

	const auto sum = std::accumulate(password.cbegin(),password.cend(),0) + 111; //16 * 6 + 0...5
	
	if (sum != 0x2f2)
	{
		throw std::runtime_error("Error generate!");
	}
	 
	
	return password;
}

int main(void)
{
	try
	{
				
		while (true)
		{
			std::cout<<"Press enter to generate password... Exit: ctrl + c";
			std::cin.get();
			std::cout<<"password - "<<generate()<<std::endl;
		}
	}
	catch (const std::exception & error)
	{

		std::cout<<error.what()<<std::endl;

	}

	catch(...)
	{
		std::cout<<"Error!"<<std::endl;
	}
	
}
