# polyvar
polymorphism with value semantics (based on std::variant)

Example:

```cpp
#include "polyvar.hpp"
#include <iostream>

struct A {
    char do_something() { return 'A'; }
    int do_something_else(int x) { return x; }
};

struct B {
    char do_something() { return 'B'; }
    int do_something_else(int x) { return x + 1; }
};

struct C {
    char do_something() { return 'C'; }
    int do_something_else(int x) { return x + 2; }
};

// Define a variant template with self-visiting member
// functions to emulate a base class
DEFINE_POLYVAR(my_var,
    (do_something)
    (do_something_else)
);

using var = my_var<A, B, C>;

int main () {

    var v = A{};
    std::cout << v.do_something() << std::endl;
    std::cout << v.do_something_else(0) << std::endl;

    v = B{};
    std::cout << v.do_something() << std::endl;
    std::cout << v.do_something_else(1) << std::endl;

    v = C{};
    std::cout << v.do_something() << std::endl;
    std::cout << v.do_something_else(2) << std::endl;
}
    
```
