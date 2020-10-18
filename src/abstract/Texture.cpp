#include "abstract/Texture.h"

olc::sycl::abs::Texture::Texture(const std::string& sImageFile, olc::ResourcePack* pack) : Sprite(sImageFile, pack)
{
    updateSyclBuffer();
}

olc::sycl::abs::Texture::Texture(int32_t w, int32_t h) : Sprite(w, h)
{
    updateSyclBuffer();
}

void olc::sycl::abs::Texture::updateSyclBuffer()
{
    buffer = cl::sycl::buffer<olc::Pixel, 2>(Sprite::pColData,
                                             cl::sycl::range<2>{
                                                     (unsigned long) Sprite::width,
                                                     (unsigned long) Sprite::height});
}

olc::sycl::abs::Texture::Texture()
{

}
