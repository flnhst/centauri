#ifndef TYPE_H_
#define TYPE_H_

#include "reflective_core.h"

namespace reflective {

enum class type_kind
{
    unknown,
    template_specialization,
    elaborated,
    record,
    l_value_reference,
    r_value_reference,
    alias,
    pointer,
    builtin,
    enum_t,
    function_proto,
    paren
};

constexpr const char* get_type_kind_c_str(type_kind kind)
{
    switch (kind)
    {
    case type_kind::unknown: return "unknown";
    case type_kind::template_specialization: return "template_specialization";
    case type_kind::elaborated: return "elaborated";
    case type_kind::record: return "record";
    case type_kind::l_value_reference: return "l_value_reference";
    case type_kind::r_value_reference: return "r_value_reference";
    case type_kind::alias: return "alias";
    case type_kind::pointer: return "pointer";
    case type_kind::builtin: return "builtin";
    case type_kind::enum_t: return "enum_t";
    case type_kind::function_proto: return "function_proto";
    case type_kind::paren: return "paren";
    }
}

class type
{
public:
    type() = delete;
    type(const type& orig) = delete;
    virtual ~type();

    type& operator=(const type& rhs) = delete;

    virtual const r_class* get_class() const = 0;

    type_kind kind() const
    {
        return kind_;
    }

    const std::string& name() const
    {
        return name_;
    }

    std::uint64_t hash() const
    {
        return hash_;
    }

    const char* kind_c_str() const
    {
        return get_type_kind_c_str(kind_);
    }

    bool is_pointer() const
    {
        return is_pointer_;
    }

    bool is_reference() const
    {
        return is_reference_;
    }

    bool is_const() const
    {
        return is_const_;
    }

    const std::string& to_string() const
    {
        return type_str_;
    }

    const type* inner_type() const
    {
        return inner_type_;
    }

    const std::vector<const type*>& template_arguments() const
    {
        return template_arguments_;
    }

    std::vector<const type*>& template_arguments()
    {
        return template_arguments_;
    }

    const type* get_first_inner_type_of_kind(type_kind kind) const;
    
protected:
    type(const std::string& type_str, std::uint64_t hash);

    const type* inner_type_{ nullptr };

    std::string name_;
    std::string type_str_;
    std::uint64_t hash_;

    type_kind kind_{ type_kind::unknown };

    bool is_pointer_{ false };
    bool is_reference_{ false };
    bool is_const_{ false };

    std::vector<const type*> template_arguments_;

    friend class database;
};

}

#endif
