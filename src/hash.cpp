#include "hash.hpp"

HashTable::HashTable (int size) {
	int optimalSize = getPrime (size);

	this->capacity = optimalSize;
	this->numOccupied = 0;

	HashItem emptyItem;
	emptyItem.key = "";
	emptyItem.isOccupied = false;
	emptyItem.isDeleted = false;
	emptyItem.pv = nullptr;

	// initialize data vector with empty items
	this->data.resize (optimalSize, emptyItem);
}

int HashTable::insert (std::string const & key, void * pv) {
	if (this->getPos (key) < 0) {
		// key does not exist
		unsigned int pos, offset;
		pos = this->keyHash (key) % this->capacity;
		offset = this->keyHash2 (key) % (this->capacity - 1) + 1;

		int i;

		// hash and insert into the table
		for (i = 0; i < this->capacity; ++i) {
			pos = (pos + i * offset) % this->capacity;
			HashItem & item = this->data.at (pos);

			// if no key found, insert
			if (!item.isOccupied) {
				item.key = key;
				item.pv = pv;
				item.isOccupied = true;
				++this->numOccupied;
				break;
			} else if (item.isDeleted && item.key == key) {
				// "reinsert" lazily deleted item
				item.isDeleted = false;
				break;
			}
		}
	} else return 1; // key already exists

	// if load factor exceeds one-half, rehash
	if ((float) this->numOccupied / (float) this->capacity > 0.5)
		if (!this->rehash ()) return 2;

	return 0;
}

bool HashTable::remove (std::string const & key) {
	// if key is in hash table, set isDeleted to true
	int pos = this->getPos (key);
	if (pos >= 0) {
		(this->data.at (pos)).isDeleted = true;
		return true;
	} else return false;
}

bool HashTable::contains (std::string const & key) {
	if (this->getPos (key) < 0) return false;
	else return true;
}

void * HashTable::getPointer (std::string const & key, bool * b) {
	int pos = this->getPos (key);
	if (pos >= 0) {
		if (b != nullptr) *b = true;
		return (this->data.at (pos)).pv;
	} else {
		if (b != nullptr) *b = false;
		return nullptr;
	}
}

bool HashTable::setPointer (std::string const & key, void * pv) {
	int pos = this->getPos (key);
	if (pos >= 0) {
		(this->data.at (pos)).pv = pv;
		return true;
	} else return false;
}

// FNV-1a (64-bit)
size_t HashTable::keyHash (std::string const & key) {
	size_t hash = 14695981039346656037UL;
	char const * str = key.c_str ();
	unsigned int c;

	while (c = *str++) {	
		hash ^= c;
		hash *= 1099511628211;
	}

	return hash;
}

// FNV-1 (64-bit)
size_t HashTable::keyHash2 (std::string const & key) {
	size_t hash = 14695981039346656037UL;
	char const * str = key.c_str ();
	unsigned int c;

	while (c = *str++) {	
		hash *= 1099511628211;
		hash ^= c;
	}

	return hash;
}

bool HashTable::rehash () {
	int oldSize = this->capacity;
	this->capacity = getPrime (this->capacity);

	HashItem item;
	HashItem emptyItem;
	emptyItem.key = "";
	emptyItem.isOccupied = false;
	emptyItem.isDeleted = false;
	emptyItem.pv = nullptr;

	std::vector<HashItem> tempData;
	tempData.resize (this->capacity, emptyItem);

	// memory allocation fail-safe
	if (tempData.size () != this->capacity) return false;

	unsigned int pos, offset;
/*

	This can certainly be improved.  Do not need to duplicate code for any reason,
	whatsoever.  Simply first copy the original array to a temporary one, then
	resize the original, then use the already available insert () function.

*/
	int i, j;
	for (i = 0; i < oldSize; ++i) {
		item = this->data.at (i); // retrieve item from old table
		
		if (item.isOccupied && !item.isDeleted) {
			pos = this->keyHash (item.key) % this->capacity;
			offset = this->keyHash2 (item.key) % (this->capacity - 1) + 1;

			for (j = 0; j < oldSize; ++j) {
				pos = (pos + j * offset) % this->capacity;
				HashItem & tempItem = tempData.at (pos);

				// if no collision occurs, insert old item into new table
				if (!tempItem.isOccupied) {
					tempItem.key = item.key;
					tempItem.pv = item.pv;
					tempItem.isOccupied = true;
					break;
				}
			}
		}
	}

	// update rehashed table data
	this->data = tempData;

	return true;
}

int HashTable::getPos (std::string const & key) {
	unsigned int pos, offset;
	pos = this->keyHash (key) % this->capacity;
	offset = this->keyHash2 (key) % (this->capacity - 1) + 1;
	
	int i;
	HashItem item;
	
	// search for key
	for (i = 0; i < this->capacity; ++i) {
		pos = (pos + i * offset) % this->capacity;
		item = this->data.at (pos);

		// if keys match, return index
		if (item.isOccupied && !item.isDeleted && item.key == key)
			return pos;

		// key is not in table
		else if (!item.isOccupied)
			return -1;
	}
}

unsigned int HashTable::getPrime (int num) {
	static unsigned int prime[] = {
		53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
		24593, 49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189,
		805306457, 1610612741
	};

	int i = 0;
	while (prime[i] < 2 * num) ++i;

	return prime[i];
}
