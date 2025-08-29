#ifndef CLASS_CONSTRUCTOR_SPEC_H_
#define CLASS_CONSTRUCTOR_SPEC_H_

#include "reflective_core.h"

#include "class_constructor.h"

namespace reflective {

template <typename T>
class class_constructor_spec : public class_constructor
{
public:
    class_constructor_spec()
        : class_constructor()
    {
        
    }
    
    class_constructor_spec(const class_constructor_spec& orig) = delete;

    virtual ~class_constructor_spec()
    {

    }

    class_constructor_spec& operator=(const class_constructor_spec& rhs) = delete;

protected:

private:

};

}

#endif
