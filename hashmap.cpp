#include <cmath>
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>


template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {
 public:
	typedef std::pair<const KeyType, ValueType> element;
	typedef std::list<element> my_list;
	typedef typename my_list::iterator iterator;
	typedef typename my_list::const_iterator const_iterator;

 private:
	size_t capacity = 1;
	size_t size_ = 0;

	my_list list_of_el;
	std::vector<iterator> start_by_key = std::vector<iterator>(capacity, list_of_el.end());
	Hash hasher;

	size_t hash_place(const KeyType &key) const {
		return hasher(key) % capacity;
	}

	iterator start_it(const KeyType &key) {
		return start_by_key[hash_place(key)];
	}

	const_iterator start_it(const KeyType &key) const {
		return start_by_key[hash_place(key)];
	}

	iterator it_by_key(const KeyType &key) {
		iterator my_start = start_it(key);
		iterator it = my_start;
		while (it != list_of_el.end() && start_it((*it).first) == my_start) {
			if ((*it).first == key)
				return it;
			it++;
		}
		return list_of_el.end();
	}

	const_iterator it_by_key(const KeyType &key) const {
		const_iterator my_start = start_it(key);
		const_iterator it = my_start;
		while (it != list_of_el.end() && start_it((*it).first) == my_start) {
			if ((*it).first == key)
				return it;
			it++;
		}
		return list_of_el.end();
	}

	bool exist(const KeyType &key) const{
		return it_by_key(key) != list_of_el.end();
	}

	const ValueType& value(const KeyType &key) const {
		if (exist(key))
			return (*it_by_key(key)).second;
		exit(0);
	}

	ValueType& value(const KeyType &key) {
		if (exist(key))
			return (*it_by_key(key)).second;
		exit(0);
	}

	void rehash() {
		size_t new_capacity = capacity * 2;

		my_list old_list = list_of_el;
		for (auto p : old_list) {
			eraser(p.first);
		}

		list_of_el.clear();
		start_by_key.clear();
		start_by_key.resize(new_capacity, list_of_el.end());
		capacity = new_capacity;

		for (auto p : old_list) {
			inserter(p);
		}	
	}

	void inserter(const element &to_add) {
		if (size_ == capacity)
			rehash();

		KeyType key = to_add.first;
		iterator old_start = start_it(key);
		list_of_el.insert(old_start, to_add);
		start_by_key[hash_place(key)]--;

		size_++;
	}

	void eraser(const KeyType &key) {
		iterator it = it_by_key(key);
		iterator start = start_it(key);
		iterator new_start = start;
		if (start == it) {
			new_start++;
			if (new_start != list_of_el.end()) {
				if (start_it((*new_start).first) != start)
					new_start = list_of_el.end();
			}
		}
		start_by_key[hash_place(key)] = new_start;
		list_of_el.erase(it);

		size_--;
	}


 public:
	HashMap(Hash new_hasher = Hash()) : hasher(new_hasher) {		
	}	

	template<class Iter>
	HashMap(Iter it_begin, Iter it_end, Hash new_hasher = Hash()) : hasher(new_hasher) {
		for (auto it = it_begin; it != it_end; it++) {
			insert(*it);
		}
	}	

	HashMap(std::initializer_list<element> init_list, Hash new_hasher = Hash()) : hasher(new_hasher) {
		for (auto it: init_list)
			insert(it);
	}	

	HashMap(const HashMap &other) {
		clear();
		hasher = other.hasher;
		for (auto it = other.begin(); it != other.end(); it++) {
			inserter(*it);
		}
	}

	HashMap& operator = (const HashMap &other) {
		auto to_add = other.list_of_el;
		clear();
		hasher = other.hasher;
		for (auto p : to_add)
			inserter(p);
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

	void insert(const element &to_add)	{
		if (exist(to_add.first))
			return;
		inserter(to_add);		
	}

	void erase(const KeyType &key)	{
		if (!exist(key))
			return;
		eraser(key);
	}

	void clear() {
		my_list old_list = list_of_el;
		for (auto p : old_list)
			eraser(p.first);
	}

	iterator begin() {
		return list_of_el.begin();
	}

	const_iterator begin() const {
		return list_of_el.begin();
	}

	iterator end() {
		return list_of_el.end();
	}
	
	const_iterator end() const {
		return list_of_el.end();
	}

	iterator find(const KeyType &key) {
		return it_by_key(key);
	}

	const_iterator find(const KeyType &key) const {
		return it_by_key(key);
	}

	ValueType& operator [](const KeyType &key) {
		if (!exist(key)) {
			insert((element){key, ValueType()});
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
