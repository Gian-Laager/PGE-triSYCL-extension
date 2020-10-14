#include "test/Shape.h"

using namespace olc::sycl::test;

void ShapeTest::SetUp()
{
    shape = olc::sycl::Shape();
}

bool ShapeTest::arePointsEqual(const olc::sycl::Shape::Point& p1, const olc::sycl::Shape::Point& p2)
{
    bool retValue = true;
    retValue &= p1.color == p2.color;
    retValue &= p1.x == p2.x;
    retValue &= p1.y == p2.y;
    return retValue;
}

olc::sycl::Shape::Point ShapeTest::constructPoint(int x, int y)
{
    return olc::sycl::Shape::Point{(unsigned char) x,
                                   (unsigned char) y,
                                   (unsigned char) (x * y)};
}

std::vector<olc::sycl::Shape::Point>& ShapeTest::getBuffer(const olc::sycl::Shape* s)
{
    return *(std::vector<olc::sycl::Shape::Point>*) s;
}

TEST_F(ShapeTest, SetShapePoint_WithIndex_1Point_WillSetTheValueCorectyl)
{
    const olc::sycl::Shape::Point point = constructPoint(0, 0);
    shape.setPoint(point.x, point.y, point.color);
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x, point.y), point));
}

TEST_F(ShapeTest, SetShapePoint_WithPoint_1Point_WillSetTheValueCorectyl)
{
    const olc::sycl::Shape::Point point = constructPoint(0, 0);
    shape.setPoint(point);
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x, point.y), point));
}

TEST_F(ShapeTest, SetShapePoint_WithIndex_MultiplePoints_WillSetTheValueCorectyl)
{
    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
            shape.setPoint(x, y, constructPoint(x, y).color);

    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
        {
            ASSERT_TRUE(arePointsEqual(shape.getPoint(x, y), constructPoint(x, y)));
        }
}

TEST_F(ShapeTest, SetShapePoint_WithPoint_MultiplePoints_WillSetTheValueCorectyl)
{
    const int w = 100;
    const int h = 100;
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
        {
            ASSERT_TRUE(arePointsEqual(shape.getPoint(x, y), constructPoint(x, y)));
        }
}

TEST_F(ShapeTest, GetShapePointWithInvalidIndices_WillReturnDefaultValue)
{
    const olc::sycl::Shape::Point point = constructPoint(0, 0);
    shape.setPoint(point);
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x + 1, point.y), olc::sycl::Shape::Point{point.x + 1, point.y, olc::BLANK}));
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x, point.y + 1), olc::sycl::Shape::Point{point.x, point.y + 1, olc::BLANK}));
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x + 1, point.y + 1),
                               olc::sycl::Shape::Point{point.x + 1, point.y + 1, olc::BLANK}));
}

TEST_F(ShapeTest, OverridingAnIndexWidthSetShapePoint_WithPoint_WillGetShapePointReturnLatestValue)
{
    int x = 0, y = 0;
    shape.setPoint(constructPoint(x, y));
    olc::sycl::Shape::Point point = constructPoint(x, y);
    point.color.n += 0x010101;
    shape.setPoint(point);
    ASSERT_TRUE(arePointsEqual(shape.getPoint(x,y), point));
}

TEST_F(ShapeTest, NotPosibleThatThereAreDublicatedIndices)
{
    const int w = 100;
    const int h = 100;
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            for (int i = 0; i < 5; i++)
                shape.setPoint(constructPoint(x, y));

    std::vector<olc::sycl::Shape::Point> buffer = shape.getBuffer();
    for (int i = 0; i < buffer.size(); i++)
        for (int j = 0; j < buffer.size(); j++)
            if (i != j)
            {
                ASSERT_FALSE(buffer[i].x == buffer[j].x && buffer[i].y == buffer[j].y);
            }
}



