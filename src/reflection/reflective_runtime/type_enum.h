#ifndef TYPE_ENUM_H_
#define TYPE_ENUM_H_

#include "reflective_core.h"

namespace reflective {

class type_enum
{
public:
    type_enum() = delete;
    type_enum(const type_enum& orig) = delete;
    virtual ~type_enum();

    type_enum& operator=(const type_enum& rhs) = delete;

protected:

private:

};

}

#endif
