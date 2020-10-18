#include "test/Shape.h"

using namespace olc::sycl::test;

void ShapeTest::SetUp()
{
    shape = olc::sycl::Shape();
}

bool ShapeTestPGE::OnUserCreate()
{
    return true;
}

bool ShapeTestPGE::OnUserUpdate(float fElapsedTime)
{
    return true;
}

ShapeTestPGE::ShapeTestPGE() : olc::PixelGameEngine()
{
    sAppName = "Shape Test";
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
    return olc::sycl::Shape::Point{x, y, {(unsigned char) x,
                                          (unsigned char) y,
                                          (unsigned char) (x * y),
                                          (unsigned char) (x % (y + 1))}};
}

std::vector<olc::sycl::Shape::Point>& ShapeTest::getBuffer(const olc::sycl::Shape* s)
{
    return *(std::vector<olc::sycl::Shape::Point>*) s;
}

void ShapeTest::TearDown()
{
    pge.SetPixelMode(olc::Pixel::NORMAL);
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
    const int h = 150;
    shape.reserve(w * h);
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
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x + 1, point.y),
                               olc::sycl::Shape::Point{point.x + 1, point.y, olc::BLANK}));
    ASSERT_TRUE(arePointsEqual(shape.getPoint(point.x, point.y + 1),
                               olc::sycl::Shape::Point{point.x, point.y + 1, olc::BLANK}));
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
    ASSERT_TRUE(arePointsEqual(shape.getPoint(x, y), point));
}

TEST_F(ShapeTest, NotPosibleThatThereAreDublicatedIndices)
{
    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);

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

TEST_F(ShapeTest, WillDrawFunction_int_drawModeNormal_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::NORMAL);
    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_int_drawModeNormal");
        shape.draw(&pge, 0, 0);
    }

    for (auto& point : shape.getBuffer())
        ASSERT_EQ(point.color, pge.GetDrawTarget()->GetPixel(point.x, point.y));
}

TEST_F(ShapeTest, WillDrawFunction_vector_drawModeNormal_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::NORMAL);
    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_vector_drawModeNormal");
        shape.draw(&pge, {0, 0});
    }

    for (auto& point : shape.getBuffer())
        ASSERT_EQ(point.color, pge.GetDrawTarget()->GetPixel(point.x, point.y));
}

TEST_F(ShapeTest, WillDrawFunction_int_drawModeAlpha_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::ALPHA);

    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));

    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_int_drawModeAlpha");
        shape.draw(&pge, 0, 0);
    }

    for (auto& point : shape.getBuffer())
    {
        olc::Pixel pColor = pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()];
        ASSERT_EQ(olc::sycl::Shape::calcPixelColorWithAlpha(((olc::sycl::PixelGameEngine*) &pge)->getBlendFactor(),
                                                            point.color, pColor),
                  pge.GetDrawTarget()->GetPixel(point.x, point.y));
    }
    delete[] pColDataCopy;
}

TEST_F(ShapeTest, WillDrawFunction_vector_drawModeAlpha_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::ALPHA);
    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));
    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_vector_drawModeAlpha");
        shape.draw(&pge, {0, 0});
    }

    for (auto& point : shape.getBuffer())
    {
        olc::Pixel pColor = pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()];
        ASSERT_EQ(olc::sycl::Shape::calcPixelColorWithAlpha(((olc::sycl::PixelGameEngine*) &pge)->getBlendFactor(),
                                                            point.color, pColor),
                  pge.GetDrawTarget()->GetPixel(point.x, point.y));
    }
    delete[] pColDataCopy;
}

TEST_F(ShapeTest, WillDrawFunction_int_dawModeMask_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::MASK);

    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));
    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_int_dawModeMask");
        shape.draw(&pge, 0, 0);
    }

    for (auto& point : shape.getBuffer())
        if (point.color.a == 255)
            ASSERT_EQ(point.color, pge.GetDrawTarget()->GetPixel(point.x, point.y));
        else
            ASSERT_EQ(pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()].n >> 8,
                      pge.GetDrawTarget()->GetPixel(point.x, point.y).n >> 8);
    delete[] pColDataCopy;
}

