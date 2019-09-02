#include "heap.hpp"

BinaryHeap::BinaryHeap (int capacity) {
	this->curSize = 0;
	this->capacity = capacity;
	this->data.resize (capacity + 1);
	this->map = new HashTable (capacity * 2);
}

int BinaryHeap::insert (std::string const & id, long long key, void * pv) {
	if (!(this->curSize < this->capacity)) return 1;
	else if (this->map->contains (id)) return 2;
	else {
		// create temporary BinaryHeap node to insert (with given data)
		HeapNode temp;
		temp.id = id;
		temp.key = key;
		temp.pv = pv;

		// insert node into BinaryHeap -> insert node into hash table -> percolate the new node up
		this->data[++this->curSize] = temp;
		this->map->insert (id, &(this->data[this->curSize]));
		this->percolateUp (this->curSize);

		return 0;
	}
}

int BinaryHeap::deleteMin (std::string * pId, long long * pKey, void * ppData) {
	if (!this->curSize) return 1;
	else {
		if (pId != nullptr) *pId = this->data[1].id;
		if (pKey != nullptr) *pKey = this->data[1].key;
		if (ppData != nullptr) *((void **) ppData) = this->data[1].pv;

		this->map->remove (this->data[1].id);         // lazily delete root node from hash table
		this->data[1] = this->data[this->curSize--];  // make last node in BinaryHeap go to root; dec. size
		this->percolateDown (1);                      // percolate down the root

		return 0;
	}
}

int BinaryHeap::setKey (std::string const & id, long long newKey) {
	if (!this->map->contains (id)) return 1;
	else {
		HeapNode * pn = (HeapNode *) this->map->getPointer (id);

		long long oldKey = pn->key;
		pn->key = newKey;

		int index = this->getPos (pn);
		if (oldKey < newKey) this->percolateDown (index);
		else if (oldKey > newKey) this->percolateUp (index);

		return 0;
	}
}

int BinaryHeap::remove (std::string const & id, long long * pKey, void * ppData) {
	if (!this->map->contains (id)) return 1;
	else {
		HeapNode * pn = (HeapNode *) this->map->getPointer (id);

		if (pKey != nullptr) *pKey = pn->key;
		if (ppData != nullptr) *((void **) ppData) = pn->pv;

		// lazily delete node from hash table
		this->map->remove (id);

		long long oldKey = pn->key;

		// replace old node with final node in BinaryHeap
		*pn = this->data[this->curSize--];

		long long newKey = pn->key;

		int index = this->getPos (pn);
		if (oldKey < newKey) this->percolateDown (index);
		else if (oldKey > newKey) this->percolateUp (index);

		return 0;
	}
}

int BinaryHeap::getPos (HeapNode * pn) {
	return pn - &this->data[0];
}

void BinaryHeap::percolateUp (int index) {
	HeapNode temp = this->data[index];
	
	while (index > 1 && temp.key < this->data[index / 2].key) {           // if we're aren't at the root and the parent is larger,
		this->data[index] = this->data[index / 2];                        // move parent to child
		this->map->setPointer (this->data[index].id, &this->data[index]); // update hash table pointer
		index /= 2;
	}

	// either we are at the root, or the parent is less than or equal to the node to be inserted;
	// in any case, this is the final destination of the node to be inserted
	this->data[index] = temp;
	this->map->setPointer (this->data[index].id, &this->data[index]);
}

void BinaryHeap::percolateDown (int index) {
	HeapNode temp = this->data[index];
	int child;

	while (index * 2 <= this->curSize) {
		child = index * 2;
		if (child < this->curSize && this->data[child + 1].key < this->data[child].key)
			++child; // look at the right child if there exists a right child and the left child is smaller

		if (this->data[child].key < temp.key) {                                      // if smaller child is smaller than the parent,
			this->data[index] = this->data[child];                                   // move smaller child to parent
			this->map->setPointer (this->data[index].id, &this->data[index]);        // update hash table pointer
		} else break;                                                                // node is in the right place
		index = child;
	}

	// either the smaller child is greater than or equal to the node to be inserted,
	// or we're at the end of the line and there are no more children
	// in any case, this is the final destination of the node to be inserted
	this->data[index] = temp;
	this->map->setPointer (this->data[index].id, &this->data[index]);
}
