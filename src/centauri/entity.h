#ifndef ENTITY_H
#define ENTITY_H

#include "centauri_generated/entity.generated.h"

#include <vector>

// [Enum]
enum class test_enum
{
    foo,
    bar,
    awesome,
    woep
};

// [Class]
class entity 
{
    REFLECTIVE_BODY_ENTITY();

public:
    // [Constructor]
    entity();
    entity(const entity& orig) = delete;
    entity(entity&& orig) = delete;
    virtual ~entity();

    entity& operator=(const entity& orig) = delete;
    entity& operator=(entity&& orig) = delete;

    // [Method]
    float test(int a, int c, entity* entity_ptr);

    void do_list(std::vector<entity*> blaap)
    {

    }

protected:


private:
    int a_field_here;

    float another_field;
};

// [Class]
class foo : public entity
{
    REFLECTIVE_BODY_FOO();
public:
protected:
private:
};

namespace ns_test
{

// [Struct]
// [Blaap(Woep=11,wfdefe=eefe)]
struct blaap
{
    // [Blaap(Woep=11,wfdefe=eefe)]
    int foo;
};

}

template <int Foo>
class bar
{
public:

private:
    static constexpr int blaap_ = Foo;
};

#endif
