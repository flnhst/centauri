#ifndef CLASS_PROPERTY_SPEC_H_
#define CLASS_PROPERTY_SPEC_H_

#include "reflective_core.h"

#include "class_property.h"

namespace reflective {

template <typename T>
class class_property_spec : public class_property
{
public:
    class_property_spec(const std::string& name, const type* type_ptr, std::uint64_t offset)
        : class_property(name, type_ptr, offset)
    {
        if constexpr (std::is_same_v<float, T>)
        {
            builtin_type_ = builtin_type::float_type;
        }
        else if constexpr (std::is_same_v<double, T>)
        {
            builtin_type_ = builtin_type::double_type;
        }
        else if constexpr (std::is_same_v<char, T>)
        {
            builtin_type_ = builtin_type::char_type;
        }
        else if constexpr (std::is_same_v<unsigned char, T>)
        {
            builtin_type_ = builtin_type::unsigned_char_type;
        }
        else if constexpr (std::is_same_v<int, T>)
        {
            builtin_type_ = builtin_type::int_type;
        }
        else if constexpr (std::is_same_v<unsigned int, T>)
        {
            builtin_type_ = builtin_type::unsigned_int_type;
        }
        else if constexpr (std::is_same_v<long, T>)
        {
            builtin_type_ = builtin_type::long_type;
        }
        else if constexpr (std::is_same_v<unsigned long, T>)
        {
            builtin_type_ = builtin_type::unsigned_long_type;
        }
        else if constexpr (std::is_same_v<long long, T>)
        {
            builtin_type_ = builtin_type::long_long_type;
        }
        else if constexpr (std::is_same_v<unsigned long long, T>)
        {
            builtin_type_ = builtin_type::unsigned_long_long_type;
        }
        else if constexpr (std::is_same_v<bool, T>)
        {
            builtin_type_ = builtin_type::bool_type;
        }
    }

    class_property_spec() = delete;

    class_property_spec(const class_property_spec& orig) = delete;

    virtual ~class_property_spec()
    {

    }

    class_property_spec& operator=(const class_property_spec& rhs) = delete;

protected:

private:

};

}

#endif
