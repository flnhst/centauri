#ifndef ENTITY_H
#define ENTITY_H

#include "centauri_generated/entity.generated.h"

// [Scriptable]
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

    virtual void update();

protected:
private:
};

#endif