TEST_F(ShapeTest, WillDrawFunction_vector_drawModeMask_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::MASK);

    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);
    pge.SetDrawTarget(new olc::Sprite(w, h));
    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_vector_drawModeMask");
        shape.draw(&pge, {0, 0});
    }

    for (auto& point : shape.getBuffer())
        if (point.color.a == 255)
            ASSERT_EQ(point.color, pge.GetDrawTarget()->GetPixel(point.x, point.y));
        else
            ASSERT_EQ(pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()],
                      pge.GetDrawTarget()->GetPixel(point.x, point.y));
    delete[] pColDataCopy;
}

olc::Pixel customDrawFunction(int x, int y, const olc::Pixel& nColor, const olc::Pixel& pColor)
{
    float a = (float) (nColor.a / 255.0f);
    float c = 1.0f - a;
    float r = a * (float) nColor.r + c * (float) pColor.r;
    float g = a * (float) nColor.g + c * (float) pColor.g;
    float b = a * (float) nColor.b + c * (float) pColor.b;
    return olc::Pixel{(unsigned char) (r * x), (unsigned char) (g * y), (unsigned char) b};
}

TEST_F(ShapeTest, WillDrawFunction_int_dawModeCustom_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::CUSTOM);
    pge.SetPixelMode(customDrawFunction);

    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);

    pge.SetDrawTarget(new olc::Sprite(w, h));
    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_int_dawModeCustom");
        shape.draw(&pge, 0, 0);
    }

    for (auto& point : shape.getBuffer())
    {
        olc::Pixel pColor = pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()];
        ASSERT_EQ(customDrawFunction(point.x, point.y, point.color, pColor),
                  pge.GetDrawTarget()->GetPixel(point.x, point.y));
    }
}

TEST_F(ShapeTest, WillDrawFunction_vector_dawModeCustom_SetPGEpCol)
{
    pge.SetPixelMode(olc::Pixel::CUSTOM);
    pge.SetPixelMode(customDrawFunction);

    const int w = 100;
    const int h = 150;
    shape.reserve(w * h);

    pge.SetDrawTarget(new olc::Sprite(w, h));
    olc::Pixel* pColDataCopy = new olc::Pixel[pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight()];
    memcpy(pColDataCopy, pge.GetDrawTarget()->pColData,
           pge.GetDrawTargetWidth() * pge.GetDrawTargetHeight() * sizeof(olc::Pixel));

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            shape.setPoint(constructPoint(x, y));

    {
        Timer("DrawFunction_vector_dawModeCustom");
        shape.draw(&pge, {0, 0});
    }

    for (auto& point : shape.getBuffer())
    {
        olc::Pixel pColor = pColDataCopy[point.x + point.y * pge.GetDrawTargetWidth()];
        ASSERT_EQ(customDrawFunction(point.x, point.y, point.color, pColor),
                  pge.GetDrawTarget()->GetPixel(point.x, point.y));
    }
}

TEST_F(ShapeTest, WillReserveSetBufferSizeAccordingly)
{
    int size = 20;
    shape.reserve(size);
    ASSERT_EQ(shape.capacity(), size);
}

TEST_F(ShapeTest, WillReserveSetBufferSizeAccordinglyWhenThersAnInitalSize)
{
    int size = 20;
    for (int i = 0; i < size; i++)
        shape.setPoint(constructPoint(i, i));
    shape.reserve(size * 2);
    ASSERT_EQ(shape.capacity(), size * 2);
}

TEST_F(ShapeTest, WillIntConstructorReserveRightsize)
{
    const int size = 10;
    olc::sycl::Shape s{size};
    ASSERT_EQ(s.capacity(), size);
}

TEST_F(ShapeTest, WillSizeReturnRightValue)
{
    int size = 20;
    for (int i = 0; i < size; i++)
        shape.setPoint(constructPoint(i, i));
    ASSERT_EQ(size, shape.size());
}

