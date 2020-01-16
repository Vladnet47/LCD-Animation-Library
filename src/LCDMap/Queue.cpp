// Title: Queue
// Created by: Vlad Netrebchenko
// Start Date: July 3, 2019
// Last Modification: July 5, 2019

// Description:
// Queue makes sure that user retrieves objects in
// the same order as they were inserted. Queue also
// allows user to retrieve object using it's id.

#include "Queue.h"
#include "Frame.h"
#include "LCDMap.h"

template <class T>
Queue<T>::Queue() {
	head = nullptr;
	tail = nullptr;
	length = 0;
}

template <class T>
Queue<T>::~Queue() {
	clear();
}

// returns true if object with given id exists, false otherwise
template <class T>
bool Queue<T>::contains(char id) const {
	return getNode(id) != nullptr;
}

// returns pointer to next object, or nullptr if it doesn't exist
template <class T>
T* Queue<T>::get() const {
	return (head == nullptr) ? nullptr : head->data;
}

// returns pointer to object with given id, or nullptr if it doesn't exist
template <class T>
T* Queue<T>::get(char id) const {
	QueueNode<T>* node = getNode(id);
	return (node == nullptr) ? nullptr : node->data;
}

// returns id of next object, or '~' if it doesn't exist
template <class T>
char Queue<T>::id() const {
	return (head == nullptr) ? ERROR : head->id;
}

// returns number of objects
template <class T>
int Queue<T>::size() const {
	return length;
}

// rotates front object to the back
template <class T>
void Queue<T>::rotate() {
	if (length > 1) {
		QueueNode<T>* node = head;
		head = head->next;
		tail->next = node;
		tail = node;
	}
}

// adds new object with given value and auto-generated id 
template <class T>
bool Queue<T>::add(T* value) {
	char id = 1;
	while (contains(id)) {
		id++;
	}

	return add(id, value);
}

// adds new object with given value and id
template <class T>
bool Queue<T>::add(char id, T* value) {
	if (contains(id) || value == nullptr) return false;

	QueueNode<T>* node;

	if (length == 0) {
		node = new QueueNode<T>{id, value, node, node};
		head = node;
	} else {
		node = new QueueNode<T>{id, value, head, tail};
		tail->next = node;	
	}

	tail = node;
	length++;

	return true;
}

// removes object with given id
template <class T>
bool Queue<T>::remove(char id) {
	if (length == 0) return false;

	// if only one object, set head and tail to null
	if (head->id == id && tail->id == id) {
		delete head->data;
		delete head;
		head = nullptr;
		tail = nullptr;
		length--;
		return true;
	}

	// if more than one object and id is in front
	if (head->id == id) {
		QueueNode<T>* temp = head->next;
		delete head->data;
		delete head;
		head = temp;
		head->prev = tail;
		length--;
		return true;
	}

	// if more than one object and id is in back
	if (tail->id == id) {
		QueueNode<T>* temp = tail->prev;
		delete tail->data;
		delete tail;
		tail = temp;
		tail->next = head;
		length--;
		return true;
	}

	// if more than one object and id is in the middle
	QueueNode<T>* node = getNode(id);

	if (node == nullptr) return false;

	node->next->prev = node->prev;
	node->prev->next = node->next;
	delete node->data;
	delete node;
	length--;
	return true;
}

// removes all objects
template <class T>
void Queue<T>::clear() {
	QueueNode<T>* node = head;
	QueueNode<T>* temp;

	while (length > 0) {
		temp = node->next;
		delete node->data;
		delete node;
		node = temp;
		length--;
	}

	head = nullptr;
	tail = nullptr;
}

// returns pointer to node with given id, or nullptr if it doesn't exist
template <class T>
QueueNode<T>* Queue<T>::getNode(char id) const {
	QueueNode<T>* node = head;

	for (int i = 0; i < length; ++i) {
		if (node->id == id) return node;
		node = node->next;
	}

	return nullptr;
}

template class Queue<Frame>;
template class Queue<Sprite>;
