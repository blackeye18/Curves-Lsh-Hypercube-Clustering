#include <iostream>
#include<gtest/gtest.h>
#include "program.cpp"
using namespace std;

TEST(DFD_TEST ,correct_dist) 
    { 
    ASSERT_EQ(3, dfd({1,2,3},{4,5,6},3,3));
    }

TEST(Filter_Test ,correct_filtering) 
    {
    vector<double>* temp = new vector<double>{100,120,200};
    Mean_filter(temp,0);
    ASSERT_EQ(3,temp->size());

    temp->clear();
    delete temp;

    vector<double>* temp2 = new vector<double>{1,2,3,7};
    Mean_filter(temp2,0);
    ASSERT_EQ(2,temp2->size());

    temp2->clear();
    delete temp2;
    }
int main(int argc, char **argv) 
    {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }