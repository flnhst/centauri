#ifndef R_CLASS_SPEC_H_
#define R_CLASS_SPEC_H_

#include "reflective_core.h"

#include "r_class.h"

namespace reflective {

template <typename T>
class r_class_spec : public r_class
{
public:
    r_class_spec() = delete;

    r_class_spec(const r_class_spec& orig) = delete;

    virtual ~r_class_spec()
    {

    }

    r_class_spec& operator=(const r_class_spec& rhs) = delete;

protected:
    r_class_spec(const std::string name) : r_class(name) {};

private:

};

}

#endif
