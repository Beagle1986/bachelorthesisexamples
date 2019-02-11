#include <iostream>
#include <memory>
#include <algorithm>
#include <list>
#include <vector>
#include <mutex>


//Observer Pattern mit Pull Ansatz

class IObserver
{
    public:
        virtual ~IObserver() = default;
        virtual void notify() = 0;
};

class ISubject
{
    public:
        virtual void subscribe(std::weak_ptr<IObserver> observer) = 0;
        virtual void unsubscribe(std::weak_ptr<IObserver> observer) = 0;
        virtual void notifyObservers() = 0;
        virtual ~ISubject() = default;
};

class Subject : public ISubject
{
public:
    Subject() = default;
    Subject(Subject const &) = delete;
    Subject& operator=(Subject const &) = delete;
    Subject(Subject &&) noexcept = default;
    Subject& operator=(Subject &&) noexcept = default;

    // virtual ~Subject() = default;

    ~Subject() override{
        std::cout << "Destruktor Subject aufgerufen" << '\n';
    }

    void subscribe(std::weak_ptr<IObserver> observer) override
    {
        observers.emplace_back(std::move(observer));
    }
    void unsubscribe(std::weak_ptr<IObserver> observer) override
    {
        observers.erase(
                std::remove_if(
                        observers.begin(),
                        observers.end(),
                        [&observer](std::weak_ptr<IObserver> const& o)
                        {
                            return o.expired() || o.lock() == observer.lock();
                        }
                ),
                observers.end()
        );
    }

protected:
    void notifyObservers() override
    {
        auto it {std::begin(observers)};

        while (it != observers.end())
        {
            if ( !it->expired() )
            {
                auto observer {it->lock()};
                observer->notify();
                ++it;
            }
            else
            {
                it = observers.erase(it);
            }
        }
    }

private:
    std::vector<std::weak_ptr<IObserver>> observers;
};

class ObservableSubject : public Subject
{
    public:
        void set_state(int newState)
        {
            state = newState;
            notifyObservers();
        }

        int get_state()
        {
            return state;
        }

    private:
        int state{0};
};

class ConcreteObserver : public IObserver
{
public:
    explicit ConcreteObserver(std::shared_ptr<ObservableSubject> s) :
            subject{std::move(s)}
    {
    }

    ConcreteObserver(ConcreteObserver const &) = delete;
    ConcreteObserver& operator=(ConcreteObserver const &) = delete;
    ConcreteObserver(ConcreteObserver &&) noexcept = default;
    ConcreteObserver& operator=(ConcreteObserver &&) noexcept = default;

    void notify() override
    {
        std::cout << "Notify Observer aufgerufen. Status Subject: " << subject->get_state() << '\n';
    }

    ~ConcreteObserver() override {
        std::cout << "Destruktor Observer aufgerufen" << '\n';
    }

private:
    std::shared_ptr<ObservableSubject> subject;
};


int main()
{
    {
        auto subject{std::make_shared<ObservableSubject>()};
        auto observer1{std::make_shared<ConcreteObserver>(subject)};
        auto observer2{std::make_shared<ConcreteObserver>(subject)};

        subject->subscribe(observer1);
        subject->subscribe(observer2);

        subject->set_state(1);

        observer2.reset();

        subject->set_state(2);

        subject->unsubscribe(observer1);
        subject->unsubscribe(observer2);

        subject->set_state(2);
    }

    return 0;
}