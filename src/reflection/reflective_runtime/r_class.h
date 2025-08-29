#ifndef R_CLASS_H_
#define R_CLASS_H_

#include "reflective_core.h"

#include "declaration.h"
#include "class_property.h"
#include "class_property_spec.h"
#include "class_constructor.h"
#include "class_constructor_spec.h"
#include "class_method.h"
#include "class_method_spec.h"
#include "database.h"

namespace reflective {

class r_class : public declaration
{
public:
    using members_map_type = std::unordered_map<std::string, declaration* >;
    using members_type = std::vector<declaration* >;
    using properties_map_type = std::unordered_map<std::string, std::unique_ptr<class_property> >;
    using properties_type = std::vector<class_property*>;
    using constructors_type = std::vector<class_constructor* >;
    using methods_map_type = std::unordered_map<std::string, class_method* >;

    using bases_type = std::vector<r_class*>;
    using children_type = std::vector<r_class*>;

    r_class() = delete;
    r_class(const r_class& orig) = delete;
    virtual ~r_class();

    r_class& operator=(const r_class& rhs) = delete;

    const members_map_type& members_map() const
    {
        return members_map_;
    }

    const members_type& members() const
    {
        return members_;
    }

    const properties_map_type& properties_map() const
    {
        return properties_map_;
    }

    const properties_type& properties() const
    {
        return properties_;
    }

    const constructors_type& constructors() const
    {
        return constructors_;
    }

    const methods_map_type& methods_map() const
    {
        return methods_map_;
    }

    const class_method* get_method_by_name(const std::string& name) const;

    const class_property* get_property_by_name(const std::string& name) const;

    virtual reflective::any create_instance() const = 0;

    virtual void destroy_instance(reflective::any& instance) const = 0;

    virtual reflective::any create_instance(std::vector<reflective::any_ref>& parameters) const = 0;
    
    bases_type& base_classes()
    {
        return bases_;
    }

    const bases_type& base_classes() const
    {
        return bases_;
    }

    children_type& child_classes()
    {
        return children_;
    }

    const children_type& child_classes() const
    {
        return children_;
    }

	bool is_subclass_of(const r_class* clazz) const;

    [[nodiscard]] bool is_template() const
    {
        return is_template_;
    }

protected:
    r_class(const std::string& name);

    template <typename T>
    void add_property_(const std::string& name, const type* type_ptr, declaration::attributes_type&& attributes, std::uint64_t offset)
    {
        std::unique_ptr<class_property_spec<T> > class_property_spec_ptr = std::make_unique<class_property_spec<T>>(name, type_ptr, offset);

        auto* ptr = class_property_spec_ptr.get();

        ptr->attributes_ = std::move(attributes);

        auto result = properties_map_.emplace(std::make_pair(name, std::move(class_property_spec_ptr)));

        if (result.second)
        {
            members_map_.emplace(std::make_pair(name, ptr));

            properties_.emplace_back(ptr);

            database::get_instance().declarations_.emplace_back(ptr);
        }
    }

    template <typename T>
    void add_constructor_(class_constructor_spec<T>* constructor_ptr)
    {
        constructors_.emplace_back(constructor_ptr);
    }

    template <typename T>
    void add_method_(class_method_spec<T>* function_ptr)
    {
        auto result = methods_map_.emplace(std::make_pair(function_ptr->name(), function_ptr));

        if (result.second)
        {
            members_map_.emplace(std::make_pair(function_ptr->name(), function_ptr));
        }
    }

    members_map_type members_map_;
    members_type members_;
    properties_map_type properties_map_;
    properties_type properties_;
    constructors_type constructors_;
    methods_map_type methods_map_;

    bases_type bases_;
    children_type children_;

    bool is_template_{ false };

private:

};

}

#endif
