/*
 * Defina la clase Nodo que implemente un nodo de una lista doblemente 
 * enlazada, albergando un dato de tipo genérico T. Incluya al menos, 
 * constructor default, constructor de copia y los operadores <<, = e ==.
 */

#include <iostream>

template < typename T >
class Node {
	private:
		T data;
		Node<T>* previous, next;
	public:
		Node();
		Node(const Node<T>& copy_from_me);
		friend std::ostream& operator<<(std::ostream& o, const Node<T>& n);
		Node<T>& operator=(const Node<T>& other);
		bool operator==(const Node<T>& other);
		~Node();
};

template < typename T >
Node<T>::Node() : previous(NULL), next(NULL) {}

template < typename T >
Node<T>::Node(const Node<T>& copy_from_me) : data(copy_from_me.data), 
											 next(copy_from_me.next), 
											 previous(copy_from_me.previous) {}

template < typename T >
std::ostream& operator<<(std::ostream& o, const Node<T>& n) {
	o << "Node data: " << n.data;
	return o;
}

template < typename T >
Node<T>& Node<T>::operator=(const Node<T>& other) {
	// Check for self-assignment
	if (this != &other) { 
		// Clean up the old contents of the object
		delete this->data;
		// Copy the resources of the other object
		this->data = other.data;
		this->previous = other.previous;
		this->next = other.next;
	}
	return *this;
}

template < typename T >
bool Node<T>::operator==(const Node<T>& other) {
	return this->data == other.data;
}

Node<T>::~Node() {
	delete this->data;
}
