#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <queue>

struct Vector2D
{
public:
	int x, y;

public:
	Vector2D GetMiddle(const Vector2D& vector)
	{
		return Vector2D { (x + vector.x) / 2,
						  (y + vector.y) / 2 };
	}

	void Draw(olc::PixelGameEngine* engine)
	{
		engine->Draw(x, y);
	}

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

class HatzTrinagleV1 : public olc::PixelGameEngine
{
private:
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

public:
	HatzTrinagleV1(const float delay, Vector2D* bounds) : 
		_delay(delay), _bounds(bounds)
	{
		sAppName = "Hatz-Triangle";
	}

private:
	float _delay;
	Vector2D* _bounds;

	float _counter;

	std::queue<Triangle> _triangles{};

public:
	bool OnUserCreate() override
	{
		Triangle mainTriangle{ _bounds->x / 2, 0,
							   0, _bounds->y,
							   _bounds->x, _bounds->y };

		auto* middleTriangles = mainTriangle.GetMiddleTriangles();
		_triangles.push(middleTriangles[0]);
		_triangles.push(middleTriangles[1]);
		_triangles.push(middleTriangles[2]);

		return true;
	}
	
	bool OnUserUpdate(float elapsedTime) override
	{
		_counter += elapsedTime;
		if (_counter < _delay)
			return true;

		_counter = 0.0f;

		int length = _triangles.size();
		while(length)
		{
			auto& triangle = _triangles.front();
			triangle.DrawTriangle(this);

			auto* middleTriangles = triangle.GetMiddleTriangles();
			_triangles.push(middleTriangles[0]);
			_triangles.push(middleTriangles[1]);
			_triangles.push(middleTriangles[2]);

			_triangles.pop(); length--;
		}

		return true;
	}
};

class HatzTrinagleV2 : public olc::PixelGameEngine
{
public:
	HatzTrinagleV2(const int speed, Vector2D* bounds) : 
		_speed(speed), _bounds(bounds)
	{
		sAppName = "Hatz-Triangle";
	}

private:
	int _speed;
	Vector2D* _bounds;

	float _counter, _delay = 5.0f;

	Vector2D _trianglePoints[3];
	std::vector<Vector2D> _insidePoints{};

public:
	bool OnUserCreate() override
	{
		_trianglePoints[0] = Vector2D{ _bounds->x / 2, 0 };
		_trianglePoints[1] = Vector2D{ 0, _bounds->y };
		_trianglePoints[2] = Vector2D{ _bounds->x, _bounds->y };

		for (int i = 0; i < 3; i++)
			_trianglePoints[i].Draw(this);

		AddMiddlePoint(_trianglePoints[0], _trianglePoints[1]);

		return true;
	}

	bool OnUserUpdate(float elapsedTime) override
	{
		for (int i = 0; i < _speed; i++)
			AddPoint();

		return true;
	}

private:
	void AddPoint()
	{
		auto& trianglePoint = GetRandomTrianglePoint();
		auto& insidePoint = GetRandomInsidePoint();

		AddMiddlePoint(trianglePoint, insidePoint);
	}

	void AddMiddlePoint(Vector2D& point1, const Vector2D& point2)
	{
		auto middlePoint = point1.GetMiddle(point2);

		middlePoint.Draw(this);
		_insidePoints.push_back(middlePoint);
	}

	Vector2D& GetRandomTrianglePoint()
	{
		int index = rand() % 3;
		return _trianglePoints[index];
	}

	Vector2D& GetRandomInsidePoint()
	{
		int index = rand() % _insidePoints.size();
		return _insidePoints[index];
	}
};

void CreateVersionOne(const Vector2D& screen, Vector2D& bounds);
void CreateVersionTwo(const Vector2D& screen, Vector2D& bounds);

int main()
{
	srand(time(NULL));

	Vector2D screen{ 1920, 1080 };
	Vector2D bounds{ 1919, 1079 };

	/*
	do std::cin >> bounds.x >> bounds.y;
	while (bounds.x > screen.x - 1 || bounds.y > screen.y - 1);
	*/

	int version;
	std::cout << "Version: "; std::cin >> version;

	if (version > 2 || version < 1)
		return 0;

	if (version == 1) CreateVersionOne(screen , bounds);
	else CreateVersionTwo(screen, bounds);

	return 0;
}

void CreateVersionOne(const Vector2D& screen, Vector2D& bounds)
{
	float delay;
	std::cout << "Delay: "; std::cin >> delay;

	HatzTrinagleV1 window(delay, &bounds);
	if (window.Construct(screen.x, screen.y, 1, 1, true))
		window.Start();
}

void CreateVersionTwo(const Vector2D& screen, Vector2D& bounds)
{
	int speed;
	std::cout << "Speed: "; std::cin >> speed;

	HatzTrinagleV2 window(speed, &bounds);
	if (window.Construct(screen.x, screen.y, 1, 1, true))
		window.Start();
}