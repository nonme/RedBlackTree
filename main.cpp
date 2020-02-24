#include <iostream>
#include "BST.h"
#include "TestRunner.h"

void TestBST();

int main() {
    {
        test::TestRunner tr;
        tr.disable();
        RUN_TEST(tr, TestBST);
    }
    {
        RedBlackBST<int, int> tree;
        int number = 0;
        std::cin >> number;
        while (number != 0) {
            tree.insert(number, number);
            std::cin >> number;
        }
        tree.print(std::cout);
    }
}

void TestBST() {
    RedBlackBST<int, int> tree;
    ASSERT_EQ(tree.size(), 0);
    tree.insert(1, 1);
    tree.insert(2, 1);
    ASSERT_EQ(tree.size(), 2);
    ASSERT_EQ(tree.get(1), 1);
    tree.insert(1, 3);
    ASSERT_EQ(tree.get(1), 3);
    ASSERT_THROWS(([]() -> void {
        RedBlackBST<int, int> throwsBST;
        throwsBST.insert(1, 1);
        throwsBST.remove(1);
        throwsBST.get(1);
    }), std::invalid_argument(""));
    tree.remove(1);
    tree.clear();
    ASSERT_EQ(tree.size(), 0);
}