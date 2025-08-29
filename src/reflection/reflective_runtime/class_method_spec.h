#ifndef CLASS_METHOD_SPEC_H_
#define CLASS_METHOD_SPEC_H_

#include "reflective_core.h"

#include "class_method.h"

namespace reflective {

class type;

template <typename T>
class class_method_spec : public class_method
{
public:
    class_method_spec(const std::string& name)
        : class_method(name)
    {

    }
    
    class_method_spec() = delete;

    class_method_spec(const class_method_spec& orig) = delete;

    virtual ~class_method_spec()
    {

    }

    class_method_spec& operator=(const class_method_spec& rhs) = delete;



protected:

private:

};

}

#endif
