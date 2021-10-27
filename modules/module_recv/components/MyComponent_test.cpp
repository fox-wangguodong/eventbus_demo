//
// Created by fox on 2021/10/21.
//

#include <gtest/gtest.h>
#include <memory>
#include "MyComponent.h"

class MyComponentTest : public ::testing::Test {
public:
    static void SetUpTestCase() {
        eventLoop = std::make_shared<muduo::net::EventLoop>();
        myComponent = std::make_shared<MyComponent>(eventLoop);
    }
    static void TearDownTestCase() {

    }
    static std::shared_ptr<muduo::net::EventLoop> eventLoop;
    static std::shared_ptr<MyComponent> myComponent;
protected:
    void SetUp() override {

    }
    void TearDown() override {

    }
};

std::shared_ptr<muduo::net::EventLoop> MyComponentTest::eventLoop;
std::shared_ptr<MyComponent> MyComponentTest::myComponent;

TEST_F(MyComponentTest, test1){

    EXPECT_EQ(myComponent->add(1, 2),3);

    ASSERT_EQ(myComponent->sum,3);
}

TEST_F(MyComponentTest, test2){

    EXPECT_EQ(myComponent->add(1, 3),4);

    ASSERT_EQ(myComponent->sum,7);
}
