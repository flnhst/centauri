#include "reflective_core.h"

#ifdef NO_LOG_STREAM

nop_stream nop_stream_instance;

#endif

namespace reflective
{

any_internal_container_allocator::any_internal_container_allocator()
{
    memory_.resize(NR_OF_BLOCKS * BLOCK_SIZE, 0);
    allocated_.resize(NR_OF_BLOCKS, false);
}

any_internal_container_allocator::~any_internal_container_allocator()
{
}

REFLECTIVE_THREAD_LOCAL any_internal_container_allocator g_any_internal_container_allocator;

any::internal_container::~internal_container()
{

}

any::any()
    : container_(nullptr)
{

}

any::any(any&& orig)
    : container_(nullptr)
{
    if (this != &orig)
    {
        auto* this_container = this->container_;

        this->container_ = orig.container_;

        orig.container_ = this_container;
    }
}

any::~any()
{
    if (this->container_ != nullptr)
    {
        container_->~internal_container();

        g_any_internal_container_allocator.deallocate(container_);
    }
}

any& any::operator=(any&& rhs)
{
    if (this != &rhs)
    {
        auto* this_container = this->container_;

        this->container_ = rhs.container_;

        rhs.container_ = this_container;
    }

    return *this;
}

bool any::empty() const
{
    return container_ == nullptr;
}

r_class * any::get_class() const
{
    return nullptr;
}

// ==============================================================================

any_ref::any_ref()
    : container_(nullptr)
{

}

any_ref::any_ref(any_ref&& orig)
    : container_(nullptr)
{
    if (this != &orig)
    {
        auto* this_container = this->container_;

        this->container_ = orig.container_;

        orig.container_ = this_container;
    }
}

any_ref::~any_ref()
{
    if (this->container_ != nullptr)
    {
        container_->~internal_container();

        g_any_internal_container_allocator.deallocate(container_);
    }
}

any_ref& any_ref::operator=(any_ref&& rhs)
{
    if (this != &rhs)
    {
        auto* this_container = this->container_;

        this->container_ = rhs.container_;

        rhs.container_ = this_container;
    }

    return *this;
}

bool any_ref::empty() const
{
    return container_ == nullptr;
}

}
