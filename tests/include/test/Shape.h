#ifndef PGE_TRISYCL_EXTENSION_TEST_SHAPE_H
#define PGE_TRISYCL_EXTENSION_TEST_SHAPE_H

#include "test/pch.h"

#include "olcSYCL.h"

namespace olc::sycl::test
{
    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> end;

    public:
        const char* name;
        std::function<void()> callback;

        Timer(const char* name, std::function<void()> callback = []() {}) : start(
                std::chrono::high_resolution_clock::now()),
                                                                            name(name),
                                                                            callback(std::move(callback)) {}

        ~Timer()
        {
            end = std::chrono::high_resolution_clock::now();
            auto mic_start = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
            auto mic_end = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
            std::cout << "Timer '" << name << "' destructed after: " << (mic_end - mic_start) * 0.001 << " milliseconds"
                      << std::endl;

            callback();
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> getStart() const { return start; }

        std::chrono::time_point<std::chrono::high_resolution_clock> getEnd() const { return end; }
    };

    class ShapeTestPGE : public olc::PixelGameEngine
    {
    public:
        ShapeTestPGE();
        bool OnUserCreate() override;

        bool OnUserUpdate(float fElapsedTime) override;
    };

    class ShapeTest : public testing::Test
    {
    public:
        static bool arePointsEqual(const olc::sycl::Shape::Point& p1, const olc::sycl::Shape::Point& p2);

        static olc::sycl::Shape::Point constructPoint(int x, int y);

        static std::vector<olc::sycl::Shape::Point>& getBuffer(const olc::sycl::Shape* s);

    public:
        void SetUp() override;

        void TearDown() override;

        olc::sycl::Shape shape;
        ShapeTestPGE pge;
    };
}

#endif //PGE_TRISYCL_EXTENSION_TEST_SHAPE_H
