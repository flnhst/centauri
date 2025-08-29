#ifndef REFLECTIVE_R_CLASS_TEMPLATE_H
#define REFLECTIVE_R_CLASS_TEMPLATE_H

#include "reflective_core.h"

#include "r_class.h"

namespace reflective {

class r_class_template : public r_class
{
public:
    r_class_template() = delete;

    r_class_template(const r_class_template &orig) = delete;

    virtual ~r_class_template();

    r_class_template &operator=(const r_class_template &rhs) = delete;

    virtual reflective::any create_instance() const override;

    virtual void destroy_instance(reflective::any& instance) const override;

    virtual reflective::any create_instance(std::vector<reflective::any_ref>& parameters) const override;

protected:
    r_class_template(const std::string &name);
};

}

#endif //REFLECTIVE_R_CLASS_TEMPLATE_H
