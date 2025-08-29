#include "r_class.h"

#include "database.h"

namespace reflective {

r_class::r_class(const std::string& name)
    : declaration(name, declaration_class::r_class)
{
    database::get_instance().classes_.emplace(name, this);
}

r_class::~r_class() = default;

const class_method * r_class::get_method_by_name(const std::string & name) const
{
    auto iter = methods_map_.find(name);

    if (iter == methods_map_.end())
    {
        return nullptr;
    }

    return iter->second;
}

const class_property* r_class::get_property_by_name(const std::string& name) const
{
	auto iter = properties_map_.find(name);

	if (iter == properties_map_.end())
	{
		return nullptr;
	}

	return iter->second.get();
}

bool r_class::is_subclass_of(const r_class* clazz) const
{
	for (const auto* base_clazz : bases_)
	{
		if (clazz == base_clazz)
		{
			return true;
		}

		if (base_clazz->is_subclass_of(clazz))
		{
			return true;
		}
	}

	return false;
}

}
