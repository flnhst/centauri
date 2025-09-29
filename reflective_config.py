from typing import Any

import pyreflective
from pyreflective import SPDLOG_INFO

pyreflective.SPDLOG_INFO("YO!!!! OVER HERE!!!!!!!!!!!!!!")
pyreflective.SPDLOG_INFO("YO!!!! OVER HERE!!!!!!!!!!!!!!")
pyreflective.SPDLOG_INFO("YO!!!! OVER HERE!!!!!!!!!!!!!!")

def reflective_type_to_simple_type(r_type: pyreflective.Type) -> str:
    if r_type.is_builtin():
        match r_type.name():
            case "float":
                return "float"
            case "char":
                return "char"
            case "int":
                return "int"
            case "unsigned long long":
                return "unsigned long long"
            case "void":
                return "void"

    if r_type.is_pointer() and r_type.get_inner_type() is not None:
        return f"{reflective_type_to_simple_type(r_type.get_inner_type())}*"

    if r_type.kind_c_str() == "template_specialization" and r_type.name() == "::std::basic_string":
        return f"string"

    if r_type.get_inner_type() is not None:
        return reflective_type_to_simple_type(r_type.get_inner_type())

    raise Exception(f"Unknown type '{r_type.kind_c_str()}' '{r_type.name()}' '{r_type.to_string()}' '{r_type.to_string_slug()}'")

def generate_dotnet_class_name(scriptable_class: pyreflective.ClassDeclaration):
    return "dotnet_{}".format(scriptable_class.fully_qualified_name_slug())

def generate_dotnet_new_name(scriptable_class: pyreflective.ClassDeclaration, constructor: pyreflective.ConstructorDeclaration):
    return "dotnet_new_{}_{}".format(constructor.order(), scriptable_class.fully_qualified_name_slug())

def generate_native_interface_parameter_list(parameter_types: pyreflective.VectorPairStringType, begin_with_comma: bool):
    parameters_str = ""

    if begin_with_comma:
        parameters_str += ", "

    parameter_count = 0
    for pair in parameter_types:
        parameter_type = pair[1]

        pyreflective.SPDLOG_INFO("Doing parameter type {} = {}".format(parameter_type.to_string(), reflective_type_to_simple_type(parameter_type)))
        parameters_str += f"/*{reflective_type_to_simple_type(parameter_type)}*/"

        if reflective_type_to_simple_type(parameter_type) == "string":
            parameters_str += f"const char* argPtr{parameter_count}, int argLength{parameter_count}"
        else:
            parameters_str += f"{parameter_type.to_string()} ar{parameter_count}"

        if parameter_count != len(parameter_types) - 1:
            parameters_str += ", "

        parameter_count += 1

    return parameters_str

def snake_case_to_camel_case(sstr):
    s2 = sstr.split('_')
    return ''.join(word.capitalize() for word in s2)

def get_all_scriptable_classes(translation_unit) -> list[pyreflective.ClassDeclaration]:
    scriptable_classes: list[pyreflective.ClassDeclaration] = []

    for decl in translation_unit.all_declarations():
        if decl.get_class() != pyreflective.CLASS_CLASS_DECLARATION:
            continue

        attrs = decl.get_attributes()

        for attr in attrs:
            if attr.name() == "Scriptable":
                pyreflective.SPDLOG_INFO("Found class with scriptable attribute: '{}'.".format(decl.name()))
                scriptable_classes.append(decl)

    return scriptable_classes

def get_all_scriptable_enums(translation_unit):
    scriptable_enums = []

    for decl in translation_unit.all_declarations():
        if decl.get_class() != pyreflective.CLASS_ENUM_DECLARATION:
            continue

        attrs = decl.get_attributes()

        for attr in attrs:
            if attr.name() == "Scriptable":
                pyreflective.SPDLOG_INFO("Found enum with scriptable attribute: '{}'.".format(decl.name()))
                scriptable_enums.append(decl)

    return scriptable_enums

