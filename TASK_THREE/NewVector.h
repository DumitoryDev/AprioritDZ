#pragma once
#include <iostream>
#include <vector>
#include <string>

template<typename T>
class MyVectorType
{
	using vec_type = std::vector<std::pair<T, std::string>>;

public:

	MyVectorType(void) :data_(std::make_shared<vec_type>()) {}

	MyVectorType(MyVectorType&& other) noexcept = default;

	MyVectorType& operator=(const MyVectorType& other) = default;

	MyVectorType& operator=(MyVectorType&& other) noexcept = default;

	MyVectorType(const MyVectorType& other) = default;

	~MyVectorType(void) = default;



	void push_back(const T& obj, const std::string& name)
	{
		copy_names();

		this->data_->emplace_back(obj, name);
	}


	std::pair<const T&, const std::string&> operator[](size_t index) const
	{

		if (index >= this->data_->size())
		{
			throw std::out_of_range("Index is out of range");
		}

		return this->data_->operator[](index);

	}


	std::pair<T, std::string>& operator[](size_t index)
	{

		if (index >= this->data_->size())
		{
			throw std::out_of_range("Index is out of range");
		}

		return this->data_->operator[](index);

	}

	const T& operator[](const std::string& name) const
	{
		return this->find(name);
	}

	T& operator[](const std::string& name)
	{
		return this->find(name);
	}

	auto begin(void) noexcept
	{
		return this->data_->begin();
	}

	auto end(void) noexcept
	{
		return this->data_->end();
	}

	auto cbegin(void) const noexcept
	{
		return this->data_->begin();
	}

	auto cend(void) const noexcept
	{
		return this->data_->end();
	}

	auto empty(void) const noexcept
	{
		return this->data_->empty();
	}

	auto size(void) const noexcept
	{
		return this->data_->size();
	}

	void clear(void) noexcept
	{
		this->data_->clear();
	}

	void reserve(size_t new_size)
	{
		this->data_->reserve(new_size);
	}

private:
	std::shared_ptr<vec_type> data_{};

	void copy_names(void)
	{
		if (this->data_.use_count() != 1)
		{
			this->data_ = std::make_shared<vec_type>(*this->data_);
		}

	}

	T& find(const std::string& name) const
	{
		for (size_t i{}; i < this->data_->size()/*should optimize*/; ++i)
		{
			if (this->data_->operator[](i).second == name)
			{
				return this->data_->operator[](i).first;
			}

		}

		throw std::invalid_argument(name + " is not found in the MyVectorType")
	}


};
