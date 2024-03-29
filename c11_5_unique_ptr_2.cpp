#include <iostream>
#include <memory>

// unique can not be copied but moved

template <typename T, typename... Args> std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Foo {
    Foo() { std::cout << "Foo::Foo" << std::endl; }
    ~Foo() { std::cout << "Foo::~Foo" << std::endl; }
    void foo() { std::cout << "Foo::foo" << std::endl; }
};

void f(const Foo&) { std::cout << "f(const Foo&)" << std::endl; }

int main() {
    // std::unique_ptr<Foo> p1(new Foo());    // Correct
    // std::unique_ptr<Foo> p1(make_unique<Foo>());  // better
    auto p1 = make_unique<Foo>(); // best
    // p1 is not empty, prints
    if (p1)
        p1->foo();
    {
        std::unique_ptr<Foo> p2(std::move(p1));
        // p2 is not empty, prints
        f(*p2);
        // p2 is not empty, prints
        if (p2)
            p2->foo();
        // p1 is empty, no prints
        if (p1)
            p1->foo();
        p1 = std::move(p2);
        // p2 is empty, no prints
        if (p2)
            p2->foo();
        std::cout << "p2 was destroyed" << std::endl;
    }
    // p1 is not empty, prints
    if (p1)
        p1->foo();
    // Foo instance will be destroyed when leaving the scope
}