#ifndef REFLECTIVE_CORE_H
#define REFLECTIVE_CORE_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>
#include <cstdint>
#include <memory>
#include <string>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>

#ifdef REFLECTIVE_RUNTIME_DEVELOPMENT
#include "log/log.h"
#endif

#if defined(_MSC_VER)
#define REFLECTIVE_STRUCT_OFFSET(type, member) (offsetof(type, member))
#elif defined(__GNUC__)
#define REFLECTIVE_STRUCT_OFFSET(type, member) (__builtin_offsetof(type, member))
#else
#error "Failed to find an 'offsetof' implementation."
#endif

#define REFLECTIVE_THREAD_LOCAL thread_local

namespace reflective
{

class attribute;
class class_method;
class declaration;
class class_property;
class database;
class r_class;
class r_enum;
class type;
class type_enum;

template <typename T>
class class_method_spec;

template <typename T>
class class_property_spec;

template <typename T>
class r_class_spec;

template <typename T>
class r_enum_spec;

template <typename T>
class type_spec;

enum class builtin_type
{
    none,
    float_type,
    double_type,
    char_type,
    unsigned_char_type,
    int_type,
    unsigned_int_type,
    long_type,
    unsigned_long_type,
    long_long_type,
    unsigned_long_long_type,
    bool_type
};

struct unknown_type {};

struct type_kind_tag_unknown {};
struct type_kind_tag_template_specialization {};
struct type_kind_tag_elaborated {};
struct type_kind_tag_record {};
struct type_kind_tag_l_value_reference {};
struct type_kind_tag_r_value_reference {};
struct type_kind_tag_alias {};
struct type_kind_tag_pointer {};
struct type_kind_tag_builtin {};
struct type_kind_tag_enum_t {};

template <typename T>
constexpr const r_class* get_class()
{
    return nullptr;
}

template <typename T, typename Tag>
constexpr const type* get_type()
{
    return nullptr;
}

template <typename Func, typename T, typename... Args>
void for_each_arg(Func func, T&& t, Args&&... args)
{
    func(t);

    for_each_arg(func, std::forward<Args>(args)...);
}

template <typename Func, typename T>
void for_each_arg(Func func, T&& t)
{
    func(t);
}

using database_initialize_hook_function_type = void(*)(database& db);

class any_internal_container_allocator
{
public:
    constexpr static std::uint64_t BLOCK_SIZE = 64;
    constexpr static std::uint64_t NR_OF_BLOCKS = 512;

    any_internal_container_allocator();
    ~any_internal_container_allocator();

    template <typename T>
    T* allocate()
    {
        static_assert(sizeof(T) <= BLOCK_SIZE, "Size of allocation must be smaller or equal to any_stack::BLOCK_SIZE.");

        auto iter = std::find(allocated_.begin(), allocated_.end(), false);

        if (iter == allocated_.end())
        {
            auto message = "iter == allocated_.end() is true.";

            throw std::runtime_error(message);
        }

        std::uint64_t index = std::distance(allocated_.begin(), iter);

        allocated_[index] = true;

        return reinterpret_cast<T*>(&(memory_[index * BLOCK_SIZE]));
    }

    template <typename T>
    void deallocate(T* t)
    {
        std::uint64_t offset = reinterpret_cast<std::uint8_t*>(t) - &memory_[0];

        allocated_[offset / BLOCK_SIZE] = false;
    }

private:
    std::vector<std::uint8_t> memory_;
    std::vector<bool> allocated_;
};

extern REFLECTIVE_THREAD_LOCAL any_internal_container_allocator g_any_internal_container_allocator;

class any
{
public:
    any();
    any(any&& orig);
    any(const any& orig) = delete;
    virtual ~any();

    template <typename T>
    explicit any(const T& t)
        : container_(nullptr)
    {
        container_ = g_any_internal_container_allocator.allocate<internal_typed_container<T>>();

        new (container_) internal_typed_container<T>(t);
    }

    template <typename T>
    explicit any(T&& t)
        : container_(nullptr)
    {
        container_ = g_any_internal_container_allocator.allocate<internal_typed_container<T>>();

        new (container_) internal_typed_container<T>(t);
    }

    any& operator=(any&& rhs);
    any& operator=(const any& rhs) = delete;

    bool empty() const;

    r_class* get_class() const;

    class internal_container
    {
    public:
        virtual ~internal_container();

        virtual const r_class* get_class() const = 0;
    };

    template <typename T>
    class internal_typed_container : public internal_container
    {
    public:
        using value_type = std::remove_reference_t<T>;

        virtual ~internal_typed_container()
        {
            (&t_)->~value_type();
        }

        internal_typed_container(const value_type& t)
            : t_(t)
        {

        }

        internal_typed_container(value_type&& t)
            : t_(t)
        {

        }

        value_type t_;

    public:
        const r_class* get_class() const override
        {
            return ::reflective::get_class<T>();
        }
    };

private:

    internal_container* container_;

    template <typename T>
    friend any make_any(const T& t);

    template <typename T>
    friend T& any_cast(any& a);

    template <typename T>
    friend const T& any_cast(const any& a);

