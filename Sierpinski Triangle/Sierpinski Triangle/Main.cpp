#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <queue>

struct Vector2D
{
public:
	int x, y;

public:
	Vector2D operator+(const Vector2D& vector) const 
	{
		return Vector2D{ x + vector.x, y + vector.y };
	}

	Vector2D& operator/(const int value)
	{
		x /= value;
		y /= value;

		return *this;
	}
};

struct Triangle
{
public:
	Vector2D vertices[3]{};

public:
	Triangle* GetMiddleTriangles()
	{
		Triangle triangles[3]{};
		Triangle middleTriangle{ (vertices[0] + vertices[1]) / 2,
								 (vertices[1] + vertices[2]) / 2,
								 (vertices[2] + vertices[0]) / 2 };

		triangles[0].vertices[0] = vertices[0];
		triangles[0].vertices[1] = middleTriangle.vertices[0]; 
		triangles[0].vertices[2] = middleTriangle.vertices[2];

		triangles[1].vertices[0] = middleTriangle.vertices[0];
		triangles[1].vertices[1] = vertices[1];
		triangles[1].vertices[2] = middleTriangle.vertices[1];

		triangles[2].vertices[0] = middleTriangle.vertices[2];
		triangles[2].vertices[1] = middleTriangle.vertices[1];
		triangles[2].vertices[2] = vertices[2];

		return triangles;
	}

	void DrawTriangle(olc::PixelGameEngine* engine)
	{
		engine->DrawLine(vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y);
		engine->DrawLine(vertices[1].x, vertices[1].y, vertices[2].x, vertices[2].y);
		engine->DrawLine(vertices[2].x, vertices[2].y, vertices[0].x, vertices[0].y);
	}
};

class HatzTrinagle : public olc::PixelGameEngine
{
public:
	HatzTrinagle(const Vector2D& bounds) : _bounds(bounds)
	{
		sAppName = "Hatz-Triangle";
	}

private:
	Vector2D _bounds{};

	std::deque<Triangle> _triangles{};

public:
	bool OnUserCreate() override
	{
		Triangle mainTriangle{ _bounds.x / 2, 0,
							   0, _bounds.y,
							   _bounds.x, _bounds.y };

		auto* middleTriangles = mainTriangle.GetMiddleTriangles();
		_triangles.push_back(middleTriangles[0]);
		_triangles.push_back(middleTriangles[1]);
		_triangles.push_back(middleTriangles[2]);

		return true;
	}

	bool OnUserUpdate(float elapsedTime) override
	{
		int length = _triangles.size();
		while(length)
		{
			auto& triangle = _triangles[0];
			triangle.DrawTriangle(this);

			auto* middleTriangles = triangle.GetMiddleTriangles();
			_triangles.push_back(middleTriangles[0]);
			_triangles.push_back(middleTriangles[1]);
			_triangles.push_back(middleTriangles[2]);

			_triangles.pop_front(); length--;
		}

		return true;
	}
};

int main()
{
	Vector2D screen{ 1920, 1080 };
	Vector2D bounds{ 1919, 1079 };

	/*
	do std::cin >> bounds.x >> bounds.y;
	while (bounds.x > screen.x - 1 || bounds.y > screen.y - 1);
	*/
	
	HatzTrinagle window(bounds);
	if (window.Construct(screen.x, screen.y, 1, 1, true))
		window.Start();

	return 0;
}