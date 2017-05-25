#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
	public:
		using key_type = KeyType;
		using mapped_type = ValueType;
		using value_type = std::pair<const key_type, mapped_type>;
		using size_type = std::size_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		class ConstIterator;
		class Iterator;
	
		using iterator = Iterator;
		using const_iterator = ConstIterator;
		
	private:
		static const size_type Hash_size = 400;
		std::list<value_type>* Hash_table;
		
	public: 
		HashMap() 
        {
            Hash_table = new std::list<value_type>[Hash_size];
        }

		HashMap(std::initializer_list<value_type> list): HashMap()
		{
			for (auto it=list.begin(); it != list.end(); ++it)
			{
				size_type index = Hash_function((*it).first);
				Hash_table[index].push_back(*it);
			}				
		}	

		HashMap(const HashMap& other) : HashMap()
        { 
			for (auto it=other.begin(); it != other.end(); ++it)
			{
				size_type index = Hash_function((*it).first);
				Hash_table[index].push_back(*it);
			}		  
        }

		HashMap(HashMap&& other): HashMap()
		{
			for (auto it=other.begin(); it != other.end(); ++it)
			{
				size_type index = Hash_function((*it).first);
				Hash_table[index].push_back(*it);
			}
			other.clear();
		}

		HashMap& operator=(const HashMap& other)
		{
			if(this == &other)
				return *this;
			else
			{
				clear();
				for (auto it=other.begin(); it != other.end(); ++it)
					Hash_table[Hash_function((*it).first)].push_back(*it);
			}
			return *this;	
		}

		HashMap& operator=(HashMap&& other)
		{
			if(this == &other)
				return *this;
			else
			{
				clear();
				for (auto it=other.begin(); it != other.end(); it++)
					Hash_table[Hash_function((*it).first)].push_back(*it);
			}
			other.clear();
			return *this;	
		}

		bool isEmpty() const
		{
			for(size_type index = 0; index<Hash_size; index++)
				if (Hash_table[index].empty() == false)
					return false;
			return true;
		}

		mapped_type& operator[](const key_type& key)
		{
			size_type index = Hash_function(key);
			if(isEmpty() == false)
			{
				for (auto it=Hash_table[index].begin(); it!=Hash_table[index].end(); ++it)
					if ((*it).first == key)
						return (*it).second;	
			}
			// adding empty key			
			Hash_table[index].push_back(std::make_pair(key,mapped_type{}));
			auto it = Hash_table[index].end();
			return (*(--it)).second;	 
		}

		const mapped_type& valueOf(const key_type& key) const
		{
			if(isEmpty() == false)
			{
			size_type index = Hash_function(key);
			for (auto it=Hash_table[index].begin(); it!=Hash_table[index].end(); ++it)
				if ((*it).first == key)
					return (*it).second;
			}
			throw std::out_of_range("No such key");		
		}

		mapped_type& valueOf(const key_type& key)
		{
			if(isEmpty() == false)
			{
			size_type index = Hash_function(key);
			for (auto it=Hash_table[index].begin(); it!=Hash_table[index].end(); ++it)
				if ((*it).first == key)
					return (*it).second;
			}
			throw std::out_of_range("No such key");	
		}

		const_iterator find(const key_type& key) const
		{
			size_type index = Hash_function(key);
			auto it = Hash_table[index].begin();
			while(it != Hash_table[index].end() )
			{
				if ((*it).first == key)
					return const_iterator(this, index,it);
				it++;
			}
			return end();
		}
		
		iterator find(const key_type& key)
		{
			size_type index = Hash_function(key);
			auto it = Hash_table[index].begin();
			while(it != Hash_table[index].end() )
			{
				if ((*it).first == key)
					return iterator(this,index,it);
				it++;
			}
			return end();
		}

		void remove(const const_iterator& it)
		{
			if (it == end())
				throw std::out_of_range("can`t remove end");
			//if(Hash_table[it.index].empty())
				//return;
			Hash_table[(it.index)].erase(it.iter);
		}

		void remove(const key_type& key)
		{			
			if(isEmpty() == true)
				throw std::out_of_range("empty map");
			const_iterator it = find(key);
			remove(it);
		}

		size_type getSize() const
		{
			size_type counter = 0;
			for (size_type index=0; index<Hash_size; index++)
			  counter = counter + Hash_table[index].size();			
			return counter;
		}

		bool operator==(const HashMap& other) const
		{
			auto it_other = other.begin();
			for(auto it = begin(); it!=end(); ++it) 
			{
				if(*it != *(it_other++))
					return false;
			}
			return true;
		}

		bool operator!=(const HashMap& other) const
		{
			return !(*this == other);
		}

		iterator begin()
		{
			for(size_type index=0; index<Hash_size; ++index)
				if(Hash_table[index].empty() == false)
					return iterator(this,index,Hash_table[index].begin());
			return end();
		}

		iterator end()
		{
			return iterator(this,Hash_size,Hash_table[Hash_size].end());
		}

		const_iterator cbegin() const
		{
			for(size_type i=0; i<Hash_size; ++i)
				if(Hash_table[i].empty() == false)
					return const_iterator(this,i,Hash_table[i].begin());
			return cend();
		}

		const_iterator cend() const
		{
			return const_iterator(this,Hash_size,Hash_table[Hash_size].end());
		}

		const_iterator begin() const
		{
			return cbegin();
		}

		const_iterator end() const
		{
			return cend();
		}
		
		void add_element(key_type& key,mapped_type& val)
		{
			size_type index = Hash_function(key);			
			Hash_table[index].push_back(std::make_pair(key,val));
		}
	private:
		void clear()
		{
			for(size_type index = 0; index<Hash_size; ++index)
				Hash_table[index].clear();
		}
    
		size_type Hash_function(const key_type& key) const
		{
			return std::hash<key_type>()(key)%Hash_size;
		}
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
  using list_iterator = typename std::list<value_type>::iterator;

