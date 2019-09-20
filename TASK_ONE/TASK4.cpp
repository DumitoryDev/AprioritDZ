#include <iostream>
#include <memory>
#include <vector>

//Реализуйте один паттерн проектирования на выбор.

//Паттерн наблюдатель(Observer)

class observer
{
public:
	virtual void update(const char * data) = 0;
};


class subject
{

public:
	void attach(observer * obs)
	{
		this->clients_.emplace_back(obs);
	}

	void set_info(const std::string & str)
	{
		this->info_data_ = str;
	}
	
	void notify(void)
	{
		for (const auto & element : this->clients_)
		{
			element->update(this->info_data_.c_str());
		}
	}
	
private:
	std::vector<observer*>  clients_{};
	std::string info_data_{};
	
};


class client final :public observer
{
public:
	client(void) = default;
	explicit client(subject * sub)
	{
		sub->attach(this);
	}
	void update(const char * data) override
	{
		std::cout<<"INFO! - "<<data<<std::endl;
	}
	
};

class client_greets final :public observer
{
public:
	client_greets(void) = default;
	explicit client_greets(subject * sub)
	{
		sub->attach(this);
	}
	void update(const char * data) override
	{
		std::cout<<"Hello, "<<data<<std::endl;
	}
	
};



int main(void)
{
	try
	{  
	        subject subject;
		std::vector<std::unique_ptr<observer>> vec_data(10);
		for (std::size_t i{}; i < vec_data.size(); ++i)
		{
			i&1?
			vec_data[i] = std::make_unique<client>(&subject):
			vec_data[i] = std::make_unique<client_greets>(&subject);
			
		}
				
		subject.set_info("David!");
		subject.notify();		

	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
	}


}


