#include "r_class_template.h"

namespace reflective {

r_class_template::r_class_template(const std::string &name)
    : r_class(name)
{
    is_template_ = true;
}

r_class_template::~r_class_template()
{

}

reflective::any r_class_template::create_instance() const
{
    return reflective::any();
}

void r_class_template::destroy_instance(any &instance) const
{

}

reflective::any r_class_template::create_instance(std::vector<reflective::any_ref> &parameters) const
{
    return reflective::any();
}

}
