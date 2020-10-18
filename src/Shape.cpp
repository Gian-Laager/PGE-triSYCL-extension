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

bool olc::sycl::Shape::draw(olc::PixelGameEngine* olcPge, const olc::vi2d& pos) const
{
    auto* pge = (olc::sycl::PixelGameEngine*) olcPge;
    Sprite* drawTarget = pge->GetDrawTarget();
    if (!drawTarget)
        return false;

    updateSyclBuffer();
    cl::sycl::buffer<olc::Pixel, 2> b_pColData{drawTarget->pColData,
                                               cl::sycl::range<2>{(unsigned long) drawTarget->height,
                                                                  (unsigned long) drawTarget->width}};
    Pixel::Mode pixelMode = pge->GetPixelMode();

    if (pixelMode == Pixel::NORMAL)
        return drawNormal(pos, b_pColData);

    if (pixelMode == Pixel::MASK)
        return drawMask(pos, b_pColData);

    if (pixelMode == Pixel::ALPHA)
        return drawAlpha(pge, pos, b_pColData);

    if (pixelMode == Pixel::CUSTOM)
        return drawCustom(pge, pos, b_pColData);

    return false;
}

bool olc::sycl::Shape::drawNormal(const olc::vi2d& pos, cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const
{
    this->syclQueue.submit([&](trisycl::handler& cgh) {
        auto accBuffer = this->syclBuffer.get_access<trisycl::access::mode::read>();
        auto accPColData = b_pColData.get_access<trisycl::access::mode::write>();

        cgh.parallel_for<class shapeDrawPixelModeNomal>(trisycl::range<1>{this->buffer.size()}, [=](trisycl::id<1> i) {
            accPColData[trisycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                        (unsigned long) accBuffer[i].x + pos.x}] = accBuffer[i].color;
        });
    });

    return true;
}

bool olc::sycl::Shape::drawMask(const vi2d& pos, cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const
{
    syclQueue.submit([&](cl::sycl::handler& cgh) {
        auto accBuffer = syclBuffer.get_access<cl::sycl::access::mode::read>();
        auto accPColData = b_pColData.get_access<cl::sycl::access::mode::write>();

        cgh.parallel_for<class shapeDrawPixelModeNomal>(cl::sycl::range<1>{buffer.size()}, [=](cl::sycl::id<1> i) {
            if (accBuffer[i].color.a == 255)
                accPColData[cl::sycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                            (unsigned long) accBuffer[i].x + pos.x}] = accBuffer[i].color;
        });
    });
    return true;
}

bool olc::sycl::Shape::drawCustom(olc::sycl::PixelGameEngine* pge, const vi2d& pos, cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const
{
    syclQueue.submit([&](cl::sycl::handler& cgh) {
        auto accBuffer = syclBuffer.get_access<cl::sycl::access::mode::read>();
        auto accPColData = b_pColData.get_access<cl::sycl::access::mode::read_write>();

        cgh.parallel_for<class shapeDrawPixelModeNomal>(cl::sycl::range<1>{buffer.size()}, [=](cl::sycl::id<1> i) {
            accPColData[cl::sycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                        (unsigned long) accBuffer[i].x + pos.x}] = pge->getFuncPixelMode()(
                    accBuffer[i].x, accBuffer[i].y, accBuffer[i].color,
                    accPColData[cl::sycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                                (unsigned long) accBuffer[i].x + pos.x}]);
        });
    });
    return true;
}

bool olc::sycl::Shape::drawAlpha(olc::sycl::PixelGameEngine* pge, const vi2d& pos, cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const
{
    float blendFactor = pge->getBlendFactor();
    syclQueue.submit([&](cl::sycl::handler& cgh) {
        auto accBuffer = syclBuffer.get_access<cl::sycl::access::mode::read>();
        auto accPColData = b_pColData.get_access<cl::sycl::access::mode::read_write>();

        cgh.parallel_for<class shapeDrawPixelModeNomal>(cl::sycl::range<1>{buffer.size()}, [=](cl::sycl::id<1> i) {
            Pixel pColData = accPColData[cl::sycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                                         (unsigned long) accBuffer[i].x + pos.x}];
            accPColData[cl::sycl::id<2>{(unsigned long) accBuffer[i].y + pos.y,
                                        (unsigned long) accBuffer[i].x + pos.x}] = calcPixelColorWithAlpha(
                    blendFactor,
                    accBuffer[i].color,
                    pColData);
        });
    });
    return true;
}

olc::Pixel olc::sycl::Shape::calcPixelColorWithAlpha(float blendFactor, const olc::Pixel& nColor, const olc::Pixel& pColor)
{
    float a = (float) (nColor.a / 255.0f) * blendFactor;
    float c = 1.0f - a;
    float r = a * (float) nColor.r + c * (float) pColor.r;
    float g = a * (float) nColor.g + c * (float) pColor.g;
    float b = a * (float) nColor.b + c * (float) pColor.b;
    return olc::Pixel{(unsigned char) r, (unsigned char) g, (unsigned char) b};
}

bool olc::sycl::Shape::draw(olc::PixelGameEngine* pge, int x, int y) const
{
    return draw(pge, {x, y});
}

void olc::sycl::Shape::reserve(int size)
{
    buffer.reserve(size);
}

int olc::sycl::Shape::capacity() const
{
    return buffer.capacity();
}

int olc::sycl::Shape::size() const
{
    return buffer.size();
}

olc::sycl::Shape::Shape(int initialSize) : olc::sycl::abs::Shape(initialSize)
{

}
