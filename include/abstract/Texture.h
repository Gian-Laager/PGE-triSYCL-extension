#ifndef PGE_TRISYCL_EXTENSION_ABSTRACT_TEXTURE_H
#define PGE_TRISYCL_EXTENSION_ABSTRACT_TEXTURE_H

#include "pch.h"

namespace olc::sycl::abs
{
    class Texture : public olc::Sprite
    {
    protected:
        cl::sycl::buffer<olc::Pixel, 2> buffer;

        void updateSyclBuffer();

    public:
        Texture();

        Texture(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);

        Texture(int32_t w, int32_t h);

        Texture(const olc::Sprite&) = delete;

        virtual cl::sycl::buffer<olc::Pixel, 2> getBuffer() const = 0;
    };
}

#endif //PGE_TRISYCL_EXTENSION_ABSTRACT_TEXTURE_H
