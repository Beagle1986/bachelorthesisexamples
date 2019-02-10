#include <iostream>
#include <unordered_map>
#include <memory>
#include <set>
#include <vector>

void function(int pitch) {
}

void function(std::set<int> const& v) {
}

void function(void (*play_function)(int)) {
}

void play_tone(int i) {}
void play_tone(int i, double length) {}

using PlayAToneFunctionType = void (*)(int);

template<typename T>
void forward_function(T&& value)
{
    function(std::forward<T>(value));
}

class Music {
public:
    static const int CONCERT_PITCH = 440;
};

const int Music::CONCERT_PITCH; //Weiterleitung nur mit Redefinition möglich

int main()
{
    //Beispiel Initalisierungslisten mit { }
    //forward_function({42,666,667}); //Nicht möglich /+\label{line:pfinitlist}+/
    auto init_list = {42,666,667};
    forward_function(init_list);

    //Beispiel Weiterleitung von deklarierten
    //integralen static const Membern
    forward_function(Music::CONCERT_PITCH);

    //Beispiel Funktions-und Templatenamen
    //forward_function(play_tone); //Nicht möglich

    PlayAToneFunctionType myPlayTone = play_tone;
    forward_function(myPlayTone);

    //Beispiel Bitfeld
    struct bitField {
        std::int64_t low:32, high:32;
    };

    bitField field;

    //forward_function(field.low); //Nicht möglich

    auto length {static_cast<int32_t>(field.low)};
    forward_function(length);

    return 0;
}