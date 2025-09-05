#include <print>

#include <reflective_runtime/r_class.h>
#include <reflective_runtime/database.h>

#include <antares/runtime.h>

#include "entity.h"

#include "centauri_generated/dotnet_amalgamation.h"

void initialize_dotnet_runtime()
{
    const auto init_result = dotnet::runtime::initialize("dotnet/Centauri/bin/Debug/net9.0/Centauri.runtimeconfig.json");

    if (!init_result)
    {
        std::println(stderr, "Failed to initialize .NET runtime.");

        std::abort();
    }

    auto& runtime = dotnet::runtime::instance();

    const auto load_antares_result = runtime.load_assembly("dotnet/Centauri/bin/Debug/net9.0/Antares.dll");

    if (load_antares_result != dotnet::load_assembly_result::ok)
    {
        std::println(stderr, "Failed to load Antares.dll.");

        std::abort();
    }

    const auto load_centauri_result = runtime.load_assembly("dotnet/Centauri/bin/Debug/net9.0/Centauri.dll");

    if (load_centauri_result != dotnet::load_assembly_result::ok)
    {
        std::println(stderr, "Failed to load Centauri.dll.");

        std::abort();
    }

    const auto retrieve_native_result = runtime.retrieve_native_entrypoints();

    if (retrieve_native_result != dotnet::retrieve_native_entrypoints_result::ok)
    {
        std::println(stderr, "Failed to retrieve native entrypoints.");

        std::abort();
    }

    runtime.set_managed_callbacks();

    dotnet::runtime::set_tracing(true);

    initialize_centauri_generated_dotnet_bindings();
}

int main (int argc, char* argv[])
{
    std::println("Hello, World!");

    initialize_dotnet_runtime();

    return 0;
}
