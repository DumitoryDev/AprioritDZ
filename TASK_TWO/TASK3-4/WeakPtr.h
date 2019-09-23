#pragma once
#include "SharedPtr.h"

template<typename T>
class WeakPtr
{
public:

	void Swap(WeakPtr<T>& rhs) noexcept
	{
		std::swap(this->data_, rhs.data_);
		std::swap(this->ptr_, rhs.ptr_);
	}

	WeakPtr(void) = default;
	~WeakPtr(void) = default;

	WeakPtr(const WeakPtr<T>& rhs) :ptr_(rhs.ptr_), data_(rhs.data_)
	{

	}

	WeakPtr(WeakPtr<T>&& rhs) noexcept
	{
		this->Swap(rhs);
	}
	
	WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept
	{
		if (this != &other)
		{
			other.Swap(*this);
			WeakPtr().Swap(other);
						
		}

		return *this;
	}
	
	WeakPtr<T>& operator=(const WeakPtr<T>& other)
	{
		if (this != &other)
		{
			WeakPtr(other).Swap(*this);
						
		}

		return *this;
	}

	explicit WeakPtr(const SharedPtr<T>& rhs) 
	{
		if (rhs.get())
		{
			this->data_ = &rhs.data_;
			this->ptr_ = rhs.ptr_;
		}
	}

	WeakPtr<T>& operator=(const SharedPtr<T>& other)
	{
		if (other.get())
		{
			this->ptr_ = other.ptr_;
			this->data_ = &other.data_;
		}
		return *this;
	}
		
	
	void Reset(void) noexcept
	{
		this->ptr_ = nullptr;
		this->data_ = nullptr;

	}

	bool Expired(void) const noexcept
	{
		return this->data_ ? *this->data_ : false;
	}

	std::size_t UseCount(void) const noexcept
	{
		
	 	return  (this->data_ && *this->data_) ? (*this->data_)->count: 0;
				
	}
	
	auto Lock(void) const 
	{
		if (this->data && *this->data_)
		{
			SharedPtr<T> tmp{};
			tmp.data_ = *const_cast<typename SharedPtr<T>::Count**>(this->data_);
			tmp.ptr_ = this->ptr_;
			++tmp.data_->count;
			
			return tmp;
		}
		return SharedPtr<T>();
	}

private:
	T* ptr_{};
	typename SharedPtr<T>::Count* const * data_{};
};