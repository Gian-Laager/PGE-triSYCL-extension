#ifndef PGE_TRISYCL_EXTENSION_TEXTURE_H
#define PGE_TRISYCL_EXTENSION_TEXTURE_H

#include "pch.h"
#include "abstract.h"

namespace olc::sycl
{
    class Texture : public olc::sycl::abs::Texture
    {
    public:
        Texture();

        Texture(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);

        Texture(int32_t w, int32_t h);

        Texture(const olc::Sprite&) = delete;

        cl::sycl::buffer<olc::Pixel, 2> getBuffer() const override;
    };
}

#endif //PGE_TRISYCL_EXTENSION_TEXTURE_H
