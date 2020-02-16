#include "Foo.hpp"
namespace foo{
    class Foo2: public Foo{
        using Foo::Foo;
    };
}