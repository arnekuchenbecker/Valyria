#pragma once

#include <unordered_map>

#include "PriorityQueue.h"
#include "Diagram.h"
#include "Beachline.h"
#include "Event.h"
#include "Util.h"

template<typename T>
class FortuneAlgorithm {
public:
	explicit FortuneAlgorithm(std::vector<Vector2<T>> points) : diagram(std::move(points)) {

	}

	void construct() {
		for (auto i = std::size_t(0); i < diagram.getNumberOfSites(); i++) {
			events.push(std::make_unique<Event<T>>(diagram.getSite(i)));
		}

		while (!events.isEmpty()) {
			auto event = events.pop();
			beachlineY = event->y;
			if (event->type == Event<T>::Type::Site) {
				handleSiteEvent(event.get());
			}
			else {
				handleCircleEvent(event.get());
			}
		}
	}

	bool bound(Box<T> box) {
		auto success = true;
		for (const auto& vertex : diagram.getVertices()) {
			box.left = std::min(vertex.point.x, box.left);
			box.bottom = std::min(vertex.point.y, box.bottom);
			box.right = std::max(vertex.point.x, box.right);
			box.top = std::max(vertex.point.y, box.top);
		}
		auto linkedVertices = std::list<LinkedVertex>();
		auto vertices = VerticesOnFrontierContainer(diagram.getNumberOfSites());
		if (!beachline.isEmpty()) {
			auto arc = beachline.getLeftmostArc();
			while (!beachline.isNil(arc->next)) {
				success = boundEdge(box, arc, arc->next, linkedVertices, vertices) && success;
				arc = arc->next;
			}
		}

		for (auto& kv : vertices) {
			success = addCorners(box, linkedVertices, kv.second) && success;
		}

		for (auto& kv : vertices) {
			joinHalfEdges(kv.first, kv.second);
		}

		return success;
	}

	Diagram<T> getDiagram() {
		return std::move(diagram);
	}

private:
	Diagram<T> diagram;
	Beachline<T> beachline;
	PriorityQueue<Event<T>> events;
	T beachlineY;

	void handleSiteEvent(Event<T>* event) {
		auto site = event->site;

		if (beachline.isEmpty()) {
			beachline.setRoot(beachline.createArc(site));
			return;
		}

		auto arcToBreak = beachline.locateArcAbove(site->point, beachlineY);
		deleteEvent(arcToBreak);

		auto middleArc = breakArc(arcToBreak, site);
		auto leftArc = middleArc->prev;
		auto rightArc = middleArc->next;

		addEdge(leftArc, middleArc);
		middleArc->rightHalfEdge = middleArc->leftHalfEdge;
		rightArc->leftHalfEdge = leftArc->rightHalfEdge;

		if (!beachline.isNil(leftArc->prev)) {
			addEvent(leftArc->prev, leftArc, middleArc);
		}

		if (!beachline.isNil(rightArc->next)) {
			addEvent(middleArc, rightArc, rightArc->next);
		}
	}

	void handleCircleEvent(Event<T>* event) {
		auto point = event->point;
		auto arc = event->arc;

		auto vertex = diagram.createVertex(point);

		auto leftArc = arc->prev;
		auto rightArc = arc->next;
		deleteEvent(leftArc);
		deleteEvent(rightArc);

		removeArc(arc, vertex);

		if (!beachline.isNil(leftArc->prev)) {
			addEvent(leftArc->prev, leftArc, rightArc);
		}
		if (!beachline.isNil(rightArc->next)) {
			addEvent(leftArc, rightArc, rightArc->next);
		}
	}

	Arc<T>* breakArc(Arc<T>* arc, typename Diagram<T>::Site* site) {
		auto middleArc = beachline.createArc(site);
		auto leftArc = beachline.createArc(arc->site, Arc<T>::Side::Left);
		leftArc->leftHalfEdge = arc->leftHalfEdge;
		auto rightArc = beachline.createArc(arc->site, Arc<T>::Side::Right);
		rightArc->rightHalfEdge = arc->rightHalfEdge;

		beachline.replace(arc, middleArc);
		beachline.insertBefore(middleArc, leftArc);
		beachline.insertAfter(middleArc, rightArc);

		delete arc;

		return middleArc;
	}

