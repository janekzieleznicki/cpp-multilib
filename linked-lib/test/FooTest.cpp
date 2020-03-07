#include <gmock/gmock.h>
#include "Foo.hpp"

TEST(FooTest, False){
    EXPECT_TRUE(foo::Foo{});
}