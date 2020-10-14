#ifndef PGE_TRISYCL_EXTENSION_SHAPE_H
#define PGE_TRISYCL_EXTENSION_SHAPE_H

#include "pch.h"
#include "abstract/Shape.h"

namespace olc::sycl
{
    class Shape : public olc::sycl::abs::Shape
    {
     public:
        Shape();

        void setPoint(int x, int y, const olc::Pixel& color) override;

        void setPoint(const Point& point) override;

        Point getPoint(int x, int y) const override;

        std::vector<Point> getBuffer() const override;
    };
}

#endif //PGE_TRISYCL_EXTENSION_SHAPE_H
