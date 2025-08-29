#ifndef DECLARATION_H_
#define DECLARATION_H_

#include "reflective_core.h"

#include "attribute.h"

namespace reflective {

enum class declaration_class
{
    r_class,
    class_constructor,
    class_method,
    class_property,
    r_enum
};

class declaration
{
public:
    using attributes_type = std::vector<attribute>;

    declaration() = delete;

    declaration(const std::string& name, declaration_class decl_class)
        : name_(name), class_(decl_class)
    {

    }

    declaration(const declaration& orig) = delete;
    virtual ~declaration();

    declaration& operator=(const declaration& rhs) = delete;

    const std::string& name() const
    {
        return name_;
    }

    declaration_class get_declaration_class() const
    {
        return class_;
    }

    const attributes_type& get_attributes() const
    {
        return attributes_;
    }

    bool has_attribute(const std::string& attribute_name) const
    {
        return std::any_of(attributes_.begin(), attributes_.end(),
            [&attribute_name](const auto& attr){
                return attr.name() == attribute_name;
        });
    }

    const attribute* get_attribute(const std::string& attribute_name) const
    {
        auto result = std::find_if(attributes_.begin(), attributes_.end(),
        [&attribute_name](const auto& attr) {
            return attr.name() == attribute_name;
        });

        if (result == attributes_.end())
        {
            return nullptr;
        }

        return &(*result);
    }

protected:

private:
    std::string name_;

    declaration_class class_;
    
protected:
    attributes_type attributes_;

    friend class r_class;
};

}

#endif
