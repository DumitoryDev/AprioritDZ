#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <filesystem>


struct info_file
{
	friend class boost::serialization::access;
	std::wstring name_file{};
	std::size_t length_file{};

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& this->name_file;
		ar& this->length_file;
	}

}; 

int wmain(int argc, wchar_t ** argv)
{
	try
	{
		if (argc == 1)
		{
			throw std::runtime_error("Invalid arguments!");
		}
		
		std::wstring path_file(argv[1]);
				
		if (path_file.empty())
		{
			throw std::runtime_error("Path is empty!");
		}

		std::ifstream file(path_file, std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("Error open file!");
		}
		
		std::wcout<<L"File is open!"<<std::endl;
		info_file info_file{};
		auto size_file = std::filesystem::file_size(path_file);
		info_file.name_file = path_file;
		info_file.length_file = size_file;
		
		std::wcout<<L"Type ip: "<<std::endl;
		std::string ip{};
		std::getline(std::cin, ip);
		
		if (ip.empty())
		{
			throw std::runtime_error("Ip is empty!");
		}
		
		std::wcout<<L"Type port: "<<std::endl;
		unsigned short port{};
		std::cin >> port;
		
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::endpoint end{ boost::asio::ip::address::from_string(ip),port};
		boost::asio::ip::tcp::iostream stream;
		
		std::wcout << L"Connect to server...." << std::endl;
		stream.connect(end);
		std::wcout << L"Ok!" << std::endl;
			
		{ 
			boost::archive::text_oarchive os(stream);
			std::wcout<<L"Send info..."<<std::endl;
			os << info_file;
			std::wcout<<L"OK!"<<std::endl;
		}
		
		const size_t size_packet = 4096;
		std::size_t full_size{};
		
		boost::archive::text_oarchive oarch( stream, boost::archive::no_header );
		while (!file.eof())
		{
			std::string data(size_packet,'\0');

			file.read(data.data(),data.size());

			if (!file.gcount())
			{
				throw std::runtime_error("Fail read file!");
			}
			data.resize(file.gcount());
		    oarch << data;
			full_size+=file.gcount();
			std::wcout<<L"\rsize packet - "<<full_size;
			
		}
			
		std::wcout<<L"\nFile received!"<<std::endl;
	}
	catch (const std::exception & error)
	{
		std::wcout<<error.what()<<std::endl;
	}
	catch(...)
	{
		std::wcout<<"Error..."<<std::endl;
	}

}


