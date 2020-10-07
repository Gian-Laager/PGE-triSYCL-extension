#include "pch.h"

#define OLC_PGE_APPLICATION

#include <olcPixelGameEngine.h>

constexpr long sizeW = 256;
constexpr long sizeH = 240;
//olc::Pixel* screenBuffer;

class Test3d : public olc::PixelGameEngine
{
private:
    static void setSpriteBufferData(olc::Sprite* sprite, olc::Pixel* buffer, int width, int height)
    {
        sprite->pColData = buffer;
        sprite->width = width;
        sprite->height = height;
    }

public:
//    olc::Pixel* screenBuffer;
    olc::Sprite* screenSprite;
    olc::Decal* screenDecal;

    Test3d()
    {
        sAppName = "3d Test with SYCL";
    }

    bool OnUserCreate() override
    {
        screenSprite = new olc::Sprite();
        screenDecal = new olc::Decal(screenSprite);
        screenBuffer = cl::sycl::buffer<olc::Pixel, 2>(cl::sycl::range<2>(sizeW, sizeH));
        queue = cl::sycl::queue(device_selector);
        return true;
    }

    std::vector<float> times;
    cl::sycl::buffer<olc::Pixel, 2> screenBuffer;
    cl::sycl::default_selector device_selector;
    cl::sycl::queue queue;

    bool OnUserUpdate(float fElapsedTime) override
    {
        queue.submit([&](cl::sycl::handler& cgh) {
            auto screenBuffer_acc = screenBuffer.get_access<cl::sycl::access::mode::write>(cgh);
            cgh.parallel_for(cl::sycl::range<2>(sizeW, sizeH), [=](cl::sycl::id<2> index) {
                screenBuffer_acc[index] = olc::Pixel(((int) rand() >> 8) | 0xff000000);
            });
        });
        queue.wait();
        auto screenBuffer_acc = screenBuffer.get_access<cl::sycl::access::mode::read>();
        setSpriteBufferData(screenSprite, screenBuffer_acc.get_pointer(), ScreenWidth(), ScreenHeight());

        screenDecal->Update();
        DrawDecal({0, 0}, screenDecal);

        std::cout << "fElapsedTime = " << fElapsedTime << std::endl;
        times.push_back(fElapsedTime);

        return true;
    }

    bool OnUserDestroy() override
    {
        float totalTime = 0.f;
        for (auto&& time : times)
            totalTime += time;

        std::cout << "Average time = " << totalTime / (float) times.size()
                  << std::endl; // = 0.00553194 Debug, 0.00159081 Release
        return true;
    }
};

int main()
{
    auto* test3d = new Test3d;
    if (test3d->Construct(256, 240, 2, 2))
        test3d->Start();
    return 0;
}
