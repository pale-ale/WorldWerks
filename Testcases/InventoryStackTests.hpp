#include <catch2/catch.hpp>
#include "../Data/InventoryStack.hpp"


TEST_CASE("Transfer InventoryStack w/ different ItemIDs", "[InvStackDiffIDs]"){
    InventoryItem i1(1);
    InventoryItem i2(2);
    i1.maxstack = 5;
    i2.maxstack = 4;
    InventoryStack s1(i1, 4);
    InventoryStack s2(i2, 3);
    s1.add_stack(&s2);
    REQUIRE( (s1.itemCount == 4 && s2.itemCount == 3) );
}

TEST_CASE("Transfer InventoryStack w/ same ItemIDs", "[InvStackSameIDs]"){
    InventoryItem i1(1);
    InventoryItem i2(1);
    i1.maxstack = 5;
    i2.maxstack = 4;
    InventoryStack s1(i1, 4);
    InventoryStack s2(i2, 3);
    s1.add_stack(&s2);
    REQUIRE( (s1.itemCount == 5 && s2.itemCount == 2) );
}

TEST_CASE("Transfer InventoryStack non-negative", "[InvStackNonNeg]"){
    InventoryItem i1(1);
    InventoryItem i2(1);
    i1.maxstack = 10;
    i2.maxstack = 10;
    InventoryStack s1(i1, 4);
    InventoryStack s2(i2, 3);
    s1.add_stack(&s2);
    REQUIRE( (s1.itemCount == 7 && s2.itemCount == 0) );
}
