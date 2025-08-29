#include "entity.h"

#include <print>

entity::entity()
{
    std::println("entity::entity(): called.");
}

entity::~entity()
{
    std::println("entity::~entity(): called.");
}

float entity::test(int a, int c, entity* entity_ptr)
{
    return 0.0f;
}

