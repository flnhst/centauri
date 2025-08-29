#include "attribute.h"

namespace reflective {

attribute::attribute(const std::string& name)
    : name_(name)
{
}

attribute::attribute(const std::string& name, const key_values_type& key_values)
    : name_(name), key_values_(key_values)
{
}

attribute::~attribute()
{

}

}
