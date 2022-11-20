#pragma once
#include "List.h"



template <typename T>

class LinkedList :
	public List<T>
{
private:

	struct Node {
		T* item;
		Node* next;

		Node(T* item) {
			this->item = item;
			this->next = nullptr;
		}
	};

	Node* head;
	Node* tail;

	class LinkedListIterator : public Iterator<T> {
		Node* current;

		virtual Iterator<T>* next() {
			current = current->next;
			return this;
		}

		virtual T* value() {
			return current->item;
		}

		virtual bool has_next() {
			return current != nullptr;
		}

	public:

		LinkedListIterator(LinkedList* list) {
			current = list->head;
		}
	};

	Node* find_node_n(Node* head, int n) {
		Node* slow = head;
		Node* fast = head ? head->next : nullptr;

		int i = 1;

		while (i < n && slow && fast) {
			slow = slow->next ? slow->next->next : nullptr;
			fast = fast->next ? fast->next->next : nullptr;
			i = i + 2;
		}

		if (n % 2 == 0) {
			return slow;
		}

		return fast;
	}

	Node* merge_sort(Node* head, int length, Comparator<T>* comparator) {
		if (length < 2) {
			return head;
		}

		int left_length = length / 2;
		int right_length = left_length + (length % 2);

		Node* left = head;
		Node* right = find_node_n(head, left_length);

		left = merge_sort(left, left_length, comparator);
		right = merge_sort(right, right_length, comparator);

		int i = 0;
		int j = 0;

		Node** iterator = &head;

		while (i < left_length && j < right_length) {
			if (comparator->compare(left->item, right->item) > 0) {
				*iterator = left;
				left = left->next;
				i++;
			}
			else {
				*iterator = right;
				right = right->next;
				j++;
			}

			iterator = &((*iterator)->next);
		}

		while (i < left_length) {
			*iterator = left;
			left = left->next;
			this->tail = *iterator;
			iterator = &((*iterator)->next);
			i++;
		}

		while (j < right_length) {
			*iterator = right;
			right = right->next;
			this->tail = *iterator;
			iterator = &((*iterator)->next);
			j++;
		}

		*iterator = nullptr;

		return head;
	}

	Node* binary_search(Node* head, Node* middle, int size, std::function<int(T*)> compare) {
		if (!size && compare(head->item) != 0) {
			return nullptr;
		}

		int comparison = compare(middle->item);

		if (comparison == 0) {
			return middle;
		}

		if (comparison > 0) {
			return binary_search(middle, find_node_n(middle, size / 2), size / 2, compare);
		}

		return binary_search(head, find_node_n(head, size / 2), size / 2, compare);
	}

public:

	virtual int get_size() {
		int size = 0;

		Node* iterator = this->head;

		while (iterator != nullptr) {
			size++;
			iterator = iterator->next;
		}

		return size;
	}

	virtual bool is_empty() {
		return this->head == nullptr;
	}

	virtual void add(T* value) {
		Node* node = new Node(value);

		if (this->is_empty()) {
			this->head = this->tail = node;
			return;
		}

		this->tail->next = node;
		this->tail = this->tail->next;
	}

	virtual bool insert(int index, T* value) {
		if (index < 0) {
			return false;
		}

		Node** iterator = &this->head;
		int i = 0;

		while (i < index && *iterator != nullptr) {
			iterator = &((*iterator)->next);
			i++;
		}

		if (*iterator == nullptr) {
			return false;
		}

		Node* node = new Node(value);

		node->next = *iterator;
		*iterator = node;

		return true;
	}

	virtual T* get(int index) {
		if (index < 0) {
			return nullptr;
		}

		Node* iterator = this->head;

		int i = 0;
		while (i < index) {
			iterator = iterator->next;
			i++;
		}

		if (iterator == nullptr) {
			return nullptr;
		}

		return iterator->item;
	}

	virtual T* remove(int index) {
		if (index < 0) {
			return nullptr;
		}

		Node** iterator = &this->head;
		Node* previous = nullptr;
		int i = 0;

		while (i < index && (*iterator)->next != nullptr) {
			previous = *iterator;
			iterator = &((*iterator)->next);
			i++;
		}

		if ((*iterator)->next == nullptr) {
			this->tail = previous;
		}

		T* item = (*iterator)->item;

		*iterator = (*iterator)->next;

		return item;
	}

	virtual Iterator<T>* iterator() {
		return new LinkedListIterator(this);
	}

	virtual void for_each(void (*action)(T*)) {
		Node* iterator = this->head;

		while (iterator != nullptr) {
			action(iterator->item);
			iterator = iterator->next;
		}
	}

	virtual List<T>* bubble_sort(int (*comparator)(T*, T*)) {
		Node** i = &(this->head);

		while (*i != nullptr) {

			Node** j = &(*i)->next;

			while (*j != nullptr) {
				T** itemA = &(*i)->item;
				T** itemB = &(*j)->item;

				if (comparator(*itemA, *itemB) > 0) {
					T* aux = *itemA;
					*itemA = *itemB;
					*itemB = aux;
				}

				j = &(*j)->next;
			}

			i = &(*i)->next;
		}

		return this;
	}

	virtual List<T>* merge_sort(Comparator<T>* comparator) {
		this->head = merge_sort(this->head, this->get_size(), comparator);
		return this;
	}

	virtual List<T>* filter(std::function<bool(T*)> predicate) {
		List<T>* results = new LinkedList();

		Node* iterator = this->head;

		while (iterator) {
			if (predicate(iterator->item)) {
				results->add(iterator->item);
			}

			iterator = iterator->next;
		}

		return results;
	}

	virtual T* binary_search(std::function<int(T*)> compare) {
		int size = this->get_size();
		Node* result = binary_search(this->head, find_node_n(head, size / 2), size, compare);
		return result ? result->item : nullptr;
	}
};
