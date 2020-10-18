#include "Texture.h"

olc::sycl::Texture::Texture()
{

}

olc::sycl::Texture::Texture(const std::string& sImageFile, olc::ResourcePack* pack) : olc::sycl::abs::Texture(sImageFile, pack) {}

olc::sycl::Texture::Texture(int32_t w, int32_t h) : olc::sycl::abs::Texture(w, h) {}

cl::sycl::buffer<olc::Pixel, 2> olc::sycl::Texture::getBuffer() const
{
    return buffer;
}
