#ifndef CLASS_METHOD_H_
#define CLASS_METHOD_H_

#include "reflective_core.h"

#include "declaration.h"
#include "type.h"

namespace reflective {

class class_method : public declaration
{
public:
    using parameters_type = std::unordered_map<std::string, const type*>;

    class_method() = delete;
    class_method(const class_method& orig) = delete;
    virtual ~class_method();

    class_method& operator=(const class_method& rhs) = delete;

    const type& return_type() const
    {
        return *return_type_;
    }

    const parameters_type& parameter_types() const
    {
        return parameters_;
    }

    parameters_type& parameter_types()
    {
        return parameters_;
    }

    template <typename Return, typename T>
    Return invoke(T instance) const
    {
        auto instance_any = make_any<T>(instance);

        return any_cast<Return>(invoke(instance_any));
    }

    template <typename T>
    void invoke_no_return(T instance) const
    {
        auto instance_any = make_any<T>(instance);

        invoke_no_return(instance_any);
    }

    template <typename Return, typename T, typename... Args>
    Return invoke(T instance, Args... args) const
    {
        std::vector<reflective::any_ref> anies;

        auto f = [&anies](auto& arg) {
            anies.emplace_back(std::move(reflective::make_any_ref(arg)));
        };

        for_each_arg(f, std::forward<Args&>(args)...);

        auto instance_any = make_any(instance);

        return any_cast<Return>(invoke(instance_any, anies));
    }

    template <typename T, typename... Args>
    void invoke_no_return(T instance, Args... args) const
    {
        std::vector<reflective::any_ref> anies;

        auto f = [&anies](auto arg) {
            anies.emplace_back(std::move(reflective::make_any_ref(arg)));
        };

        for_each_arg(f, std::forward<Args&>(args)...);

        auto instance_any = make_any(instance);

        invoke(instance_any, anies);
    }

    virtual void invoke_no_return(any& instance, std::vector<any_ref>& parameters) const = 0;
    virtual void invoke_no_return(any& instance) const = 0;

    virtual any invoke(any& instance, std::vector<any_ref>& parameters) const = 0;
    virtual any invoke(any& instance) const = 0;

protected:
    class_method(const std::string& name);
    
    const type* return_type_;
    parameters_type parameters_;

private:
    friend class database;
};

}

#endif
