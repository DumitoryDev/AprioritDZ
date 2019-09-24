#ifndef CODEREVIEWTASK_MYVECTOR_HPP
#define CODEREVIEWTASK_MYVECTOR_HPP

template <typename T>
class MyVector : public std::vector<T> //Inheritance from a vector is not necessary. Need to use aggregation
{
	//Copy constructor and constructor defined, but no operators
public:
    MyVector()
    {
        m_ref_ptr = new size_t(1);
        m_names = new std::vector<std::string>(); //Allocation of memory in the constructor, leaks are possible. Need to use smart pointers
    }

    MyVector(const MyVector& other)
        : std::vector<T>(other),
          m_ref_ptr(other.m_ref_ptr), //Initialization of variables is not in the order in which they are written in the class, this can be confusing, since initialization will happen in the order in which the variables are written
          m_names(other.m_names)
    {
        (*m_ref_ptr)++; //Постфиксный инкремент, лучше префиксный использовать
    }

    ~MyVector()
    {
        if (--*m_ref_ptr == 0) // can reduce the code. The expression in if will still be converted to bool, so you can just put it! (logical not)
        {
            delete m_ref_ptr;
            delete m_names;
        }
    }

    void push_back(const T& obj, const std::string& name)
    {
        copy_names();

        std::vector<T>::push_back(obj); //it is better to use emplace_back, it does not copy if possible. But in this example, there will not be much difference
        m_names->push_back(name); //here too
    }

    std::pair<const T&, const std::string&> operator[](int index) const
    {
        if (index >= std::vector<T>::size())
        {
            throw new std::out_of_range("Index is out of range");// inappropriate throwing (new is used)
        }
		    	
        return std::pair<const T&, const std::string&>(std::vector<T>::operator[](index), (*m_names)[index]); //there is make_pair. Or, in general, you can use the "lazy" initialization {}
    }

    const T& operator[](const std::string& name) const
    {
        std::vector<std::string>::const_iterator iter = std::find(m_names->begin(), m_names->end(), name); //мyou can use auto, it is better to use constant versions of iterators
    	
        if (iter == m_names->end()) //it is better to compare iterators of one type of constancy
        {
            throw new std::invalid_argument(name + " is not found in the MyVector"); // throwing and turning off not by value (new is used). The second problem is the + operator of string. This operator does not have a noexcept specifier, which means that this operator can throw an exception, even if new works fine, it is not a fact that the memory leak will not occur due to the operator
        }
						
        return std::vector<T>::operator[](iter - m_names->begin()); //for such things, there is std :: distance, which is safer. Also, iter is a constant iterator, and begin () returns not constant, it is better to use one type.
    }

private:
    void copy_names()
    {
        if (*m_ref_ptr == 1)
        {
            return;
        }

    	//Use smart pointers.
        size_t* temp_ref_ptr = new size_t(1);//use auto, because duplication of type names
        std::vector<std::string>* temp_names = new std::vector<std::string>(*m_names); //five possible leak. Мuse auto, because duplication of type names

        (*m_ref_ptr)--; //better use prefix increment
        m_ref_ptr = temp_ref_ptr;
        m_names = temp_names;
    }

private: //what for two times to write private?
    //
    // Use copy-on-write idiom for efficiency (not a premature optimization)

    //it is better to always initialize the variables with the default value, if possible.

    std::vector<std::string>* m_names; //Better use a vector pair + smart pointer
    size_t* m_ref_ptr;
};


#endif //CODEREVIEWTASK_MYVECTOR_HPP