	void removeArc(Arc<T>* arc, typename Diagram<T>::Vertex* vertex) {
		setDestination(arc->prev, arc, vertex);
		setDestination(arc, arc->next, vertex);

		arc->leftHalfEdge->next = arc->rightHalfEdge;
		arc->rightHalfEdge->prev = arc->leftHalfEdge;

		beachline.remove(arc);

		auto prevHalfEdge = arc->prev->rightHalfEdge;
		auto nextHalfEdge = arc->next->leftHalfEdge;
		addEdge(arc->prev, arc->next);
		setOrigin(arc->prev, arc->next, vertex);
		setPrevHalfEdge(arc->prev->rightHalfEdge, prevHalfEdge);
		setPrevHalfEdge(nextHalfEdge, arc->next->leftHalfEdge);

		delete arc;
	}

	bool isMovingRight(const Arc<T>* left, const Arc<T>* right) const {
		return left->site->point.y < right->site->point.y;
	}

	T getInitialX(const Arc<T>* left, const Arc<T>* right, bool movingRight) const {
		return movingRight ? left->site->point.x : right->site->point.x;
	}

	void addEdge(Arc<T>* left, Arc<T>* right) {
		left->rightHalfEdge = diagram.createHalfEdge(left->site->face);
		right->leftHalfEdge = diagram.createHalfEdge(right->site->face);

		left->rightHalfEdge->twin = right->leftHalfEdge;
		right->leftHalfEdge->twin = left->rightHalfEdge;
	}

	void setOrigin(Arc<T>* left, Arc<T>* right, typename Diagram<T>::Vertex* vertex) {
		left->rightHalfEdge->destination = vertex;
		right->leftHalfEdge->origin = vertex;
	}

	void setDestination(Arc<T>* left, Arc<T>* right, typename Diagram<T>::Vertex* vertex) {
		left->rightHalfEdge->origin = vertex;
		right->leftHalfEdge->destination = vertex;
	}

	void setPrevHalfEdge(typename Diagram<T>::HalfEdge* prev, typename Diagram<T>::HalfEdge* next) {
		prev->next = next;
		next->prev = prev;
	}

	void addEvent(Arc<T>* left, Arc<T>* middle, Arc<T>* right) {
		auto y = T();
		auto convergencePoint = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);

		if (!almostLower(y, beachlineY)) {
			return;
		}

		auto leftBreakpointMovingRight = isMovingRight(left, middle);
		auto rightBreakpointMovingRight = isMovingRight(middle, right);

		auto leftInitialX = getInitialX(left, middle, leftBreakpointMovingRight);
		auto rightInitialX = getInitialX(middle, right, rightBreakpointMovingRight);

