#ifndef PGE_TRISYCL_EXTENSION_ABSTRACT_SHAPE_H
#define PGE_TRISYCL_EXTENSION_ABSTRACT_SHAPE_H

#include "pch.h"

namespace olc::sycl::abs
{
    class Shape
    {
    public:
        struct Point
        {
            int x = 0, y = 0;
            olc::Pixel color = olc::BLANK;
        };

    protected:
        std::vector<Point> buffer;
        cl::sycl::buffer<Point, 1> syclBuffer;
        cl::sycl::default_selector syclDeviceSelector;
        cl::sycl::queue syclQueue;

        void updateSyclBuffer();

    public:
        virtual void setPoint(int x, int y, const olc::Pixel& color) = 0;

        virtual void setPoint(const Point& point) = 0;

        virtual Point getPoint(int x, int y) const = 0;

        virtual std::vector<Point> getBuffer() const = 0;

        Shape();
    };
}

#endif //PGE_TRISYCL_EXTENSION_ABSTRACT_SHAPE_H
