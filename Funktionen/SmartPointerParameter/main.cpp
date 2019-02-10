#include <iostream>
#include <memory>

class Example {
    public:
        explicit Example(std::string t) : text{std::move(t)} {
            std::cout << "Konstruktor Example Aufruf" << '\n';
        }

        void print() const { std::cout << text << '\n';}

        void set_text(std::string const &text) {
            this->text = text;
        }

    private:
        std::string text;
};

void unique_by_value(std::unique_ptr<Example> sample) {
    sample->set_text("unique_by_value");
    sample->print();
}

//Erstellt shared pointer Kopie und erhöht Referenzzähler
//durch Übergabe per Wert
void shared_by_value(std::shared_ptr<Example> sample) {
    sample->set_text("shared_by_value");
    sample->print();
    std::cout << "Referenzzähler sample = " << sample.use_count() << '\n';
}

//Erhöht nicht Referenzzähler
void shared_by_ref(std::shared_ptr<Example> const &sample) {
    sample->set_text("shared_const_ref");
    std::cout << "Referenzzähler sample = " << sample.use_count() << '\n';
    sample->print();
}

//Für nicht optionale Parameter. Kein Aufruf von delete möglich
//da per Referenz
void raw_ref(Example &sample) {
    sample.set_text("raw_ref");
    sample.print();
}

//Für Optionale Parameter. Zeigt keinen Besitz
//somit klar das kein delete aufgerufen werden darf.
void raw_pointer(Example *sample) {
    sample->set_text("raw_pointer");
    sample->print();
}


//Für nicht optionale Eingangsparameter.
//* zeigt kein Ownership an
void raw_ref_in(Example const &sample) {
    sample.print();
}

//Für Optionale Eingangsparameter.
//* zeigt kein Ownership an
void raw_pointer_in(Example const *sample) {
    sample->print();
}

int main()
{
    auto up { std::make_unique<Example>("Test")};
    unique_by_value(std::move(up));

    auto sp {std::make_shared<Example>("Test")};
    std::cout << "Referenzzähler sp = " << sp.use_count() << '\n';
    shared_by_value(sp);
    std::cout << "Referenzzähler sp = " << sp.use_count() << '\n';
    shared_by_ref(sp);
    std::cout << "Referenzzähler sp = " << sp.use_count() << '\n';
    raw_ref(*sp.get());
    std::cout << "Referenzzähler sp = " << sp.use_count() << '\n';
    raw_pointer(sp.get());
    std::cout << "Referenzzähler sp = " << sp.use_count() << '\n';
    return 0;
}