class DotnetAmalgamationTemplate(pyreflective.SourceTemplate):
    translation_unit: pyreflective.TranslationUnit
    context: pyreflective.Context
    project: pyreflective.Project
    project_configuration: pyreflective.ProjectConfiguration

    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit
        self.context = context
        self.project = self.context.get_project()
        self.project_configuration = self.project.get_project_configuration()

    def render(self):
        self.emit_include("dotnet_amalgamation.h", False)
        self.emit_line_empty()

        self.emit_include("antares/runtime.h", True)
        self.emit_include("antares/string_intptr_dictionary.h", True)
        self.emit_include("antares/ref.h", True)

        self.emit_line_empty()

        self.emit_include("print", True)

        self.emit_line_empty()

        self.render_translation_unit_includes()

        self.emit_line_empty()

        scriptable_classes = get_all_scriptable_classes(self.translation_unit)
        scriptable_enums = get_all_scriptable_enums(self.translation_unit)

        self.emit_comment("Classes begin")

        for scriptable_class in scriptable_classes:
            self.emit_comment(scriptable_class.name())
            self.render_dotnet_class(scriptable_class)

        self.emit_comment("Classes end")

        self.emit_line_empty()
        self.emit_line(f"void initialize_{self.project.name()}_generated_dotnet_bindings()")
        self.begin_scope_block()
        self.emit_line("auto& runtime = dotnet::runtime::instance();")

        project_name_cc = snake_case_to_camel_case(self.project.name())

        self.emit_line("const auto initialize_bindings_result = runtime.get_function_pointer<void(dotnet::object_handle_t)>(")
        self.emit_line(f"    \"{project_name_cc}.Generated.ManagedEntrypoints, {project_name_cc}\",")
        self.emit_line("    \"InitializeBindings\",")
        self.emit_line("    dotnet::delegate_type_name_options::unmanaged_callers_only);")

        self.emit_line("if (!initialize_bindings_result)")
        self.begin_scope_block()
        self.emit_line("std::println(stderr, \"Failed to initialize .NET bindings.\");")
        self.emit_line("std::abort();")
        self.end_scope_block()

        self.emit_line("auto callback_dict = dotnet::make_new<dotnet::string_intptr_dictionary>();")

        for scriptable_class in scriptable_classes:
            for constructor in scriptable_class.get_constructors():
                if constructor.should_be_reflected():
                    self.emit_line(f"callback_dict->set_element(\"{generate_dotnet_new_name(scriptable_class, constructor)}\", reinterpret_cast<std::intptr_t>(&{generate_dotnet_new_name(scriptable_class, constructor)}));")

        self.emit_line("initialize_bindings_result.value()(callback_dict.get_handle());")

        self.end_scope_block()

        return True

    def render_dotnet_class(self, scriptable_class: pyreflective.ClassDeclaration):
        self.begin_class(generate_dotnet_class_name(scriptable_class), scriptable_class.fully_qualified_name())

        self.emit_declaration_visibility(pyreflective.VIS_PUBLIC)

        self.emit_line(f"~{generate_dotnet_class_name(scriptable_class)}() override = default;")

        self.end_class()

        for constructor in scriptable_class.get_constructors():
            if constructor.should_be_reflected():
                pyreflective.SPDLOG_INFO("HELLLO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                pyreflective.SPDLOG_INFO("HELLLO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                pyreflective.SPDLOG_INFO("HELLLO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                self.emit_comment("Hello!")
                self.emit_line(f"{generate_dotnet_class_name(scriptable_class)}* {generate_dotnet_new_name(scriptable_class, constructor)}(dotnet::object_handle_t obj{generate_native_interface_parameter_list(constructor.get_parameters(), True)})")
                self.begin_scope_block()
                self.emit_line(f"return new {generate_dotnet_class_name(scriptable_class)}();")
                self.end_scope_block()

    def render_dotnet_class_new(self, scriptable_class: pyreflective.ClassDeclaration, constructor: pyreflective.ConstructorDeclaration):
        pass


class DotnetAmalgamationHeaderTemplate(pyreflective.SourceTemplate):
    translation_unit: pyreflective.TranslationUnit
    context: pyreflective.Context
    project: pyreflective.Project
    project_configuration: pyreflective.ProjectConfiguration

    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit
        self.context = context
        self.project = self.context.get_project()
        self.project_configuration = self.project.get_project_configuration()

    def render(self):
        self.begin_header_guard()

        self.emit_line_empty()
        self.emit_line(f"void initialize_{self.project.name()}_generated_dotnet_bindings();")
        self.emit_line_empty()

        self.end_header_guard()

        return True


class DotnetFooterHeaderTemplate(pyreflective.SourceTemplate):
    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit

    def render(self):
        pass


def setup(context: pyreflective.Context):
    pyreflective.SPDLOG_INFO("Hello, from Centauri!")

    project: pyreflective.Context = context.get_project()
    config: pyreflective.ProjectConfiguration = project.get_project_configuration()

    pyreflective.SPDLOG_INFO("Adding dotnet_amalgamation generated files...")
    project.add_generated_source_file(DotnetAmalgamationTemplate, "dotnet_amalgamation.cpp")
    project.add_generated_header_file(DotnetAmalgamationHeaderTemplate, "dotnet_amalgamation.h")
    pyreflective.SPDLOG_INFO("PY: Added dotnet_amalgamation generated files.")

    config.set_header_template_footer_class(DotnetFooterHeaderTemplate)


if __name__ == '__main__' and pyreflective.context is not None:
    setup(pyreflective.context)
