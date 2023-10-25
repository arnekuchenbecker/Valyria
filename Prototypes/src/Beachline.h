#pragma once

#include "Vector2.h"
#include "Diagram.h"
#include "Arc.h"
#include "Util.h"

template<typename T>
class Beachline {
public:
	Beachline() : nil(new Arc<T>), root(nil) {
		nil->color = Arc<T>::Color::Black;
	}

	~Beachline() {
		free(root);
		delete nil;
	}

	Beachline(const Beachline&) = delete;
	Beachline& operator=(const Beachline&) = delete;

	Beachline(Beachline&& other) {
		nil = other.nil;
		root = other.root;
		other.nil = nullptr;
		other.root = nullptr;
	}

	Beachline& operator=(Beachline&& other) {
		free(root);
		delete nil;
		nil = other.nil;
		root = other.root;
		other.nil = nullptr;
		other.root = nullptr;
	}

	Arc<T>* createArc(typename Diagram<T>::Site* site, typename Arc<T>::Side side = Arc<T>::Side::Left) {
		return new Arc<T>{ nil, nil, nil, site, nullptr, nullptr, nullptr, nil, nil, Arc<T>::Color::Red, side };
	}

	bool isEmpty() const {
		return isNil(root);
	}

	bool isNil(const Arc<T>* x) const {
		return x == nil;
	}

	void setRoot(Arc<T>* x) {
		root = x;
		root->color = Arc<T>::Color::Black;
	}

	Arc<T>* getLeftmostArc() const {
		auto x = root;
		while (!isNil(x->prev)) {
			x = x->prev;
		}
		return x;
	}

	Arc<T>* locateArcAbove(const Vector2<T>& point, T l) const {
		auto node = root;
		auto found = false;
		while (!found) {
			auto breakpointLeft = -std::numeric_limits<T>::infinity();
			auto breakpointRight = std::numeric_limits<T>::infinity();
			if (!isNil(node->prev)) {
				breakpointLeft = computeBreakpoint(node->prev->site->point, node->site->point, l, node->prev->side);
			}
			if (!isNil(node->next)) {
				breakpointRight = computeBreakpoint(node->site->point, node->next->site->point, l, node->next->side);
			}

			if (point.x < breakpointLeft) {
				node = node->left;
			}
			else if (point.x > breakpointRight) {
				node = node->right;
			}
			else {
				found = true;
			}
		}
		return node;
	}

	void insertBefore(Arc<T>* x, Arc<T>* y) {
		if (isNil(x->left)) {
			x->left = y;
			y->parent = x;
		}
		else {
			x->prev->right = y;
			y->parent = x->prev;
		}

		y->prev = x->prev;
		if (!isNil(y->prev)) {
			y->prev->next = y;
		}
		y->next = x;
		x->prev = y;
		insertFixup(y);
	}

	void insertAfter(Arc<T>* x, Arc<T>* y) {
		if (isNil(x->right)) {
			x->right = y;
			y->parent = x;
		}
		else {
			x->next->left = y;
			y->parent = x->next;
		}

		y->next = x->next;
		if (!isNil(y->next)) {
			y->next->prev = y;
		}
		y->prev = x;
		x->next = y;
		insertFixup(y);
	}

	void replace(Arc<T>* x, Arc<T>* y) {
		transplant(x, y);
		y->left = x->left;
		y->right = x->right;
		if (!isNil(y->left)) {
			y->left->parent = y;
		}
		if (!isNil(y->right)) {
			y->right->parent = y;
		}

		y->prev = x->prev;
		y->next = x->next;
		if (!isNil(y->prev)) {
			y->prev->next = y;
		}
		if (!isNil(y->next)) {
			y->next->prev = y;
		}

		y->color = x->color;
	}

	void remove(Arc<T>* z) {
		auto y = z;
		auto yOriginalColor = y->color;
		auto x = nil;
		if (isNil(z->left)) {
			x = z->right;
			transplant(z, z->right);
		}
		else if (isNil(z->right)) {
			x = z->left;
			transplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			yOriginalColor = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}

		if (yOriginalColor == Arc<T>::Color::Black) {
			removeFixup(x);
		}

		if (!isNil(z->prev)) {
			z->prev->next = z->next;
		}
		if (!isNil(z->next)) {
			z->next->prev = z->prev;
		}
	}

private:
	Arc<T>* nil;
	Arc<T>* root;

	Arc<T>* minimum(Arc<T>* x) const {
		while (!isNil(x->left)) {
			x = x->left;
		}
		return x;
	}

