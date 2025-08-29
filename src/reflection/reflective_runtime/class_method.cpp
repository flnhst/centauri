#include "class_method.h"

namespace reflective {

class_method::~class_method() = default;

class_method::class_method(const std::string& name)
    : declaration(name, declaration_class::class_method), return_type_(nullptr)
{
}

}
