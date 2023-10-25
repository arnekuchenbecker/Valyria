#pragma once

#include <vector>
#include <list>
#include <unordered_set>

#include "Box.h"
#include "Triangulation.h"

template<typename T>
class FortuneAlgorithm;

template<typename T>
class Diagram {
public:
	struct HalfEdge;
	struct Face;

	struct Site {
		std::size_t index;
		Vector2<T> point;
		Face* face;
	};

	struct Vertex {
		Vector2<T> point;

	private:
		friend Diagram<T>;
		typename std::list<Vertex>::iterator it;
	};

	struct HalfEdge {
		Vertex* origin = nullptr;
		Vertex* destination = nullptr;
		HalfEdge* twin = nullptr;
		Face* incidentFace;
		HalfEdge* prev = nullptr;
		HalfEdge* next = nullptr;

	private:
		friend Diagram;
		typename std::list<HalfEdge>::iterator it;
	};

	struct Face {
		Site* site;
		HalfEdge* outerComponent;
	};

	Diagram(const Diagram&) = delete;
	Diagram& operator=(const Diagram&) = delete;

	Diagram(Diagram&&) = default;
	Diagram& operator=(Diagram&&) = default;

	const std::vector<Site>& getSites() const {
		return sites;
	}

	const Site* getSite(std::size_t i) const {
		return &sites[i];
	}

	std::size_t getNumberOfSites() const {
		return sites.size();
	}

	const std::vector<Face>& getFaces() const {
		return faces;
	}

	const Face* getFace(std::size_t i) const {
		return &faces[i];
	}

	const std::list<Vertex>& getVertices() const {
		return vertices;
	}

	const std::list<HalfEdge>& getHalfEdges() const {
		return halfEdges;
	}

	bool intersect(Box<T> box) {
		auto success = true;
		auto processedHalfEdges = std::unordered_set<HalfEdge*>();
		auto verticesToRemove = std::unordered_set<Vertex*>();

		for (const auto& site : sites) {
			auto halfEdge = site.face->outerComponent;
			auto inside = box.contains(halfEdge->origin->point);
			auto outerComponentDirty = !inside;
			auto incomingHalfEdge = static_cast<HalfEdge*>(nullptr);
			auto outgoingHalfEdge = static_cast<HalfEdge*>(nullptr);
			auto incomingSide = typename Box<T>::Side{};
			auto outgoingSide = typename Box<T>::Side{};

			do {
				auto intersections = std::array<typename Box<T>::Intersection, 2>{};
				auto numberOfIntersections = box.getIntersections(halfEdge->origin->point, halfEdge->destination->point, intersections);
				auto nextInside = box.contains(halfEdge->destination->point);
				auto nextHalfEdge = halfEdge->next;

				if (!inside && !nextInside) {
					if (numberOfIntersections == 0) {
						verticesToRemove.emplace(halfEdge->origin);
						removeHalfEdge(halfEdge);
					}
					else if (numberOfIntersections == 2) {
						verticesToRemove.emplace(halfEdge->origin);
						if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end()) {
							halfEdge->origin = halfEdge->twin->destination;
							halfEdge->destination = halfEdge->twin->origin;
						}
						else {
							halfEdge->origin = createVertex(intersections[0].point);
							halfEdge->destination = createVertex(intersections[1].point);
						}

						if (outgoingHalfEdge != nullptr) {
							link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
						}

						if (incomingHalfEdge == nullptr) {
							incomingHalfEdge = halfEdge;
							incomingSide = intersections[0].side;
						}

						outgoingHalfEdge = halfEdge;
						outgoingSide = intersections[1].side;
						processedHalfEdges.emplace(halfEdge);
					}
					else {
						success = false;
					}
				}
				else if (inside && !nextInside) {
					if (numberOfIntersections >= 1) {
						if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end()) {
							halfEdge->destination = halfEdge->twin->origin;
						}
						else {
							halfEdge->destination = createVertex(intersections[0].point);
						}

						outgoingHalfEdge = halfEdge;
						outgoingSide = intersections[0].side;
						processedHalfEdges.emplace(halfEdge);
					}
					else {
						success = false;
					}
				}
				else if (!inside && nextInside) {
					if (numberOfIntersections >= 1) {
						verticesToRemove.emplace(halfEdge->origin);
						if (processedHalfEdges.find(halfEdge->twin) != processedHalfEdges.end()) {
							halfEdge->origin = halfEdge->twin->destination;
						}
						else {
							halfEdge->origin = createVertex(intersections[0].point);
						}

						if (outgoingHalfEdge != nullptr) {
							link(box, outgoingHalfEdge, outgoingSide, halfEdge, intersections[0].side);
						}

						if (incomingHalfEdge == nullptr) {
							incomingHalfEdge = halfEdge;
							incomingSide = intersections[0].side;
						}

						processedHalfEdges.emplace(halfEdge);
					}
					else {
						success = false;
					}
				}

				halfEdge = nextHalfEdge;
				inside = nextInside;
			} while (halfEdge != site.face->outerComponent);

