#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <string>

class HashTable {
	public:
		HashTable (int size = 0);

		int insert (std::string const & key, void * pv = nullptr);
		bool remove (std::string const & key);

		bool contains (std::string const & key);

		void * getPointer (std::string const & key, bool * b = nullptr);
		bool setPointer (std::string const & key, void * pv);
	private:
		typedef struct {
			std::string key;
			bool isOccupied;
			bool isDeleted;
			void * pv;
		} HashItem;

		std::vector<HashItem> data;

		int capacity;
		int numOccupied;

		int getPos (std::string const & key);

		size_t keyHash (std::string const & key);
		size_t keyHash2 (std::string const & key);

		bool rehash ();
		
		static unsigned int getPrime (int num);
};

#endif
