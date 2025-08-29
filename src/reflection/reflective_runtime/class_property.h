#ifndef CLASS_PROPERTY_H_
#define CLASS_PROPERTY_H_

#include "reflective_core.h"

#include "declaration.h"

namespace reflective {

class class_property : public declaration
{
public:
    class_property() = delete;
    class_property(const class_property& orig) = delete;
    virtual ~class_property();

    class_property& operator=(const class_property& rhs) = delete;

    std::uint64_t offset() const
    {
        return offset_;
    }

    template <typename T, typename Class>
    T* get_value(Class* thiz) const
    {
        auto* u8_ptr = reinterpret_cast<std::uint8_t*>(thiz) + offset_;

        return reinterpret_cast<T*>(u8_ptr);
    }

	template <typename T, typename Class>
	const T* get_value_const(const Class* thiz) const
	{
		const auto* u8_ptr = reinterpret_cast<const std::uint8_t*>(thiz) + offset_;

		return reinterpret_cast<const T*>(u8_ptr);
	}

	template <typename T, typename Class>
	void get_value_const(const Class* thiz, T& value) const
	{
		value = *get_value_const(thiz);
	}

    builtin_type get_builtin_type() const
    {
        return builtin_type_;
    }

    const type* get_type() const
    {
        return type_;
    }

protected:
    class_property(const std::string& name, const type* type_ptr, std::uint64_t offset);

    std::uint64_t offset_;

    builtin_type builtin_type_{ builtin_type::none };
    const type* type_;

private:
    friend class database;
};

}

#endif
