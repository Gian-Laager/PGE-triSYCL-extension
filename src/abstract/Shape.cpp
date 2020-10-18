#include "abstract/Shape.h"

olc::sycl::abs::Shape::Shape() : syclQueue(syclDeviceSelector)
{
    updateSyclBuffer();
}

void olc::sycl::abs::Shape::updateSyclBuffer() const
{
    syclBuffer = cl::sycl::buffer<Point, 1>(buffer.data(), cl::sycl::range<1>(buffer.size()));
}

olc::sycl::abs::Shape::Shape(int initialSize) : buffer(initialSize) {}

