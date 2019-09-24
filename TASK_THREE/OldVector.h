#ifndef CODEREVIEWTASK_MYVECTOR_HPP
#define CODEREVIEWTASK_MYVECTOR_HPP

template <typename T>
class MyVector : public std::vector<T> //Наследование от вектора не явлется необходимым. Нужно использовать агрегацию
{
	//Определен конструктор и конструктор копирования, но операторы - нет
public:
    MyVector()
    {
        m_ref_ptr = new size_t(1);
        m_names = new std::vector<std::string>(); //Выделение памяти в конструкторе, возможны утечки. Нужно использовать умные указаетли
    }

    MyVector(const MyVector& other)
        : std::vector<T>(other),
          m_ref_ptr(other.m_ref_ptr), //Инициализия переменных не в порядке написания их в классе, это может запутать, т.к. всё равно инициализация произойдет в порядке написания переменных
          m_names(other.m_names)
    {
        (*m_ref_ptr)++; //Постфиксный инкремент, лучше префиксный использовать
    }

    ~MyVector()
    {
        if (--*m_ref_ptr == 0) // можно уменьшить код. Выражние в if все равно будет преведено к bool, значит, можно просто поставить ! (логическое не)
        {
            delete m_ref_ptr;
            delete m_names;
        }
    }

    void push_back(const T& obj, const std::string& name)
    {
        copy_names();

        std::vector<T>::push_back(obj); //лучше использовать emplace_back, он по возможности не производит копирования. Но в данном примере особой разницы не будет, скорее всего
        m_names->push_back(name); //тут тоже
    }

    std::pair<const T&, const std::string&> operator[](int index) const
    {
        if (index >= std::vector<T>::size())
        {
            throw new std::out_of_range("Index is out of range");// бросание иключение не по значению(используется new)
        }
		    	
        return std::pair<const T&, const std::string&>(std::vector<T>::operator[](index), (*m_names)[index]); //есть make_pair. Или вообще можно "ленивую" инициализацию {} использовать 
    }

    const T& operator[](const std::string& name) const
    {
        std::vector<std::string>::const_iterator iter = std::find(m_names->begin(), m_names->end(), name); //можно использовать auto, лучше использовать константные версии итераторов
    	
        if (iter == m_names->end()) //лучше сравнивать итераторы одного типа константности
        {
            throw new std::invalid_argument(name + " is not found in the MyVector"); // бросание иключение не по значению(используется new). Вторая проблема - оператор + у string. У этого оператора нет noexcept спецификатора, значит, этот оператор может кинуть исключение, даже если new отработает нормально, то не факт, что из-за оператора не случится утечка памяти
        }
						
        return std::vector<T>::operator[](iter - m_names->begin()); //Для таких вещей есть std::distance, который безопаснее. Так же iter это константный итератор, а begin()  возвращает не константный, лучше использовать один тип.
    }

private:
    void copy_names()
    {
        if (*m_ref_ptr == 1)
        {
            return;
        }

    	//Нужно использовать умные указатели 
        size_t* temp_ref_ptr = new size_t(1);//Можно использовать auto, т.к. идет дублирование имен типов
        std::vector<std::string>* temp_names = new std::vector<std::string>(*m_names); //Опять возможная утечка. Можно использовать auto, т.к. идет дублирование имен

        (*m_ref_ptr)--; //Лучше поставить префиксный декримент
        m_ref_ptr = temp_ref_ptr;
        m_names = temp_names;
    }

private: //нафига два раз писать private?
    //
    // Use copy-on-write idiom for efficiency (not a premature optimization)
	//Вообще, лучше всегда инициализировать переменные дефолтным значением, если это возможно.
    std::vector<std::string>* m_names; //Лучше сделать вектор пар + умный указатель
    size_t* m_ref_ptr;
};


#endif //CODEREVIEWTASK_MYVECTOR_HPP