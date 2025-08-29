#ifndef DATABASE_H_
#define DATABASE_H_

#include "reflective_core.h"

namespace reflective {

void initialize_database_(database& db);

class database
{
public:
    using declarations_type = std::vector<declaration*>;
    using enumerators_type = std::unordered_map<std::string, const r_enum*>;
    using classes_type = std::unordered_map<std::string, const r_class*>;
    using types_hash_map_type = std::unordered_map<std::uint64_t, const type*>;
    using types_name_map_type = std::unordered_map<std::string, const type*>;

    database();
    database(const database& orig) = delete;
    virtual ~database();

    database& operator=(const database& rhs) = delete;

    static database& get_instance();

    const declarations_type& get_declarations() const
    {
        return declarations_;
    }

    declarations_type& get_declarations()
    {
        return declarations_;
    }

    const enumerators_type& get_enumerators() const
    {
        return enumerators_;
    }

    enumerators_type& get_enumerators()
    {
        return enumerators_;
    }

    const r_enum* get_enumerator_by_name(const std::string& name) const;

    const classes_type& get_classes() const
    {
        return classes_;
    }

    classes_type& get_classes()
    {
        return classes_;
    }

    const r_class* get_class_by_name(const std::string& name) const;

    const types_name_map_type& get_types_name_map() const
    {
        return types_name_map_;
    }

    types_name_map_type& get_types_types_name_map()
    {
        return types_name_map_;
    }

    const types_hash_map_type& get_types_hash_map() const
    {
        return types_hash_map_;
    }

    types_hash_map_type& get_types_types_hash_map()
    {
        return types_hash_map_;
    }

    const type* get_type_by_name(const std::string& name) const;
    const type* get_type_by_hash(std::uint64_t hash, const type* def = nullptr) const;

    void fix_type_pointers();

protected:

private:
    void add_type_(const std::string& name, std::uint64_t hash, const type* type_ptr);

    static database* database_instance_;

    declarations_type declarations_;
    enumerators_type enumerators_;
    classes_type classes_;
    types_name_map_type types_name_map_;
    types_hash_map_type types_hash_map_;

    friend class r_enum;
    friend class r_class;
    friend class type;
};

}

#endif
