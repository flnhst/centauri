#ifndef TYPE_SPEC_H_
#define TYPE_SPEC_H_

#include "reflective_core.h"

#include "type.h"

namespace reflective {

template <typename T>
class type_spec : public type
{
public:
    type_spec(const std::string& type_str, std::uint64_t hash, const type* inner_type, type_kind kind, std::string name, std::vector<const type*> template_args)
        : type(type_str, hash)
    {
        is_pointer_ = std::is_pointer_v<T>;
        is_reference_ = std::is_reference_v<T>;
        is_const_ = std::is_const_v<T>;
        inner_type_ = inner_type;
        kind_ = kind;
        name_ = name;
        template_arguments_ = template_args;
    }

    type_spec(const type_spec& orig) = delete;

    virtual ~type_spec()
    {

    }

    type_spec& operator=(const type_spec& rhs) = delete;

    virtual const r_class* get_class() const
    {
        return reflective::get_class<std::remove_pointer_t<std::remove_cv_t<T>>>();
    }

protected:

private:

};

}

#endif
