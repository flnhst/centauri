#include "r_enum.h"

#include "database.h"

namespace reflective {

r_enum::r_enum(const std::string& name)
    : declaration(name, declaration_class::r_enum)
{
    database::get_instance().enumerators_.emplace(name, this);
}

r_enum::~r_enum()
{

}

}
