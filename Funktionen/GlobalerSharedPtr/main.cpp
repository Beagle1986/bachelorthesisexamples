#include <iostream>
#include <memory>

//Herb Sutter Back to the Basics of Modern C++ Style
//Rule Don't dereference a non local possibly aliased shared_ptr

struct Widget {
    void print() { std::cout << "Widget aufgerufen" << '\n'; }
};

std::shared_ptr<Widget> g_p { std::make_shared<Widget>() };

void g() {
    g_p = std::make_shared<Widget>(); //Funktion g setzt den shared_ptr neu /+\label{line:resetgp}+/
}

void doSomething(Widget& widget) {
    g();
    widget.print(); //ohne lokale Kopie widget hier nicht mehr Gültig /+\label{line:widgetcall}+/
}

int main() {
    //Anti-Pattern
    //doSomething(*g_p); //g_p ist nicht lokal /+\label{line:nolocalcopy}+/

    //Besser lokale Kopie erstellen
    //die Referenzzähler um 1 erhöht
    auto p_localCopy = g_p;
    doSomething(*p_localCopy); //lokale Kopie für Funktionsaufruf/+\label{line:localcopy}+/
    p_localCopy->print(); //Aufrufe nach Funktionsaufruf ebenfalls mit Kopie /+\label{line:localcopycall}+/

    return 0;
}