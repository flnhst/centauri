#include "type.h"

#include "database.h"

namespace reflective {

type::~type()
{

}

const type* type::get_first_inner_type_of_kind(type_kind kind) const
{
    const auto* current_type = inner_type_;

    while (current_type != nullptr)
    {
        if (current_type->kind() == kind)
        {
            return current_type;
        }

        current_type = current_type->inner_type_;
    }

    return nullptr;
}

type::type(const std::string& type_str, std::uint64_t hash)
    : type_str_(type_str)
    , hash_(hash)
{
    database::get_instance().add_type_(type_str_, hash_, this);
}

}
