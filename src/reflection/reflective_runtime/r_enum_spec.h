#ifndef R_ENUM_SPEC_H_
#define R_ENUM_SPEC_H_

#include "reflective_core.h"

#include "r_enum.h"

namespace reflective {

template <typename T>
class r_enum_spec : public r_enum
{
public:
    r_enum_spec(const r_enum_spec& orig) = delete;

    virtual ~r_enum_spec()
    {

    }

    r_enum_spec& operator=(const r_enum_spec& rhs) = delete;

protected:
    r_enum_spec(const std::string name) : r_enum(name) {};

private:

};

}

#endif
