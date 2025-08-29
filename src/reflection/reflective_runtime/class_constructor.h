#ifndef CLASS_CONSTRUCTOR_H_
#define CLASS_CONSTRUCTOR_H_

#include "reflective_core.h"

#include "declaration.h"
#include "type.h"

namespace reflective {

class class_constructor : public declaration
{
public:
    using parameters_type = std::unordered_map<std::string, const type*>;

    class_constructor(const class_constructor& orig) = delete;
    virtual ~class_constructor();

    class_constructor& operator=(const class_constructor& rhs) = delete;

    const parameters_type& parameter_types() const
    {
        return parameters_;
    }

    parameters_type& parameter_types()
    {
        return parameters_;
    }

protected:
    class_constructor();
    
    parameters_type parameters_;

private:
    friend class database;
};

}

#endif
