#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include "reflective_core.h"

namespace reflective {

class attribute
{
public:
    using key_values_type = std::unordered_map<std::string, std::string>;

    attribute() = default;
    attribute(const std::string& name);
    attribute(const std::string& name, const key_values_type& key_values);
    attribute(const attribute& orig) = default;
    virtual ~attribute();

    attribute& operator=(const attribute& rhs) = delete;
    attribute& operator=(attribute&& rhs) = delete;

    const std::string& name() const
    {
        return name_;
    }

    const key_values_type get_key_values() const
    {
        return key_values_;
    }

    bool has_key(const std::string& name) const
    {
        return key_values_.find(name) != key_values_.end();
    }

    const std::string* get_value(const std::string& name) const
    {
        auto iter = key_values_.find(name);

        if (iter == key_values_.end())
        {
            return nullptr;
        }

        return &(iter->second);
    }

protected:

private:
    const std::string name_;
    key_values_type key_values_;
};

}

#endif
