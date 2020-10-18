#ifndef PGE_TRISYCL_EXTENSION_SHAPE_H
#define PGE_TRISYCL_EXTENSION_SHAPE_H

#include "pch.h"
#include "abstract.h"

namespace olc::sycl
{
    class Shape : public olc::sycl::abs::Shape
    {
    protected:
        bool drawNormal(const olc::vi2d& pos, cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const override;

        bool drawMask(const olc::vi2d& pos,
                      cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const override;

        bool drawAlpha(olc::sycl::PixelGameEngine* pge, const olc::vi2d& pos,
                       cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const override;

        bool drawCustom(olc::sycl::PixelGameEngine* pge, const olc::vi2d& pos,
                        cl::sycl::buffer<olc::Pixel, 2>& b_pColData) const override;

    public:
        Shape();

        Shape(int initialSize);

        void setPoint(int x, int y, const olc::Pixel& color) override;

        void setPoint(const Point& point) override;

        Point getPoint(int x, int y) const override;

        std::vector<Point> getBuffer() const override;

        bool draw(olc::PixelGameEngine* pge, const olc::vi2d& pos) const override;

        bool draw(olc::PixelGameEngine* pge, int x, int y) const override;

        static olc::Pixel
        calcPixelColorWithAlpha(float blendFactor, const olc::Pixel& nColor, const olc::Pixel& pColor);

        void reserve(int size) override;

        int capacity() const override;
        int size() const override;
    };
}

#endif //PGE_TRISYCL_EXTENSION_SHAPE_H
