#include <iostream>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>

using boost::asio::ip::tcp;

struct info_file
{
	friend class boost::serialization::access;
	std::wstring name_file{};
	std::size_t length_file{};

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & this->name_file;
		ar & this->length_file;
	}
	
};

int wmain(void)
{
	try
	{
		unsigned short port = 33333;
		
		boost::asio::io_service io_service;
		tcp::iostream stream;
        tcp::acceptor a { io_service, tcp::endpoint { tcp::v4(), port } };
		tcp::socket sock{io_service};
		
		std::wcout<<L"Server started...\n";
		a.accept(*stream.rdbuf());
		std::wcout<<L"Connected!\n";
		
		info_file info_file{};
		{
			boost::archive::text_iarchive oa(stream);
			oa >> info_file;
		}

		std::ofstream file(info_file.name_file,std::ios::binary | std::ios::trunc);

		if (!file.is_open())
		{
			throw std::runtime_error("Error create file!");
		}

		std::wcout<<L"Start receive..\n";
		
		size_t full_size = 0;
		const size_t size_packet = 4096;
		
		boost::archive::text_iarchive iarch(stream, boost::archive::no_header );
		
		while (full_size != info_file.length_file)
		{
			
			std::string buff;
			iarch >> buff;
			file.write(buff.data(),buff.size());
			full_size+=buff.length();
			std::wcout<<L"\rsize packet - "<<full_size;
					
		}
		std::wcout<<L"\nFile received!"<<std::endl;
		
		
	}
	catch (const std::exception & err)
	{
		std::wcout<<err.what();
	}
	catch(...)
	{
		std::wcout<<L"Error..."<<std::endl;
	}
		
}
