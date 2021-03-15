#pragma once
#include<iostream>
#include"Exceptions.h"
template <class T>
class DynamicArray
{
public:

	
	DynamicArray(int size = 0) : log(size), phy(size)

	{
		if (size < 0)
		{
			throw std::out_of_range("out of found DynamicArray ctor");
		}
		else if (size > 0)
		{

			try
			{
				arr = new T[phy]();
			}
			catch (...)
			{
				delete arr;
				throw;
			}
		}

	}

	DynamicArray(const DynamicArray& other) : arr(nullptr) {
		*this = other;
	}
	~DynamicArray() {
		delete[] arr;
	}

	 DynamicArray& operator=(const DynamicArray& other) {
		if (this != &other) {
			log = other.log;
			phy = other.phy;
			delete[] arr;
			arr = nullptr;
			try
			{
				arr = new T[phy]();
				for (int i = 0; i < log; i++)
					arr[i] = other.arr[i];
			}
			catch (...)
			{
				log = phy = 0;
				throw MemoryException("DynamicArrar::operator= memory error");
			}
			
		}
		return *this;
	}

	const T& operator[](int i) const { return arr[i]; }
	T& operator[](int i) { return arr[i]; }

	void push_back(const T& value) {
		fixSize();
		arr[log++] = value;
	}


	const T&back()const { return arr[log - 1]; }
	T&back() { return arr[log - 1]; }
	const T& front()    const { return arr[0]; }
	T&front() { return arr[0]; }
	int      size()     const { return log; }
	int      capacity() const { return phy; }
	bool     empty()    const { return log == 0; }
	void     clear()
	{
		log = phy = 0;
		delete[]arr;
		arr = nullptr;
	}
	void resize(int size)
	{
		if (size < 0)
		{
			throw std::out_of_range("size must be positive!!!");
		}

		if (size > phy)
		{
			reserve(size * 2);

		}
		log = size;

	}
	void reserve(int phy)
	{
		if (phy <= this->phy)
			return;
		T*temp = nullptr;
		try
		{
			temp = new T[phy]();
			for (int i = 0; i < log; i++)
				temp[i] = arr[i];
			delete[]arr;
			arr = temp;
			this->phy = phy;
		}
		catch (std::bad_alloc&)
		{
			throw MemoryException("memory exception in DynamicArray::revserve");
		}


	}

	// example iterator implementation:
	// (causes code duplication)
	template <bool is_const, bool is_reverse>
	class base_iterator
	{
	public:
		using ds_type = std::conditional_t<is_const, const DynamicArray, DynamicArray>;

		//using iterator_category = std::bidirectional_iterator_tag;
		using iterator_category = std::bidirectional_iterator_tag;
		// other options exist, e.g., std::forward_iterator_tag
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<is_const, const T, T>;
		using pointer = value_type * ;
		using reference = value_type & ;

		bool operator <(const base_iterator&other)const
		{
			return _i < other._i;
		}
		bool operator >(const base_iterator&other)const
		{
			return _i > other._i;
		}
		bool operator<=(const base_iterator&other)const
		{
			return ((*this) < other) || (*this) == other;
		}
		base_iterator(ds_type& arr, int i) : _da(&arr), _i(i) {}

		// we want to use the default constructor
		base_iterator(const base_iterator&) = default;

		// and the default assignment operator
		constexpr base_iterator& operator=(const base_iterator&) = default;

		// we want to allow construction of const_iterator from iterator
		friend class base_iterator<true, is_reverse>;
		template <bool _is_const = is_const, class = std::enable_if_t<_is_const>>
		base_iterator(const base_iterator<false, is_reverse>& other) : _da(other._da), _i(other._i) {}