    template <typename T>
    friend const T& any_ref_cast(const any& a);
};

template <typename T>
any make_any(const T& t)
{
    return any(t);
}

template <typename T>
any make_any(T&& t)
{
    return any(std::forward<T>(t));
}

template <typename T>
T& any_cast(any& a)
{
    if (a.container_ == nullptr)
    {
        throw std::runtime_error("a.container_ == nullptr is true.");
    }

    return reinterpret_cast<any::internal_typed_container<T>*>(a.container_)->t_;
}

template <typename T>
const T& any_cast(const any& a)
{
    if (a.container_ == nullptr)
    {
        throw std::runtime_error("a.container_ == nullptr is true");
    }

    return reinterpret_cast<const any::internal_typed_container<T>*>(a.container_)->t_;
}

// =================================================================================================

class any_ref
{
public:
    any_ref();
    any_ref(any_ref&& orig);
    any_ref(const any_ref& orig) = delete;
    virtual ~any_ref();

    template <typename T>
    explicit any_ref(const T& t)
        : container_(nullptr)
    {
        container_ = g_any_internal_container_allocator.allocate<internal_typed_container<const T&>>();

        new (container_) internal_typed_container<const T&>(t);
    }

    template <typename T>
    explicit any_ref(T& t)
        : container_(nullptr)
    {
        container_ = g_any_internal_container_allocator.allocate<internal_typed_container<T&>>();

        new (container_) internal_typed_container<T&>(t);
    }

    any_ref& operator=(any_ref&& rhs);
    any_ref& operator=(const any_ref& rhs) = delete;

    bool empty() const;

    class internal_container
    {
    public:
    };

    template <typename T>
    class internal_typed_container : public internal_container
    {
    public:
        using value_type = T&;

        internal_typed_container(value_type& t)
            : t_(t)
        {

        }

        value_type t_;

    public:
    };

private:

    internal_container* container_;

    template <typename T>
    friend any_ref make_any_ref(T& t);

    template <typename T>
    friend any_ref make_any_ref(const T& t);

    template <typename T>
    friend T& any_ref_cast(any_ref& a);

    template <typename T>
    friend const T& any_ref_cast(const any_ref& a);
};

template <typename T>
any_ref make_any_ref(T& t)
{
    return any_ref(t);
}

template <typename T>
any_ref make_any_ref(const T& t)
{
    return any_ref(t);
}

template <typename T>
any_ref make_any_ref(std::reference_wrapper<T> ref_wrapper)
{
    return any_ref(ref_wrapper.get());
}

template <typename T>
T& any_ref_cast(any_ref& a)
{
    if (a.container_ == nullptr)
    {
        throw std::runtime_error("a.container_ == nullptr is true");
    }

    return reinterpret_cast<any_ref::internal_typed_container<T&>*>(a.container_)->t_;
}

template <typename T>
const T& any_ref_cast(const any& a)
{
    if (a.container_ == nullptr)
    {
        throw std::runtime_error("a.container_ == nullptr is true");
    }

    return reinterpret_cast<const any_ref::internal_typed_container<T&>*>(a.container_)->t_;
}

template <typename Return, typename... Args>
struct discombobulate_function_type
{
    using tuple_type = std::tuple<Args...>;

    template <size_t Size>
    using argument_type = typename std::tuple_element<Size, tuple_type>::type;

    using return_type = Return;
};

template <typename Return, typename... Args>
auto discombobulate_function(Return(*func_ptr)(Args...)) -> discombobulate_function_type<Return, Args...>
{
    return discombobulate_function_type<decltype(func_ptr), Return, Args...>();
}

template <typename Func>
using function_type = decltype(discombobulate_function(std::declval<Func>()));

template <typename T, typename Return, typename... Args>
struct discombobulate_method_type
{
    using tuple_type = std::tuple<Args...>;

    template <size_t Size>
    using argument_type = typename std::tuple_element<Size, tuple_type>::type;

    using class_type = T;

    using return_type = Return;
};

template <typename T, typename Return>
struct discombobulate_method_no_arguments_type
{
    using class_type = T;

    using return_type = Return;
};

template <typename T, typename Return, typename... Args>
auto discombobulate_method(Return(T::*func_ptr)(Args...)) -> discombobulate_method_type<T, Return, Args...>
{
    return discombobulate_method_type<T, Return, Args...>();
}

template <typename T, typename Return, typename... Args>
auto discombobulate_method(Return(T::*func_ptr)(Args...) const) -> discombobulate_method_type<T, Return, Args...>
{
    return discombobulate_method_type<T, Return, Args...>();
}

template <typename T, typename Return>
auto discombobulate_method(Return(T::*func_ptr)(void)) -> discombobulate_method_no_arguments_type<T, Return>
{
    return discombobulate_method_no_arguments_type<T, Return>();
}

template <typename T, typename Return>
auto discombobulate_method(Return(T::*func_ptr)(void) const) -> discombobulate_method_no_arguments_type<T, Return>
{
    return discombobulate_method_no_arguments_type<T, Return>();
}

template <typename Func>
using method_type = decltype(discombobulate_method(std::declval<Func>()));

template <typename T>
struct is_reflected : std::false_type
{
};

template <typename T>
inline constexpr bool is_reflected_v = is_reflected<T>::value;

}

#endif
