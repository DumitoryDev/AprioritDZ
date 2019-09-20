#include <iostream>
#include <memory>
#include <vector>

//Напишите пример с использованием исключений и обеспечением строгой гарантии безопасности исключений.
//Обязательное требование - демонстрация транзакционности, откат в случае всего лишь частичного исполнения. 

template<typename T>
class Array
{
public:

	explicit Array(const std::size_t size = 0) :u_ptr_(std::make_unique<T[]>(size)), size_(size)
	{

	}

	void push(const T& val)
	{
		auto ptr_tmp = std::make_unique<T[]>(this->size_ + 1);
		
		for (std::size_t i{}; i < this->size_; ++i)
		{
			ptr_tmp[i] = this->u_ptr_[i];
		}
			
		ptr_tmp[this->size_] = val;
		this->u_ptr_ = std::move(ptr_tmp);
		++this->size_;
	}

	void resize(const std::size_t new_size) // strict exclusion guarantee
	{
		Array tmp(new_size);

		for (std::size_t i = 0; i != new_size && i != this->size_; ++i)
		{
			tmp.at(i) = this->u_ptr_[i];
		}

		tmp.swap(*this);

	}

	T& at(const std::size_t index)
	{
		if (this->size_ <= index)
		{
			throw std::out_of_range("Invalid index!");
		}

		return this->u_ptr_[index];
	}
	
	void swap(Array& left)  noexcept
	{
		std::swap(this->u_ptr_, left.u_ptr_);
		std::swap(this->size_, left.size_);
	}

private:
	std::unique_ptr<T[]> u_ptr_{};
	std::size_t size_{};

};


struct BadStruct
{
	BadStruct& operator = (const BadStruct&)
	{
		throw std::runtime_error("Exception in copy operator ");
	}

	friend std::ostream& operator << (std::ostream& os, const BadStruct& tmp)
	{
		os << "Alive! val - " << tmp.val << std::endl;
		return os;
	}

	BadStruct() = default;
	BadStruct(const BadStruct& other) = default;
	BadStruct(BadStruct&& other) noexcept = default;
	BadStruct& operator=(BadStruct&& other) noexcept = default;

	int val{};
};

int main(void)
{
	try
	{
		Array<BadStruct> arr(10);
	        arr.at(9).val = 10;

		try
		{
			arr.push(BadStruct()); // throw
		}
		catch (const std::exception& error)
		{
			std::cout << error.what() << std::endl;
		}
		
		try
		{
			arr.resize(5); //throw
		}
		catch (const std::exception& error)
		{
			std::cout << error.what() << std::endl;
		}
		
		std::cout << arr.at(9); //but array is alive 

	}
	catch (const std::exception& error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
	}


}


