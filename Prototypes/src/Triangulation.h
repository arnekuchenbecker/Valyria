#pragma once

#include <vector>

class Triangulation {
public:
	explicit Triangulation(std::vector<std::vector<std::size_t>> neighbors) : neighbors(neighbors) {

	}

	std::size_t getNumberOfVertices() const {
		return neighbors.size();
	}

	const std::vector<std::size_t>& getNeighbors(std::size_t i) const {
		return neighbors[i];
	}

private:
	std::vector<std::vector<std::size_t>> neighbors;
};