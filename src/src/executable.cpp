
#include "Foo.hpp"
#include "FSM.hpp"
#include <iostream>



int main()
{
    Door door;

    door.handle(OpenEvent{});
    door.handle(CloseEvent{});

    door.handle(CloseEvent{});
    door.handle(OpenEvent{});

    return 0;
}