	void transplant(Arc<T>* u, Arc<T>* v) {
		if (isNil(u->parent)) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void insertFixup(Arc<T>* z) {
		while (z->parent->color == Arc<T>::Color::Red) {
			if (z->parent == z->parent->parent->left) {
				auto y = z->parent->parent->right;
				if (y->color == Arc<T>::Color::Red) {
					z->parent->color = Arc<T>::Color::Black;
					y->color = Arc<T>::Color::Black;
					z->parent->parent->color = Arc<T>::Color::Red;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->right) {
						z = z->parent;
						leftRotate(z);
					}

					z->parent->color = Arc<T>::Color::Black;
					z->parent->parent->color = Arc<T>::Color::Red;
					rightRotate(z->parent->parent);
				}
			}
			else {
				auto y = z->parent->parent->left;
				if (y->color == Arc<T>::Color::Red) {
					z->parent->color = Arc<T>::Color::Black;
					y->color = Arc<T>::Color::Black;
					z->parent->parent->color = Arc<T>::Color::Red;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						rightRotate(z);
					}

					z->parent->color = Arc<T>::Color::Black;
					z->parent->parent->color = Arc<T>::Color::Red;
					leftRotate(z->parent->parent);
				}
			}
		}
		root->color = Arc<T>::Color::Black;
	}

	void removeFixup(Arc<T>* x) {
		while (x != root && x->color == Arc<T>::Color::Black) {
			if (x == x->parent->left) {
				auto w = x->parent->right;

				if (w->color == Arc<T>::Color::Red) {
					w->color = Arc<T>::Color::Black;
					x->parent->color = Arc<T>::Color::Red;
					leftRotate(x->parent);
					w = x->parent->right;
				}

				if (w->left->color == Arc<T>::Color::Black && w->right->color == Arc<T>::Color::Black) {
					w->color = Arc<T>::Color::Red;
					x = x->parent;
				}
				else {
					if (w->right->color == Arc<T>::Color::Black) {
						w->left->color = Arc<T>::Color::Black;
						w->color = Arc<T>::Color::Red;
						rightRotate(w);
						w = x->parent->right;
					}

					w->color = x->parent->color;
					x->parent->color = Arc<T>::Color::Black;
					w->right->color = Arc<T>::Color::Black;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				auto w = x->parent->left;
				
				if (w->color == Arc<T>::Color::Red)
				{
					w->color = Arc<T>::Color::Black;
					x->parent->color = Arc<T>::Color::Red;
					rightRotate(x->parent);
					w = x->parent->left;
				}
				
				if (w->left->color == Arc<T>::Color::Black && w->right->color == Arc<T>::Color::Black)
				{
					w->color = Arc<T>::Color::Red;
					x = x->parent;
				}
				else
				{
					if (w->left->color == Arc<T>::Color::Black)
					{
						w->right->color = Arc<T>::Color::Black;
						w->color = Arc<T>::Color::Red;
						leftRotate(w);
						w = x->parent->left;
					}
					
					w->color = x->parent->color;
					x->parent->color = Arc<T>::Color::Black;
					w->left->color = Arc<T>::Color::Black;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = Arc<T>::Color::Black;
	}

	void leftRotate(Arc<T>* x) {
		auto y = x->right;
		x->right = y->left;
		if (!isNil(y->left)) {
			y->left->parent = x;
		}
		y->parent = x->parent;

		if (isNil(x->parent)) {
			root = y;
		}
		else if (x->parent->left == x) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(Arc<T>* y) {
		auto x = y->left;
		y->left = x->right;
		if (!isNil(x->right)) {
			x->right->parent = y;
		}
		x->parent = y->parent;

		if (isNil(y->parent)) {
			root = x;
		}
		else if (y->parent->left == y) {
			y->parent->left = x;
		}
		else {
			y->parent->right = x;
		}
		x->right = y;
		y->parent = x;
	}

	T computeBreakpoint(const Vector2<T>& point1, const Vector2<T>& point2, T l, typename Arc<T>::Side side) const {
		auto x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;

		if (almostEqual(y1, y2)) {
			if (x1 < x2) {
				return (x1 + x2) / 2;
			}
			else {
				return side == Arc<T>::Side::Left ? -std::numeric_limits<T>::infinity() : std::numeric_limits<T>::infinity();
			}
		}

		if (almostEqual(y1, l)) {
			return x1;
		}
		if (almostEqual(y2, l)) {
			return x2;
		}

		auto d1 = 1.0 / (2.0 * (y1 - l));
		auto d2 = 1.0 / (2.0 * (y2 - l));
		auto a = d1 - d2;
		auto b = 2.0 * (x2 * d2 - x1 * d1);
		auto c = (y1 * y1 + x1 * x1 - l * l) * d1 - (y2 * y2 + x2 * x2 - l * l) * d2;
		auto delta = b * b - 4.0 * a * c;
		return (-b + std::sqrt(delta)) / (2.0 * a);
	}

	void free(Arc<T>* x) {
		if (isNil(x)) {
			return;
		}
		else {
			free(x->left);
			free(x->right);
			delete x;
		}
	}
};