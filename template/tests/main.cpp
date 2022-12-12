
#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include <add.h>

using namespace std;

int main(int argc,char** argv)
{
    int test = add(1,2);
    std::cout << "test ok ! " << test << std::endl;
}
