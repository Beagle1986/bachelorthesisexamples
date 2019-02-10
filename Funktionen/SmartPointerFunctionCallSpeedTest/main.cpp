#include <iostream>
#include <chrono>
#include <memory>

//void func(std::shared_ptr<int> ptr) {} //Für shared per Wert
void func(std::shared_ptr<int>const& ptr) {} //Für shared per Referenz
void func(int *ptr) {}

static const long COUNT {100000000};
static const int MEASUREMENTS {10};
static const int DEFAULT_INIT {42};

int main()
{
    using namespace std::chrono;

    auto shared {std::make_shared<int>(DEFAULT_INIT)};
    auto unique {std::make_unique<int>(DEFAULT_INIT)};
    milliseconds overallDuration{};
    for(int i{0}; i < MEASUREMENTS; ++i)
    {
        auto t1{high_resolution_clock::now()};
        for (long i{0}; i < COUNT; ++i)
        {
            //Jeweiligen Test einkommentieren
            func(shared); //shared_ptr per Wert
            //func(shared.get()); //per Raw-Pointer aus shared
            //func(unique.get()); //per Raw-Pointer aus shared
        }
        auto t2{high_resolution_clock::now()};
        overallDuration += duration_cast<milliseconds>(t2 - t1);
    }

    std::cout << (overallDuration.count() / MEASUREMENTS) << " ms";
    return 0;
}