#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <optional>

std::vector<std::string> get_blocks(const std::string& data);
std::vector<std::string> get_repeated_elements(const std::vector<std::string>& data);

std::optional<size_t> get_ecb_index(const std::vector<std::string>& data)
{
	for (std::size_t i{}; i < data.size(); ++i)
	{
		if (!get_repeated_elements(get_blocks(data[i])).empty())
		{
			return i;
		}
	}

	return std::nullopt;
}

std::vector<std::string> get_blocks(const std::string& data)
{
	std::vector<std::string> blocks{};

	for (std::size_t i{}; i < data.size(); i += 16)
		blocks.push_back(data.substr(i, 16));

	return blocks;
}

std::vector<std::string> get_repeated_elements(const std::vector<std::string>& data)
{
	std::vector<std::string> repeated_elements{};

	for (std::size_t i{}; i < data.size(); ++i)
	{
		if (std::count(data.cbegin(), data.cend(), data[i]) > 1)
		{
			repeated_elements.push_back(data[i]);
		}
	}

	return repeated_elements;
}


int main(void)
{
	try
	{
		//in your file, the answer is in line 54 

		std::ifstream file("crypto.txt", std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("File could not be opened!");
		}

		const std::istream_iterator<std::string> is(file);
		const std::istream_iterator<std::string> is_end{};

		const auto res = get_ecb_index({ is,is_end });

		res.has_value() ? std::cout << "Index:  " << *res : std::cout << "Not found!";
		
		std::cout << std::endl;

	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}

}
