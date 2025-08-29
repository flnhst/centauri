cmake_minimum_required(VERSION 3.31)
project(centauri)

set(CMAKE_CXX_STANDARD 23)

find_package(utf8cpp CONFIG REQUIRED)
find_package(unofficial-nethost CONFIG REQUIRED)
find_package(antares CONFIG REQUIRED)
find_package(Reflective CONFIG REQUIRED)

set(CENTAURI_SOURCES
        src/reflection/reflective_runtime/attribute.cpp
        src/reflection/reflective_runtime/attribute.h
        src/reflection/reflective_runtime/class_constructor.cpp
        src/reflection/reflective_runtime/class_constructor.h
        src/reflection/reflective_runtime/class_constructor_spec.h
        src/reflection/reflective_runtime/class_method.cpp
        src/reflection/reflective_runtime/class_method.h
        src/reflection/reflective_runtime/class_method_spec.h
        src/reflection/reflective_runtime/class_property.cpp
        src/reflection/reflective_runtime/class_property.h
        src/reflection/reflective_runtime/class_property_spec.h
        src/reflection/reflective_runtime/database.cpp
        src/reflection/reflective_runtime/database.h
        src/reflection/reflective_runtime/declaration.cpp
        src/reflection/reflective_runtime/declaration.h
        src/reflection/reflective_runtime/reflective_core.cpp
        src/reflection/reflective_runtime/reflective_core.h
        src/reflection/reflective_runtime/r_class.cpp
        src/reflection/reflective_runtime/r_class.h
        src/reflection/reflective_runtime/r_class_spec.h
        src/reflection/reflective_runtime/r_class_template.cpp
        src/reflection/reflective_runtime/r_class_template.h
        src/reflection/reflective_runtime/r_enum.cpp
        src/reflection/reflective_runtime/r_enum.h
        src/reflection/reflective_runtime/r_enum_spec.h
        src/reflection/reflective_runtime/type.cpp
        src/reflection/reflective_runtime/type.h
        src/reflection/reflective_runtime/type_enum.cpp
        src/reflection/reflective_runtime/type_enum.h
        src/reflection/reflective_runtime/type_spec.h
        src/centauri/main.cpp
        src/centauri/entity.cpp
        src/centauri/entity.h)

add_executable(centauri ${CENTAURI_SOURCES})

set(ALL_SOURCES "${CENTAURI_SOURCES}")

set(ALL_TARGETS_AND_DEPENDENCIES)

list(APPEND ALL_TARGETS_AND_DEPENDENCIES
    unofficial::nethost::nethost 
    utf8::cpp
    Antares::antares)

list(APPEND SYSTEM_INCLUDE_PREFIXES
    ${VCPKG_INSTALLED_DIR})

reflective_setup("${ALL_SOURCES}" "${ALL_TARGETS_AND_DEPENDENCIES}" "${SYSTEM_INCLUDE_PREFIXES}")

foreach(RUNTIME_TARGET ${RUNTIME_TARGETS})
    target_link_libraries(${RUNTIME_TARGET} PUBLIC ReflectiveGenerated)
endforeach()

target_include_directories(ReflectiveGenerated PRIVATE src src/reflection)

target_include_directories(centauri PUBLIC src/reflection)

target_link_libraries(centauri 
        PUBLIC
        ReflectiveGenerated
        ${ALL_TARGETS_AND_DEPENDENCIES})
