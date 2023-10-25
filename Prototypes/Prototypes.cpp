#include <random>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>

#include "Prototypes.h"
#include "src/FortuneAlgorithm.h"
#include <functional>

vector2 randomGradient(int ix, int iy) {
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3284157443;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float angle = a * (3.14159265 / ~(~0u >> 1));

	vector2 gradient;
	gradient.x = sin(angle);
	gradient.y = cos(angle);

	return gradient;
}

float dotGridGradient(int ix, int iy, float x, float y) {
	vector2 gradient = randomGradient(ix, iy);

	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w) {
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float perlin(float x, float y) {
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - (float)x0;
	float sy = y - (float)y0;

	float n0 = dotGridGradient(x0, y0, x, y);
	float n1 = dotGridGradient(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);

	float result = interpolate(ix0, ix1, sy);
	return result;
}

int getIndex(int x, int y, int width, int height, int quadrant) {
	switch (quadrant) {
	case 0:
		return (y * width + x);
	case 1:
		return (y * width + x + width / 2);
	case 2:
		return (width * height / 2 + y * width + x);
	case 3:
		return (width * height / 2 + y * width + x + width / 2);
	default:
		return 0;
	}
}

bool compareVerticesByY(const Vector2<int>& first, const Vector2<int>& second) {
	return first.y < second.y;
}

std::pair<int, int> performBresenhamStep(const Vector2<int>& start, const Vector2<int>& dest, int err, sf::Uint8* pixels, int width, int height, int quadrant, int color) {
	int dx = abs(start.x - dest.x);
	int sx = start.x < dest.x ? 1 : -1;

	int dy = -abs(start.y - dest.y);
	int sy = start.y < dest.y ? 1 : -1;

	int e = err == -1 ? dx + dy : err;

	int x = start.x;
	int y = start.y;

	while (true) {
		int index = getIndex(x, y, width, height, quadrant) * 4;
		pixels[index] = (color % 2 + 1) * 127;
		pixels[index + 1] = ((color / 2) % 2 + 1) * 127;
		pixels[index + 2] = ((color / 4) % 2 + 1) * 127;
		pixels[index + 3] = 255;

		if (x == dest.x && y == dest.y) {
			break;
		}

		int e2 = 2 * e;

		if (e2 >= dy) {
			if (x == dest.x) {
				return { x, e };
			}
			e = e + dy;
			x = x + sx;
		}
		if (e2 <= dx) {
			if (y == dest.y) {
				return { x, e };
			}
			e = e + dx;
			y = y + sy;
			return { x, e };
		}
	}
}

void drawBottomFlatTriangle(Triangle* triangle, sf::Uint8* pixels, int width, int height, int quadrant) {
	Vector2<int> v1Left;
	Vector2<int> v1Right;
	Vector2<int> v2 = triangle->v2.x < triangle->v3.x ? triangle->v2 : triangle->v3;
	Vector2<int> v3 = triangle->v2.x < triangle->v3.x ? triangle->v3 : triangle->v2;

	int newLeftX = triangle->v1.x;
	int newRightX = triangle->v1.x;
	int errLeft = -1;
	int errRight = -1;

	for (int y = triangle->v1.y + 1; y <= triangle->v2.y; y++) {
		v1Left = { newLeftX, y - 1 };
		v1Right = { newRightX, y - 1 };
		std::pair resultLeft = performBresenhamStep(v1Left, v2, errLeft, pixels, width, height, quadrant, triangle->color);
		newLeftX = resultLeft.first;
		errLeft = resultLeft.second;
		std::pair resultRight = performBresenhamStep(v1Right, v3, errRight, pixels, width, height, quadrant, triangle->color);
		newRightX = resultRight.first;
		errRight = resultRight.second;

		for (int x = newLeftX; x <= newRightX; x++) {
			int index = getIndex(x, y, width, height, quadrant) * 4;
			pixels[index] = (triangle->color % 2 + 1) * 127;
			pixels[index + 1] = ((triangle->color / 2) % 2 + 1) * 127;
			pixels[index + 2] = ((triangle->color / 4) % 2 + 1) * 127;
			pixels[index + 3] = 255;
		}
	}
}

void drawTopFlatTriangle(Triangle* triangle, sf::Uint8* pixels, int width, int height, int quadrant) {
	Vector2<int> v1 = triangle->v1.x < triangle->v2.x ? triangle->v1 : triangle->v2;
	Vector2<int> v2 = triangle->v1.x < triangle->v2.x ? triangle->v2 : triangle->v1;
	Vector2<int> v3Left;
	Vector2<int> v3Right;

	int newLeftX = triangle->v3.x;
	int newRightX = triangle->v3.x;
	int errLeft = -1;
	int errRight = -1;

	for (int y = triangle->v3.y; y >= triangle->v1.y; y--) {
		v3Left = { newLeftX, y };
		v3Right = { newRightX, y };
		std::pair resultLeft = performBresenhamStep(v3Left, v1, errLeft, pixels, width, height, quadrant, triangle->color);
		newLeftX = resultLeft.first;
		errLeft = resultLeft.second;
		std::pair resultRight = performBresenhamStep(v3Right, v2, errRight, pixels, width, height, quadrant, triangle->color);
		newRightX = resultRight.first;
		errRight = resultRight.second;

		for (int x = newLeftX; x <= newRightX; x++) {
			int index = getIndex(x, y, width, height, quadrant) * 4;
			pixels[index] = (triangle->color % 2 + 1) * 127;
			pixels[index + 1] = ((triangle->color / 2) % 2 + 1) * 127;
			pixels[index + 2] = ((triangle->color / 4) % 2 + 1) * 127;
			pixels[index + 3] = 255;
		}
	}
}

void drawTriangle(Triangle* triangle, sf::Uint8* pixels, int width, int height, int quadrant) {
	if (triangle->v2.y == triangle->v3.y) {
		drawBottomFlatTriangle(triangle, pixels, width, height, quadrant);
	}
	else if (triangle->v1.y == triangle->v2.y) {
		drawTopFlatTriangle(triangle, pixels, width, height, quadrant);
	}
	else {
		Vector2<int> v4((int)(triangle->v1.x + ((float)(triangle->v2.y - triangle->v1.y) / (float)(triangle->v3.y - triangle->v1.y)) * (triangle->v3.x - triangle->v1.x)), triangle->v2.y);
		Triangle topTriangle;
		Triangle bottomTriangle;
		topTriangle = { triangle->v1, triangle->v2, v4, triangle->color };
		bottomTriangle = { triangle->v2, v4, triangle->v3, triangle->color };
		drawBottomFlatTriangle(&topTriangle, pixels, width, height, quadrant);
		drawTopFlatTriangle(&bottomTriangle, pixels, width, height, quadrant);
	}
}

int main()
{
	const int windowWidth = 1024;
	const int windowHeight = 512;

	const int elementWidth = windowWidth / 2;
	const int elementHeight = windowHeight / 2;

	sf::RenderWindow  window(sf::VideoMode(windowWidth, windowHeight, 32), "Perlin");
	
	sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight * 4];

	//Perlin Noise for Height
	const int GRID_SIZE = 50;

	for (int x = 0; x < elementWidth; x++) {
		for (int y = 0; y < elementHeight; y++) {
			int index = (y * windowWidth + x) * 4;

			float val = 0;

			float freq = 1;
			float amp = 1;

			for (int i = 0; i < 12; i++) {
				val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

				freq *= 2;
				amp /= 2;
			}

			val *= 1.2;

			if (val > 1.0f) {
				val = 1.0f;
			}
			else if (val < -1.0f) {
				val = -1.0f;
			}

			int color = (int)(((val + 1.0f) * 0.5f) * 255);

			pixels[index] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255;
		}
	}

	//Voronoi Diagram Generation
	const int numberOfRelaxations = 2;

	std::vector<Vector2<double>> vertices;
	vertices.push_back(Vector2(225.0 / elementWidth, 104.0 / elementHeight));
	vertices.push_back(Vector2(132.0 / elementWidth, 254.0 / elementHeight));
	vertices.push_back(Vector2(62.0 / elementWidth, 16.0 / elementHeight));
	vertices.push_back(Vector2(125.0 / elementWidth, 211.0 / elementHeight));
	vertices.push_back(Vector2(93.0 / elementWidth, 126.0 / elementHeight));
	vertices.push_back(Vector2(163.0 / elementWidth, 177.0 / elementHeight));
	vertices.push_back(Vector2(334.0 / elementWidth, 134.0 / elementHeight));
	vertices.push_back(Vector2(463.0 / elementWidth, 225.0 / elementHeight));
	vertices.push_back(Vector2(195.0 / elementWidth, 72.0 / elementHeight));
	vertices.push_back(Vector2(222.0 / elementWidth, 117.0 / elementHeight));
	vertices.push_back(Vector2(489.0 / elementWidth, 87.0 / elementHeight));
	vertices.push_back(Vector2(22.0 / elementWidth, 66.0 / elementHeight));
	vertices.push_back(Vector2(386.0 / elementWidth, 218.0 / elementHeight));
	vertices.push_back(Vector2(335.0 / elementWidth, 220.0 / elementHeight));
	vertices.push_back(Vector2(52.0 / elementWidth, 36.0 / elementHeight));

	FortuneAlgorithm fortuneAlgorithm(vertices);

	fortuneAlgorithm.construct();
	fortuneAlgorithm.bound(Box<double>{-0.05, -0.05, 1.05, 1.05});
	Diagram diagram = fortuneAlgorithm.getDiagram();
	diagram.intersect(Box<double>{0.0, 0.0, 1.0, 1.0});

	for (int i = 0; i < numberOfRelaxations; i++) {
		vertices = diagram.computeLloydRelaxation();
		FortuneAlgorithm fortuneAlgorithm(vertices);
		fortuneAlgorithm.construct();
		fortuneAlgorithm.bound(Box<double>{-0.05, -0.05, 1.05, 1.05});
		Diagram diagram = fortuneAlgorithm.getDiagram();
		diagram.intersect(Box<double>{0.0, 0.0, 1.0, 1.0});
	}

	std::vector<Diagram<double>::Face> faces = diagram.getFaces();
	std::list<Diagram<double>::Vertex> diagramVertices = diagram.getVertices();

	int* cellColors = new int[elementWidth * elementHeight];
	int i = 1;

	std::vector<Triangle> triangles;

	for (Diagram<double>::Face face : faces) {
		Diagram<double>::HalfEdge* edge = face.outerComponent;
		do {
			std::vector<Vector2<int>> corners;
			corners.push_back(Vector2<int>{ (int)(edge->origin->point.x * (elementWidth - 1)), (int)(edge->origin->point.y * (elementHeight - 1)) });
			corners.push_back(Vector2<int>{ (int)(edge->destination->point.x * (elementWidth - 1)), (int)(edge->destination->point.y * (elementHeight - 1)) });
			corners.push_back(Vector2<int>{ (int)(face.site->point.x * (elementWidth - 1)), (int)(face.site->point.y * (elementHeight - 1)) });
			std::sort(corners.begin(), corners.end(), compareVerticesByY);

			triangles.push_back(Triangle{ corners[0], corners[1], corners[2], i });

//			int originX = edge->origin->point.x * (elementWidth - 1);
//			int originY = edge->origin->point.y * (elementHeight - 1);
//			int destinationX = edge->destination->point.x * (elementWidth - 1);
//			int destinationY = edge->destination->point.y * (elementHeight - 1);
//			int xDistance = originX - destinationX;
//			int yDistance = originY - destinationY;
//			if (xDistance < 0) {
//				for (int k = 0; k >= xDistance; k--) {
//					int x = originX - k;
//					double gradient = (double)yDistance / (double)xDistance;
//					int y = originY - (gradient * k);
//					if (cellColors[y * elementWidth + x] == 0 || xDistance )
//					cellColors[y * elementWidth + x] = i;
//				}
//			}
			edge = edge->next;
		} while (edge != face.outerComponent);
		i++;
	}

//	int color = 0;
//
//	for (int x = 0; x < elementWidth; x++) {
//		for (int y = 0; y < elementHeight; y++) {
//			if (cellColors[y * elementWidth + x] > 0) {
//				color = cellColors[y * elementWidth + x];
//			}
//			else {
//				cellColors[y * elementWidth + x] = color;
//			}
//			int index = getIndex(x, y, windowWidth, windowHeight, 1) * 4;
//			pixels[index] = (cellColors[y * elementWidth + x] % 2 + 1) * 127;
//			pixels[index + 1] = ((cellColors[y * elementWidth + x] / 2) % 2 + 1) * 127;
//			pixels[index + 2] = ((cellColors[y * elementWidth + x] / 4) % 2 + 1) * 127;
//			pixels[index + 3] = 255;
//		}
//	}

	for (Triangle triangle : triangles) {
		drawTriangle(&triangle, pixels, windowWidth, windowHeight, 1);
	}

	sf::Texture texture;
	sf::Sprite sprite;

	texture.create(windowWidth, windowHeight);
	texture.update(pixels);

	sprite.setTexture(texture);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}