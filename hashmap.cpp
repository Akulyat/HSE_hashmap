#include <cmath>
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>


template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {
 public:
	typedef std::pair<const KeyType, ValueType> Element;
	typedef std::list<Element> MyList;
	typedef typename MyList::iterator iterator;
	typedef typename MyList::const_iterator const_iterator;

 private:
	size_t capacity = 1;
	size_t size_ = 0;

	MyList elementList;
	std::vector<iterator> hashStart = std::vector<iterator>(capacity, elementList.end());
	Hash hasher;

	size_t hash_place(const KeyType &key) const {
		return hasher(key) % capacity;
	}

	iterator start_it(const KeyType &key) {
		return hashStart[hash_place(key)];
	}

	const_iterator start_it(const KeyType &key) const {
		return hashStart[hash_place(key)];
	}

	iterator it_by_key(const KeyType &key) {
		iterator myStart = start_it(key);
		iterator it = myStart;
		while (it != elementList.end() && start_it((*it).first) == myStart) {
			if ((*it).first == key) {
				return it;
			}
			it++;
		}
		return elementList.end();
	}

	const_iterator it_by_key(const KeyType &key) const {
		const_iterator myStart = start_it(key);
		const_iterator it = myStart;
		while (it != elementList.end() && start_it((*it).first) == myStart) {
			if ((*it).first == key) {
				return it;
			}
			it++;
		}
		return elementList.end();
	}

	bool exist(const KeyType &key) const{
		return it_by_key(key) != elementList.end();
	}

	const ValueType& value(const KeyType &key) const {
		if (exist(key)) {
			return (*it_by_key(key)).second;
		}
		exit(0);
	}

	ValueType& value(const KeyType &key) {
		if (exist(key)) {
			return (*it_by_key(key)).second;
		}
		exit(0);
	}

	void rehash() {
		size_t newCapacity = capacity * 2;

		MyList oldList = elementList;
		for (auto p : oldList) {
			eraser(p.first);
		}

		elementList.clear();
		hashStart.clear();
		hashStart.resize(newCapacity, elementList.end());
		capacity = newCapacity;

		for (auto p : oldList) {
			inserter(p);
		}	
	}

	void inserter(const Element &toAdd) {
		if (size_ == capacity) {
			rehash();
		}

		KeyType key = toAdd.first;
		iterator oldStart = start_it(key);
		elementList.insert(oldStart, toAdd);
		hashStart[hash_place(key)]--;

		size_++;
	}

	void eraser(const KeyType &key) {
		iterator it = it_by_key(key);
		iterator start = start_it(key);
		iterator newStart = start;
		if (start == it) {
			newStart++;
			if (newStart != elementList.end()) {
				if (start_it((*newStart).first) != start) {
					newStart = elementList.end();
				}
			}
		}
		hashStart[hash_place(key)] = newStart;
		elementList.erase(it);

		size_--;
	}


 public:
	HashMap(Hash newHasher = Hash()) : hasher(newHasher) {		
	}	

	template<class Iter>
	HashMap(Iter begin, Iter end, Hash newHasher = Hash()) : hasher(newHasher) {
		for (auto it = begin; it != end; it++) {
			insert(*it);
		}
	}	

	HashMap(std::initializer_list<Element> initList, Hash newHasher = Hash()) : hasher(newHasher) {
		for (auto it: initList) {
			insert(it);
		}
	}	

	HashMap(const HashMap &other) {
		clear();
		hasher = other.hasher;
		for (auto it = other.begin(); it != other.end(); it++) {
			inserter(*it);
		}
	}

	HashMap& operator = (const HashMap &other) {
		auto toAdd = other.elementList;
		clear();
		hasher = other.hasher;
		for (auto p : toAdd) {
			inserter(p);
		}
		return *this;
	}


	size_t size() const {
		return size_;
	}	

	bool empty() const {
		return size_ == 0;
	}	

	Hash hash_function() const {		
		return hasher;
	}

	void insert(const Element &toAdd)	{
		if (exist(toAdd.first)) {
			return;
		}
		inserter(toAdd);		
	}

	void erase(const KeyType &key)	{
		if (!exist(key)) {
			return;
		}
		eraser(key);
	}

	void clear() {
		MyList oldList = elementList;
		for (auto p : oldList) {
			eraser(p.first);
		}
	}

	iterator begin() {
		return elementList.begin();
	}

	const_iterator begin() const {
		return elementList.begin();
	}

	iterator end() {
		return elementList.end();
	}
	
	const_iterator end() const {
		return elementList.end();
	}

	iterator find(const KeyType &key) {
		return it_by_key(key);
	}

	const_iterator find(const KeyType &key) const {
		return it_by_key(key);
	}

	ValueType& operator [](const KeyType &key) {
		if (!exist(key)) {
			insert((Element){key, ValueType()});
		}
		return value(key);
	}

	const ValueType& at(const KeyType &key) const {
		if (!exist(key)) {
			throw std::out_of_range("");
		}
		return value(key);
	}
};
