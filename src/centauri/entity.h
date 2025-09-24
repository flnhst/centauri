#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "centauri_generated/entity.generated.h"

// [Scriptable]
// [Class]
class entity 
{
    REFLECTIVE_BODY_ENTITY();

public:
    entity();
    // [Constructor]
    entity(std::uint64_t blaap, int woep, float waap, const std::string& something);
    entity(const entity& orig) = delete;
    entity(entity&& orig) = delete;
    virtual ~entity();

    entity& operator=(const entity& orig) = delete;
    entity& operator=(entity&& orig) = delete;

    // [Method]
    virtual void update();

    // [Method]
    void some_ptr(int* ptr);

    // [Method]
    void some_string1(std::string str);
    // [Method]
    void some_string2(std::string& str);
    // [Method]
    void some_string3(const std::string& str);

protected:
private:
};

#endif
