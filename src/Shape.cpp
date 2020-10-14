#include "Shape.h"

olc::sycl::Shape::Shape() : olc::sycl::abs::Shape() {}

void olc::sycl::Shape::setPoint(int x, int y, const olc::Pixel& color)
{
    setPoint({x, y, color});
}

void olc::sycl::Shape::setPoint(const Point& point)
{
    for (auto& index : this->buffer)
        if (index.x == point.x && index.y == point.y)
        {
            index.color = point.color;
            return;
        }
    buffer.push_back(point);
}

olc::sycl::Shape::Point olc::sycl::Shape::getPoint(int x, int y) const
{
    for (const auto& point : buffer)
        if (point.x == x && point.y == y)
            return point;
    return Point{x, y};
}

std::vector<olc::sycl::Shape::Point> olc::sycl::Shape::getBuffer() const
{
    return buffer;
}
