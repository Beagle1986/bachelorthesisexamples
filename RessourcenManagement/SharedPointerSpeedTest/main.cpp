#include <iostream>
#include <chrono>
#include <memory>

static const long COUNT {100000000};
static const int MEASUREMENTS {10};
static const int DEFAULT_INIT {42};

int main()
{
    using namespace std::chrono;

    milliseconds overallDuration{};
    for(int i{0}; i < MEASUREMENTS; ++i)
    {
        auto t1{high_resolution_clock::now()};

        for (long j{0}; j < COUNT; ++j)
        {
            //Jeweiligen Test einkommentieren
            //int* tmp{new int(DEFAULT_INIT)};
            //delete tmp;
            std::shared_ptr<int> tmp(new int(DEFAULT_INIT));
            //std::shared_ptr<int> tmp{std::make_shared<int>(DEFAULT_INIT)};
            //std::unique_ptr<int> tmp(new int(DEFAULT_INIT));
            //std::unique_ptr<int> tmp(std::make_unique<int>(DEFAULT_INIT));
        }
        auto t2{high_resolution_clock::now()};

        overallDuration += duration_cast<milliseconds>(t2 - t1);
    }

    std::cout << (overallDuration.count() / MEASUREMENTS) << " ms";
    return 0;
}