#include "class_constructor.h"

namespace reflective {

class_constructor::~class_constructor()
{

}

class_constructor::class_constructor()
    : declaration("(ctor)", declaration_class::class_constructor)
{
}

}
