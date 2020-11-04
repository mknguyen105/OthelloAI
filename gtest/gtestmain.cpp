// gtestmain.cpp
//
// launch Google Test and run any unit tests that you wrote in source files in the "gtest" directory.


#include <gtest/gtest.h>


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