private:
    const HashMap *myMap;
    size_type index;
    list_iterator iter;
    
    
    friend void HashMap<KeyType, ValueType>::remove(const const_iterator&);
    
public: 
  explicit ConstIterator(const HashMap* my, size_type in, list_iterator it) : myMap(my), index(in), iter(it)
  {}

  ConstIterator(const ConstIterator& other) : ConstIterator(other.myMap, other.index, other.iter)
  {}

  ConstIterator& operator++()
  {
    if (*this == myMap->end())
            throw std::out_of_range("end of map ");
    ++iter;
      
    if (iter == myMap->Hash_table[index].end())
    {
      for (size_type i=index+1; i!=myMap->Hash_size; ++i)
        if(!myMap->Hash_table[i].empty())
        {
          iter=myMap->Hash_table[i].begin();
          index=i;
          return *this;
        }
        *this = myMap->end();
        return *this;
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  ConstIterator& operator--()
  {
    if (*this == myMap->begin())
            throw std::out_of_range("out of range - operator--()");
    
    if (*this == myMap->end())
      for (size_type i=Hash_size-1; i>=0; --i)
        if(!myMap->Hash_table[i].empty())
        {
          iter=myMap->Hash_table[i].end();
          index=i;
          --iter;
          return *this;
        }
        
    if (iter == myMap->Hash_table[index].begin())
    {
      for (size_type i=index-1; i>=0; --i)
        if(!myMap->Hash_table[i].empty())
        {
          iter=myMap->Hash_table[i].end();
          index=i;
          --iter;
          return *this;
        }
    }
    --iter;
    return *this;
            
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  reference operator*() const
  {
    if ( iter == myMap->Hash_table[Hash_size].end() )
      throw std::out_of_range("end of table");
    return *iter;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (myMap == other.myMap && iter == other.iter && index==other.index);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;
  using list_iterator = typename std::list<value_type>::iterator;
    
public:
  explicit Iterator(const HashMap* my, size_type in,list_iterator it) : ConstIterator(my,in,it)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
