#ifndef PGE_TRISYCL_EXTENSION_TEST_SHAPE_H
#define PGE_TRISYCL_EXTENSION_TEST_SHAPE_H

#include "test/pch.h"

#include "olcSYCL.h"

namespace olc::sycl::test
{
class ShapeTest : public testing::Test
    {
    protected:
        static bool arePointsEqual(const olc::sycl::Shape::Point& p1, const olc::sycl::Shape::Point& p2);
        static olc::sycl::Shape::Point constructPoint(int x, int y);
        static std::vector<olc::sycl::Shape::Point>& getBuffer(const olc::sycl::Shape* s);
    public:
        void SetUp() override;

        olc::sycl::Shape shape;
    };
}

#endif //PGE_TRISYCL_EXTENSION_TEST_SHAPE_H
