#pragma once
#include <iostream>
#include <atomic>

template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr
{
	struct Count
	{
		std::atomic_size_t count = 1;
	};

	friend class WeakPtr<T>;
public:
	
	
	explicit SharedPtr(T* ptr = nullptr) :ptr_(ptr)
	{
		if (ptr)
			this->data_ = new Count();

	}


	SharedPtr(SharedPtr&& other) noexcept
	{
		this->Swap(other);
	}


	SharedPtr& operator = (SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			this->Clear();
			this->Swap(other);
			
		}

		return *this;

	}

	SharedPtr(const SharedPtr& right) :ptr_(right.ptr_), data_(right.data_)
	{

		if (right.ptr_)
			++this->data_->count;


	}

	SharedPtr& operator=(const SharedPtr& right)
	{
		if (this != &right)
		{
			SharedPtr(right).Swap(*this);
		}

		return *this;

	}


	~SharedPtr(void)
	{
		this->Clear();
	}


	T* get(void) const
	{
		return  this->ptr_;
	}

	void Reset(T* ptr = {})
	{
		if (ptr != this->ptr_)
			SharedPtr(ptr).Swap(*this);
	}

	T& operator*(void) const
	{
		return  *this->ptr_;
	}

	T* operator->(void) const
	{
		return this->ptr_;
	}

	std::size_t UseCount(void) const noexcept
	{
		return this->data_ ? this->data_->count : 0;
	}


	void Swap(SharedPtr& other) noexcept
	{
		std::swap(this->data_, other.data_);
		std::swap(this->ptr_, other.ptr_);
	}

private:
	T* ptr_{};
	Count* data_{};

	void Clear(void) noexcept
	{

		if (this->ptr_ && !--(data_->count))
		{
			delete ptr_;
			delete data_;

			this->ptr_ = nullptr;
			this->data_ = nullptr;
#ifdef _DEBUG
			std::cout << "Clear memory SUCCESS!" << std::endl;
#endif

		}

	}

};


template <class T, class... Args>
SharedPtr<T> MakeShared (Args&& ... args)
{
	return SharedPtr<T>(new T(std::forward<T>(args)...));
}

