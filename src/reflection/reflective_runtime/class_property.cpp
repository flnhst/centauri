#include "class_property.h"

namespace reflective {

class_property::class_property(const std::string& name, const type* type_ptr, std::uint64_t offset)
    : declaration(name, declaration_class::class_property),
      offset_(offset),
      type_(type_ptr)
{

}

class_property::~class_property() = default;

}
