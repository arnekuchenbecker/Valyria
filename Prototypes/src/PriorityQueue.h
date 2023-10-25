#pragma once

#include <vector>
#include <memory>

template<typename T>
class PriorityQueue {
public:
	PriorityQueue() {

	}

	bool isEmpty() const {
		return elements.empty();
	}

	std::unique_ptr<T> pop() {
		swap(0, elements.size() - 1);
		auto top = std::move(elements.back());
		elements.pop_back();
		siftDown(0);
		return top;
	}

	void push(std::unique_ptr<T>&& elem) {
		elem->index = elements.size();
		elements.emplace_back(std::move(elem));
		siftUp(elements.size() - 1);
	}

	void update(std::size_t i) {
		auto parent = getParent(i);
		if (parent >= 0 && *elements[parent] < *elements[i]) {
			siftUp(i);
		}
		else {
			siftDown(i);
		}
	}

	void remove(std::size_t i) {
		swap(i, elements.size() - 1);
		elements.pop_back();
		if (i < elements.size()) {
			update(i);
		}
	}

private:
	std::vector<std::unique_ptr<T>> elements;

	int getParent(int i) const {
		return (i + 1) / 2 - 1;
	}

	std::size_t getLeftChild(std::size_t i) const {
		return 2 * (i + 1) - 1;
	}

	std::size_t getRighthChild(std::size_t i) const {
		return 2 * (i + 1);
	}

	void siftDown(std::size_t i) {
		auto left = getLeftChild(i);
		auto right = getRighthChild(i);
		auto j = i;
		if (left < elements.size() && *elements[j] < *elements[left]) {
			j = left;
		}
		if (right < elements.size() && *elements[j] < *elements[right]) {
			j = right;
		}
		if (j != i) {
			swap(i, j);
			siftDown(j);
		}
	}

	void siftUp(std::size_t i) {
		auto parent = getParent(i);
		if (parent >= 0 && *elements[parent] < *elements[i]) {
			swap(i, parent);
			siftUp(parent);
		}
	}

	inline void swap(std::size_t i, std::size_t j) {
		std::swap(elements[i], elements[j]);
		elements[i]->index = i;
		elements[j]->index = j;
	}
};
