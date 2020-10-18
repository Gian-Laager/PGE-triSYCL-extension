#include "test/Texture.h"

using namespace olc::sycl::test;

void olc::sycl::test::TextureTest::SetUp()
{
    texture = olc::sycl::Texture(std::string("../tests/res/pics/mario.png"));
}

TEST_F(TextureTest, WillColDataBeInitializedRight)
{
    auto colorBufferTexture = texture.getBuffer().get_access<cl::sycl::access::mode::read>();
    olc::Sprite s = olc::Sprite(std::string("../tests/res/pics/mario.png"));
    olc::Pixel* colorBufferSprite = s.pColData;

    for (int i = 0; i < s.width; i++)
        for (int j = 0; j < s.height; j++)
            ASSERT_EQ(colorBufferSprite[i + j * s.width],
                      (colorBufferTexture[cl::sycl::id<2>{(unsigned long) i, (unsigned long) j}]));
}

TEST_F(TextureTest, WillDefaultConstructorInitVariablesRight)
{
    olc::sycl::Texture t{};
    ASSERT_EQ(t.pColData, nullptr);
    ASSERT_EQ(t.width, 0);
    ASSERT_EQ(t.height, 0);
}

TEST_F(TextureTest, WillWidthHeightConstructorInitVariablesRight)
{
    int w = 10;
    int h = 20;
    olc::sycl::Texture t{w, h};
    ASSERT_NE(t.pColData, nullptr);
    ASSERT_EQ(t.width, w);
    ASSERT_EQ(t.height, h);
}