		if (!(!leftBreakpointMovingRight && rightBreakpointMovingRight) &&
			((leftBreakpointMovingRight && almostLower(leftInitialX, convergencePoint.x))
				|| (!leftBreakpointMovingRight && almostGreater(leftInitialX, convergencePoint.x))) &&
			((rightBreakpointMovingRight && almostLower(rightInitialX, convergencePoint.x))
				|| (!rightBreakpointMovingRight && almostGreater(rightInitialX, convergencePoint.x)))) {
			auto event = std::make_unique<Event<T>>(y, convergencePoint, middle);
			middle->event = event.get();
			events.push(std::move(event));
		}
	}

	void deleteEvent(Arc<T>* arc) {
		if (arc->event != nullptr) {
			events.remove(arc->event->index);
			arc->event = nullptr;
		}
	}

	Vector2<T> computeConvergencePoint(const Vector2<T>& point1, const Vector2<T>& point2, const Vector2<T>& point3, T& y) const {
		auto v1 = (point1 - point2).getOrthogonal();
		auto v2 = (point2 - point3).getOrthogonal();

		auto delta = static_cast<T>(0.5) * (point3 - point1);
		auto denom = v1.getDet(v2);

		if (almostZero(denom)) {
			y = std::numeric_limits<T>::infinity();
			return Vector2<T>();
		}

		auto t = delta.getDet(v2) / denom;
		auto center = static_cast<T>(0.5) * (point1 + point2) + t * v1;
		auto r = center.getDistance(point1);
		y = center.y - r;
		return center;
	}

	struct LinkedVertex {
		typename Diagram<T>::HalfEdge* prevHalfEdge;
		typename Diagram<T>::Vertex* vertex;
		typename Diagram<T>::HalfEdge* nextHalfEdge;
	};

	using VerticesOnFrontierContainer = std::unordered_map<std::size_t, std::array<LinkedVertex*, 8>>;

	bool boundEdge(const Box<T>& box, Arc<T>* leftArc, Arc<T>* rightArc, std::list<LinkedVertex>& linkedVertices, VerticesOnFrontierContainer& vertices) {
		auto success = true;

		auto direction = (leftArc->site->point - rightArc->site->point).getOrthogonal();
		auto origin = (leftArc->site->point + rightArc->site->point) * static_cast<T>(0.5);

		auto intersection = box.getFirstIntersection(origin, direction);

		auto vertex = diagram.createVertex(intersection.point);
		setDestination(leftArc, rightArc, vertex);

		if (vertices.find(leftArc->site->index) == vertices.end()) {
			vertices[leftArc->site->index].fill(nullptr);
		}
		if (vertices.find(rightArc->site->index) == vertices.end()) {
			vertices[rightArc->site->index].fill(nullptr);
		}

		success = vertices[leftArc->site->index][2 * static_cast<int>(intersection.side) + 1] == nullptr && success;
		success = vertices[rightArc->site->index][2 * static_cast<int>(intersection.side)] == nullptr && success;

		linkedVertices.emplace_back(LinkedVertex{ nullptr, vertex, leftArc->rightHalfEdge });
		vertices[leftArc->site->index][2 * static_cast<int>(intersection.side) + 1] = &linkedVertices.back();

		linkedVertices.emplace_back(LinkedVertex{ rightArc->leftHalfEdge, vertex, nullptr });
		vertices[rightArc->site->index][2 * static_cast<int>(intersection.side)] = &linkedVertices.back();

		return success;
	};

	bool addCorners(const Box<T> box, std::list<LinkedVertex>& linkedVertices, std::array<LinkedVertex*, 8>& cellVertices) {
		auto success = true;
		for (auto i = std::size_t(0); i < 5; i++) {
			auto side = i % 4;
			auto nextSide = (side + 1) % 4;

			if (cellVertices[2 * side] == nullptr && cellVertices[2 * side + 1] != nullptr) {
				auto prevSide = (side + 3) % 4;
				auto corner = diagram.createCorner(box, static_cast<typename Box<T>::Side>(side));
				linkedVertices.emplace_back(LinkedVertex{ nullptr, corner, nullptr });

				success = cellVertices[2 * prevSide + 1] == nullptr && success;

				cellVertices[2 * prevSide + 1] = &linkedVertices.back();
				cellVertices[2 * side] = &linkedVertices.back();
			}
			else if (cellVertices[2 * side] != nullptr && cellVertices[2 * side + 1] == nullptr) {
				auto corner = diagram.createCorner(box, static_cast<typename Box<T>::Side>(nextSide));
				linkedVertices.emplace_back(LinkedVertex{ nullptr, corner, nullptr });

				success = cellVertices[2 * nextSide] == nullptr && success;

				cellVertices[2 * side + 1] = &linkedVertices.back();
				cellVertices[2 * nextSide] = &linkedVertices.back();
			}
		}

		return success;
	}

	void joinHalfEdges(std::size_t i, std::array<LinkedVertex*, 8>& cellVertices) {
		for (auto side = std::size_t(0); side < 4; side++) {
			if (cellVertices[2 * side] != nullptr) {
				auto halfEdge = diagram.createHalfEdge(diagram.getFace(i));
				halfEdge->origin = cellVertices[2 * side]->vertex;
				halfEdge->destination = cellVertices[2 * side + 1]->vertex;

				cellVertices[2 * side]->nextHalfEdge = halfEdge;
				halfEdge->prev = cellVertices[2 * side]->prevHalfEdge;
				if (cellVertices[2 * side]->prevHalfEdge != nullptr) {
					cellVertices[2 * side]->prevHalfEdge->next = halfEdge;
				}

				cellVertices[2 * side + 1]->prevHalfEdge = halfEdge;
				halfEdge->next = cellVertices[2 * side + 1]->nextHalfEdge;
				if (cellVertices[2 * side + 1]->nextHalfEdge != nullptr) {
					cellVertices[2 * side + 1]->nextHalfEdge->prev = halfEdge;
				}
			}
		}
	}
};