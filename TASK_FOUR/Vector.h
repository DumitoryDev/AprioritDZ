#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <memory_resource>

template <typename T>
class CustomAllocator : public std::allocator<T>
{
private:
	using Base = std::allocator<T>;

public:
	T* allocate(size_t count)
	{
		std::cout << ">> Allocating " << count << " elements" << std::endl;
		return Base::allocate(count);
	}

	T* allocate(size_t count, const void* p)
	{
		std::cout << ">> Allocating " << count << " elements" << std::endl;
		return Base::allocate(count, p);
	}

	void deallocate(T* p, size_t count)
	{
		if (p != nullptr)
		{
			std::cout << ">> Deallocating " << count << " elements" << std::endl;
			Base::deallocate(p, count);
		}
	}
};




class AllocatorGuard
{
public:
	~AllocatorGuard(void) = delete;
	AllocatorGuard(void) = delete;
	AllocatorGuard(const AllocatorGuard& other) = delete;
        AllocatorGuard(AllocatorGuard&& other) noexcept = delete;
	AllocatorGuard& operator=(const AllocatorGuard& other) = delete;
	AllocatorGuard& operator=(AllocatorGuard&& other) noexcept = delete;

	template <typename T, template <typename Y> class Allocator >
	static auto make( Allocator<T>  allocator, size_t count)
	{
		return std::unique_ptr<T[],std::function<void(T*)>>(
			std::allocator_traits<Allocator<T>>::allocate(allocator,count),
			[allocator,count](T * data)  mutable 
		{
			std::allocator_traits<Allocator<T>>::deallocate(allocator,data,count);
		});
	}
};

template <  typename T, template <typename Y> class Allocator = std::allocator >
class Vector
{
public:
	using AllocatorType = Allocator<T>;
	using AllocatorTraits = std::allocator_traits<AllocatorType>;
	using VectorType = Vector<T, Allocator>;
	using u_ptr_data = std::unique_ptr<T[],std::function<void(T *)>>;

	explicit Vector(const AllocatorType& allocator = AllocatorType()) :allocator_(allocator)
	{
		
	}

	Vector(
		const std::initializer_list<T>& values,
		const AllocatorType& allocator = AllocatorType()
	) :
		allocator_(allocator),
		size_(values.size()),
		capacity_(this->size_ + this->step_capacity_),
	        data_(AllocatorGuard::make(this->allocator_,this->capacity_))
		
	{
				
		std::size_t index{};
		for (const auto& element : values)
		{
			new (this->data_.get() + index) T(element);
			
			++index;
		}


	}

	explicit Vector(
		const std::size_t size,
		const T& value = T(),
		const AllocatorType& allocator = AllocatorType())
		:
		allocator_(allocator),
		size_(size),
		capacity_(this->size_ + this->step_capacity_),
		data_(AllocatorGuard::make(this->allocator_,this->capacity_))
	{
		   		
		for (size_t i{}; i < this->size_; ++i)
		{
			new (this->data_.get() + i) T(value);
		}

	}

	Vector(const Vector& rhs) :
		allocator_(rhs.allocator_),
		size_(rhs.size_),
		capacity_(rhs.capacity_),
		data_(AllocatorGuard::make(this->allocator_,this->capacity_))
	{
				
		for (size_t i{}; i < this->size_; ++i)
		{
			new (this->data_.get() + i) T(rhs.data_[i]);
		}

	}

	Vector(Vector&& rhs) noexcept
	{
		this->swap(*this, rhs);
	}


	Vector& operator = (const Vector& other)
	{
		if (this != &other)
		{
			Vector(other).swap(*this);
		}

		return *this;
	}

	Vector& operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			this->clear();
			this->swap(other);
		}
		return *this;
	}


	~Vector(void)
	{
		this->clear();
	}


	void push_back(const T& element)
	{

		this->check_and_edit_free_memory();
		new (this->data_[this->size_]) T(element);
		++this->size_;


	}


	void push_back(T&& element)
	{
		this->check_and_edit_free_memory();
		new (this->data_.get() + this->size_) T(std::move(element));
		++this->size_;

	}

	void pop_back(void)
	{
		(this->data_.get() + this->size_)->~T();
		--this->size_;
	}


	void swap(Vector& other) noexcept
	{
		std::swap(this->data_, other.data_);
		std::swap(this->allocator_, other.allocator_);
		std::swap(this->size_, other.size_);
		std::swap(this->capacity_, other.capacity_);
	}


	T* data(void) noexcept
	{
		return &this->data_[0];
	}

	[[nodiscard]] const T* data(void) const noexcept
	{
		return this->data_.get();
	}

	[[nodiscard]] bool empty(void) const noexcept
	{
		return this->size_;
	}


	void resize(std::size_t new_size)
	{
		this->resize_and_move_memory(new_size);
		this->capacity_ = this->size_ = new_size;
	}

	void reserve(std::size_t new_capacity)
	{
		if (this->size_ > new_capacity)
		{
			return; //throw
		}
		this->capacity_ = new_capacity;
		this->move_memory();
	}

	T& operator [](std::size_t index)
	{
		return this->data_[index];
	}

	const T& operator [](std::size_t index) const
	{
		return this->data_[index];
	}

	T& at(std::size_t index)
	{
		return this->find(index);
	}

	[[nodiscard]] const T& at(std::size_t index) const
	{
		return this->find(index);
				
	}

	[[nodiscard]] auto size(void) const noexcept
	{
		return this->size_;
	}

	[[nodiscard]] auto capacity(void) const noexcept
	{
		return this->capacity_;
	}
		
private:

	std::size_t step_capacity_{ 2 };
	AllocatorType allocator_{};
	std::size_t size_{};
	std::size_t capacity_{};
	u_ptr_data data_{};

	

	void clear(void) noexcept
	{
		for (std::size_t i{}; i < this->size_; ++i)
		{
			(this->data_.get() + i)->~T();
		}
			
		this->size_ = 0;
		this->capacity_ = 0;
		this->data_ = nullptr;

	}


	void check_and_edit_free_memory(void)
	{
		if (this->capacity_ == this->size_)
		{
			this->capacity_ += this->step_capacity_;
			this->move_memory();

		}

	}

	void move_memory(void)
	{
		u_ptr_data new_data(AllocatorGuard::make(this->allocator_,this->capacity_));
					
		for (std::size_t i{}; i < this->size_; ++i)
		{
			new (new_data.get() + i) T(std::move(this->data_[i]));
			(this->data_.get() + i)->~T();
			
		}
				
		this->data_ = std::move(new_data);
	}

	void resize_and_move_memory(std::size_t new_size)
	{
		u_ptr_data new_data(AllocatorGuard::make(this->allocator_,new_size));
		std::size_t index{};

		for (; index != new_size; ++index)
		{
			if (index < this->size_)
			{
				new (new_data.get() + index) T(std::move(this->data_[index]));
				
				continue;
			}

			new (new_data.get() + index) T();
					

		}


		this->clear();
		this->data_ = std::move(new_data);

	}

	[[nodiscard]] T & find(std::size_t index) const 
	{
		if (index >= this->size_)
		{
			throw std::out_of_range("Invalid index!");
		}

		return this->data_[index];
	}


	
};
