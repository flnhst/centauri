#include "entity.h"

#include <print>

entity::entity()
{
    std::println("entity::entity(): called.");
}

entity::entity(std::uint64_t blaap, int woep, float waap, const std::string& something)
{
}

entity::~entity()
{
    std::println("entity::~entity(): called.");
}

void entity::update()
{
    std::println("entity::update(): called.");
}