			if (outerComponentDirty && incomingHalfEdge != nullptr) {
				link(box, outgoingHalfEdge, outgoingSide, incomingHalfEdge, incomingSide);
			}

			if (outerComponentDirty) {
				site.face->outerComponent = incomingHalfEdge;
			}
		}

		for (auto& vertex : verticesToRemove) {
			removeVertex(vertex);
		}

		return success;
	}

	std::vector<Vector2<T>> computeLloydRelaxation() const {
		auto sites = std::vector<Vector2<T>>();
		for (const auto& face : faces) {
			auto area = static_cast<T>(0.0);
			auto centroid = Vector2<T>();
			auto halfEdge = face.outerComponent;

			do {
				auto det = halfEdge->origin->point.getDet(halfEdge->destination->point);
				area += det;
				centroid += (halfEdge->origin->point + halfEdge->destination->point) * det;
				halfEdge = halfEdge->next;
			} while (halfEdge != face.outerComponent);
			area *= 0.5;
			centroid *= 1.0 / (6.0 * area);
			sites.push_back(centroid);
		}

		return sites;
	}

	Triangulation computeTriangulation() const {
		auto neighbors = std::vector<std::vector<std::size_t>>(sites.size());
		for (auto i = std::size_t(0); i < sites.size(); i++) {
			auto face = faces[i];
			auto halfEdge = face.outerComponent;
			while (halfEdge->prev != nullptr) {
				halfEdge = halfEdge->prev;
				if (halfEdge == face.outerComponent) {
					break;
				}
			}

			while (halfEdge != nullptr) {
				if (halfEdge->twin != nullptr) {
					neighbors[i].push_back(halfEdge->twin->incidentFace->site->index);
				}

				halfEdge = halfEdge->next;
				if (halfEdge == face.outerComponent) {
					break;
				}
			}
		}

		return Triangulation(neighbors);
	}

private:
	std::vector<Site> sites;
	std::vector<Face> faces;
	std::list<Vertex> vertices;
	std::list<HalfEdge> halfEdges;

	template<typename T>
	friend class FortuneAlgorithm;

	Diagram(const std::vector<Vector2<T>>& points) {
		sites.reserve(points.size());
		faces.reserve(points.size());

		for (auto i = std::size_t(0); i < points.size(); i++) {
			sites.push_back(Diagram::Site{ i, points[i], nullptr });
			faces.push_back(Diagram::Face{ &sites.back(), nullptr });
			sites.back().face = &faces.back();
		}
	}

	Site* getSite(std::size_t i) {
		return &sites[i];
	}

	Face* getFace(std::size_t i) {
		return &faces[i];
	}

	Vertex* createVertex(Vector2<T> point) {
		vertices.emplace_back();
		vertices.back().point = point;
		vertices.back().it = std::prev(vertices.end());
		return &vertices.back();
	}

	Vertex* createCorner(Box<T> box, typename Box<T>::Side side) {
		switch (side) {
		case Box<T>::Side::Left:
			return createVertex(Vector2<T>(box.left, box.top));
		case Box<T>::Side::Bottom:
			return createVertex(Vector2<T>(box.left, box.bottom));
		case Box<T>::Side::Right:
			return createVertex(Vector2<T>(box.right, box.bottom));
		case Box<T>::Side::Top:
			return createVertex(Vector2<T>(box.right, box.top));
		default:
			return nullptr;
		}
	}

	HalfEdge* createHalfEdge(Face* face) {
		halfEdges.emplace_back();
		halfEdges.back().incidentFace = face;
		halfEdges.back().it = std::prev(halfEdges.end());
		if (face->outerComponent == nullptr) {
			face->outerComponent = &halfEdges.back();
		}
		return &halfEdges.back();
	}

	void link(Box<T> box, HalfEdge* start, typename Box<T>::Side startSide, HalfEdge* end, typename Box<T>::Side endSide) {
		auto halfEdge = start;
		auto side = static_cast<int>(startSide);
		while (side != static_cast<int>(endSide)) {
			side = (side + 1) % 4;
			halfEdge->next = createHalfEdge(start->incidentFace);
			halfEdge->next->prev = halfEdge;
			halfEdge->next->origin = halfEdge->destination;
			halfEdge->next->destination = createCorner(box, static_cast<typename Box<T>::Side>(side));
			halfEdge = halfEdge->next;
		}

		halfEdge->next = createHalfEdge(start->incidentFace);
		halfEdge->next->prev = halfEdge;
		halfEdge->next->next = end;
		halfEdge->next->origin = halfEdge->destination;
		halfEdge->next->destination = end->origin;
	}

	void removeVertex(Vertex* vertex) {
		vertices.erase(vertex->it);
	}

	void removeHalfEdge(HalfEdge* halfEdge) {
		halfEdges.erase(halfEdge->it);
	}
};