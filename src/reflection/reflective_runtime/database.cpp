#include "database.h"

#include "declaration.h"
#include "class_constructor.h"
#include "class_method.h"
#include "class_property.h"

extern "C" {

bool IsDebuggerPresent();

}

namespace reflective {

database* database::database_instance_ = nullptr;

database::database() = default;

database::~database() = default;

database& database::get_instance()
{
    if (!database::database_instance_)
    {
        database::database_instance_ = new database();
    }

    return *database::database_instance_;
}

const r_enum* database::get_enumerator_by_name(const std::string& name) const
{
    auto iter = enumerators_.find(name);

    if (iter == enumerators_.end())
    {
        return nullptr;
    }

    return iter->second;
}

const r_class* database::get_class_by_name(const std::string& name) const
{
    auto iter = classes_.find(name);

    if (iter == classes_.end())
    {
        return nullptr;
    }

    return iter->second;
}

const type* database::get_type_by_name(const std::string& name) const
{
    auto iter = types_name_map_.find(name);

    if (iter == types_name_map_.end())
    {
        return nullptr;
    }

    return iter->second;
}

const type* database::get_type_by_hash(std::uint64_t hash, const type* def) const
{
    auto iter = types_hash_map_.find(hash);

    if (iter == types_hash_map_.end())
    {
        return def;
    }

    return iter->second;
}

void database::fix_type_pointers()
{
    for (auto* decl : declarations_)
    {
        if (decl->get_declaration_class() == declaration_class::class_constructor)
        {
            auto* class_constructor_decl = dynamic_cast<class_constructor*>(decl);
            
            for (auto& parameter_type : class_constructor_decl->parameters_)
            {
                parameter_type.second = get_type_by_hash(reinterpret_cast<std::uint64_t>(parameter_type.second), parameter_type.second);
            }
        }

        if (decl->get_declaration_class() == declaration_class::class_method)
        {
            auto* class_method_decl = dynamic_cast<class_method*>(decl);

            for (auto& parameter_type : class_method_decl->parameters_)
            {
                parameter_type.second = get_type_by_hash(reinterpret_cast<std::uint64_t>(parameter_type.second), parameter_type.second);
            }

            class_method_decl->return_type_ = get_type_by_hash(reinterpret_cast<std::uint64_t>(class_method_decl->return_type_), class_method_decl->return_type_);
        }

        if (decl->get_declaration_class() == declaration_class::class_property)
        {
            auto* class_property_decl = dynamic_cast<class_property*>(decl);

            class_property_decl->type_ = get_type_by_hash(reinterpret_cast<std::uint64_t>(class_property_decl->type_), class_property_decl->type_);
        }
    }

    for (auto& type_pair : types_hash_map_)
    {
        auto* type_ptr = const_cast<type*>(type_pair.second);

        type_ptr->inner_type_ = get_type_by_hash(reinterpret_cast<std::uint64_t>(type_ptr->inner_type_), type_ptr->inner_type_);

        for (auto& template_arg : type_ptr->template_arguments())
        {
            template_arg = get_type_by_hash(reinterpret_cast<std::uint64_t>(template_arg), template_arg);
        }
    }
}

void database::add_type_(const std::string& name, std::uint64_t hash, const type* type_ptr)
{
    if (types_hash_map_.find(hash) == types_hash_map_.end())
    {
        types_hash_map_.emplace(hash, type_ptr);
        types_name_map_.emplace(name, type_ptr);
    }
}

}
