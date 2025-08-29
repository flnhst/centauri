#ifndef R_ENUM_H_
#define R_ENUM_H_

#include "reflective_core.h"

#include "declaration.h"

namespace reflective {

class r_enum : public declaration
{
public:
    using items_type = std::unordered_map<std::string, std::uint64_t>;

    r_enum(const r_enum& orig) = delete;
    virtual ~r_enum();

    r_enum& operator=(const r_enum& rhs) = delete;

    const items_type& items() const
    {
        return items_;
    }

protected:
    r_enum(const std::string& name);

    items_type items_;

private:
};

}

#endif
