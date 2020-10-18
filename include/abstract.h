#ifndef PGE_TRISYCL_EXTENSION_ABSTRACT_H
#define PGE_TRISYCL_EXTENSION_ABSTRACT_H

#include "pch.h"
#include "abstract/Shape.h"
#include "abstract/Texture.h"

namespace olc::sycl
{
    class PixelGameEngine : public olc::PixelGameEngine
    {
    public:
        float getBlendFactor() const
        {
            return *(float*) ((long) this + 44);
        }

        std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)>
        getFuncPixelMode() const
        {
            return *(std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)>*) (
                    (long) this + 240);
        }
    };
}

#endif //PGE_TRISYCL_EXTENSION_ABSTRACT_H
