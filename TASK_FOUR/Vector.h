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




template <  typename T, template <typename Y> class Allocator = std::allocator >
class Vector
{
public:
	using AllocatorType = Allocator<T>;
	using AllocatorTraits = std::allocator_traits<AllocatorType>;
	using VectorType = Vector<T, Allocator>;

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
		data_(AllocatorTraits::allocate(this->allocator_, this->capacity_))
	{
		std::size_t index{};
		for (const auto& element : values)
		{
			AllocatorTraits::construct(this->allocator_, this->data_ + index, element);
			++index;
		}


	}

	explicit Vector(
		const size_t size,
		const T& value = T(),
		const AllocatorType& allocator = AllocatorType())
		:
		allocator_(allocator),
		size_(size),
		capacity_(this->size_ + this->step_capacity_),
		data_(AllocatorTraits::allocate(this->allocator_, this->capacity_))
	{
		for (size_t i{}; i < this->size_; ++i)
		{
			AllocatorTraits::construct(this->allocator_, this->data_ + i, value);
		}

	}

	Vector(const Vector& rhs) :
		allocator_(rhs.allocator_),
		size_(rhs.size_),
		capacity_(rhs.capacity_),
		data_(AllocatorTraits::allocate(this->allocator_, this->capacity_))

	{
		for (size_t i{}; i < this->size_; ++i)
		{
			AllocatorTraits::construct(this->allocator_, this->data_ + i, *(rhs.data_ + i));
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
		AllocatorTraits::construct(this->allocator_, this->data_ + size_, element);
		++this->size_;


	}


	void push_back(T&& element)
	{
		this->check_and_edit_free_memory();
		AllocatorTraits::construct(this->allocator_, this->data_ + size_, std::move(element));
		++this->size_;

	}

	void pop_back(void)
	{
		AllocatorTraits::destroy(this->allocator_, this->data_ + this->size_ - 1);
		--this->size_;
	}


	void swap(Vector& other) noexcept
	{
		std::swap(this->data_, other.data_);
		std::swap(this->allocator_, other.allocator_);
		std::swap(this->size_, other.size_);
		std::swap(this->capacity_, other.capacity_);
	}


	T* data() noexcept
	{
		return &this->data_;
	}

	[[nodiscard]] const T* data() const noexcept
	{
		return this->data_;
	}

	[[nodiscard]] bool empty() const noexcept
	{
		return this->size_;
	}


	void resize(std::size_t new_size)
	{
		this->resize_and_move_memory(new_size);
		this->capacity_ = this->size_ = new_size;
	}

	void reserve(size_t new_capacity)
	{
		if (this->size_ > new_capacity)
		{
			return; //throw
		}
		this->capacity_ = new_capacity;
		this->move_memory();
	}

	T& operator [](size_t index)
	{
		return this->data_[index];
	}

	const T& operator [](size_t index) const
	{
		return this->data_[index];
	}

	T& at(size_t index)
	{
		return this->find(index);
	}

	[[nodiscard]] const T& at(size_t index) const
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

	size_t step_capacity_{ 2 };
	AllocatorType allocator_{};
	size_t size_{};
	size_t capacity_{};
	T* data_{};

	void clear(void) noexcept
	{
		for (size_t i{}; i < this->size_; ++i)
		{
			AllocatorTraits::destroy(this->allocator_, this->data_ + i);
		}

		AllocatorTraits::deallocate(this->allocator_, this->data_, this->capacity_);

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

		T* new_data = AllocatorTraits::allocate(this->allocator_, this->capacity_);

		for (size_t i{}; i < this->size_; ++i)
		{
			AllocatorTraits::construct(
				this->allocator_, 
				new_data + i, 
				std::move(*(this->data_ + i)));
			
			AllocatorTraits::destroy(this->allocator_, this->data_ + i);
		}

		AllocatorTraits::deallocate(this->allocator_, this->data_, this->size_);

		this->data_ = new_data;
	}

	void resize_and_move_memory(size_t new_size)
	{
		T* new_data = AllocatorTraits::allocate(this->allocator_, new_size);

		size_t index{};

		for (; index != new_size; ++index)
		{
			if (index < this->size_)
			{
				AllocatorTraits::construct(
					this->allocator_, 
					new_data + index, 
					std::move(*(this->data_ + index)));
				
				continue;
			}

			AllocatorTraits::construct(this->allocator_, new_data + index);

		}


		this->clear();
		this->data_ = new_data;

	}

	[[nodiscard]] T & find(size_t index) const 
	{
		if (index >= this->size_)
		{
			throw std::out_of_range("Invalid index!");
		}

		return this->data_[index];
	}


	
};