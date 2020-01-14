#ifndef QUEUE_H
#define QUEUE_H

using namespace std;

template <class T>
struct QueueNode {
	char id;
	T* data;
	QueueNode<T>* next;
	QueueNode<T>* prev;
};

template <class T>
class Queue {
public:
	Queue();
	~Queue();

	bool contains(char id) const;
	T* get() const;
	T* get(char id) const;
	char id() const;
	int size() const;
	void rotate();
	bool add(T* value);
	bool add(char id, T* value);
	bool remove(char id);
	void clear();

private:
	char ERROR = '~';
	QueueNode<T>* head;
	QueueNode<T>* tail;
	int length;

	QueueNode<T>* getNode(char id) const;
};

#endif
