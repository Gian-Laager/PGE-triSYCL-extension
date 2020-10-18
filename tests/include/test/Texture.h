#ifndef PGE_TRISYCL_EXTENSION_TEST_TEXTURE_H
#define PGE_TRISYCL_EXTENSION_TEST_TEXTURE_H

#include "test/pch.h"

#include "olcSYCL.h"

namespace olc::sycl::test
{
    class TextureTest : public testing::Test
    {
    protected:
        olc::sycl::Texture texture;
        void SetUp() override;
    };
}

#endif //PGE_TRISYCL_EXTENSION_TEST_TEXTURE_H