		// comparison with another iterator
		bool operator==(const base_iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const base_iterator& other) const {
			return !(*this == other);
		}
		template <bool notReverse = !is_reverse, class = std::enable_if_t<notReverse>>
		base_iterator operator+(int x)const
		{
			return base_iterator(*_da, _i + x);
		}
		template <bool notReverse = !is_reverse, class = std::enable_if_t<notReverse>>
		base_iterator operator-(int x)const
		{
			base_iterator itr = *this;
			itr._i -= x;
			return itr;
		}
		template <bool notReverse = !is_reverse, class = std::enable_if_t<notReverse>>
		std::ptrdiff_t operator -(const base_iterator&other)
		{
			return (_i - other._i);
		}
		template <bool notReverse = !is_reverse, class = std::enable_if_t<notReverse>>
		std::ptrdiff_t operator -(const base_iterator&other)const
		{
			return (_i - other._i);
		}




		// smart-pointer iterator methods
		reference operator*() {
			return _da->arr[_i];
		}
		pointer operator->() {
			return &_da->arr[_i];
		}

		// increment-decrement iterator methods
		base_iterator& operator++() {
			_i += is_reverse ? -1 : 1;
			return *this;
		}
		base_iterator operator++(int) {
			base_iterator temp(*this);
			_i += is_reverse ? -1 : 1;
			return temp;
		}
		base_iterator& operator--() {
			_i += is_reverse ? 1 : -1;
			return *this;
		}
		base_iterator operator--(int) {
			base_iterator temp(*this);
			_i += is_reverse ? 1 : -1;
			return temp;
		}
	private:
		ds_type*	_da;
		int			_i;

	};



	using iterator = base_iterator<false, false>;
	using const_iterator = base_iterator<true, false>;
	using reverse_iterator = base_iterator<false, true>;
	using const_reverse_iterator = base_iterator<true, true>;
	const T&at(int index)const
	{
		
		isOutOfBound(const_iterator(*this,index));
		return arr[index];
	}
	T&at(int index)
	{
		const DynamicArray&thisconst = *this;
		return const_cast<T&>(thisconst.at(index));
	}


	iterator insert(const iterator& pos, const T& val) {
		fixSize();
		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++log;
		return p;
	}

	 iterator erase(const iterator& iter)
	{
		isOutOfBound((iter));
		iterator prev = iter;
		iterator next = iter + 1;
		for (auto next = iter + 1; next != end(); ++next, ++prev)
		{
			*prev = *next;
		}

		log--;
		return iter;
	}
	 iterator erase(const iterator& first, const iterator& last)
	{
		isOutOfBound(first);
		if (!(first < last&&last <= end()))
			throw out_of_range("out of range in DynamicArray::erase2");
		auto prev = first;
		auto next = last;
		for (; next != end(); ++next, ++prev)
		{
			*prev = *next;
		}
		log -= ((last - first));
		return first;

	}
	//normal
	iterator begin() {
		return iterator(*this, 0);
	}
	iterator end() {
		return iterator(*this, log);
	}
	const_iterator begin() const {
		return cbegin();
	}
	const_iterator end() const {
		return cend();
	}
	const_iterator cbegin()const
	{
		return const_iterator(*this, 0);
	}
	const_iterator cend()const
	{
		return const_iterator(*this, log);
	}
	//reverse
	reverse_iterator rbegin()
	{
		return reverse_iterator(*this, log - 1);
	}
	reverse_iterator rend()
	{
		return reverse_iterator(*this, -1);
	}
	const_reverse_iterator rend()const
	{
		return crend();
	}
	const_reverse_iterator rbegin()const
	{
		return crbegin();

	}
	const_reverse_iterator crend()const
	{
		return const_reverse_iterator(*this, -1);
	}
	const_reverse_iterator crbegin()const
	{
		return const_reverse_iterator(*this, log - 1);

	}





private:
	void fixSize()
	{
		if (phy == 0)
			reserve(1);
		else if (log == phy)
			reserve(phy * 2);
	}
	void isOutOfBound( const_iterator itr)const
	{
		if (log == 0 ||cend()<=itr || itr < cbegin())
			throw out_of_range("out of range DynamicArray");
	}

	T* arr = nullptr;
	int log = 0;
	int phy = 0;
};
