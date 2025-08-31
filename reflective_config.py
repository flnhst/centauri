import pyreflective

pyreflective.SPDLOG_INFO("Hello, from Centauri!")

def snake_case_to_camel_case(sstr):
    s2 = sstr.split('_')
    return ''.join(word.capitalize() for word in s2)

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

        self.emit_line_empty()
        self.emit_include("print", True)
        self.emit_line_empty()

        self.emit_line_empty()
        self.emit_line(f"void initialize_{self.project.name()}_generated_dotnet_bindings()")
        self.begin_scope_block()
        self.emit_line("auto& runtime = dotnet::runtime::instance();")

        project_name_cc = snake_case_to_camel_case(self.project.name())

        self.emit_line("const auto initialize_bindings_result = runtime.get_function_pointer<void()>(")
        self.emit_line(f"    \"{project_name_cc}.Generated.NativeEntrypoints, {project_name_cc}\",")
        self.emit_line("    \"InitializeBindings\",")
        self.emit_line("    dotnet::delegate_type_name_options::unmanaged_callers_only);")

        self.emit_line("if (!initialize_bindings_result)")
        self.begin_scope_block()
        self.emit_line("std::println(stderr, \"Failed to initialize .NET bindings.\");")
        self.emit_line("std::abort();")
        self.end_scope_block()

        self.emit_line("initialize_bindings_result.value()();")

        self.end_scope_block()


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


class DotnetFooterHeaderTemplate(pyreflective.SourceTemplate):
    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit

    def render(self):
        pass


def setup(context: pyreflective.Context):
    project: pyreflective.Context = context.get_project()
    config: pyreflective.ProjectConfiguration = project.get_project_configuration()

    pyreflective.SPDLOG_INFO("Project arcturus.")

    pyreflective.SPDLOG_INFO("Adding dotnet_amalgamation generated files...")
    project.add_generated_source_file(DotnetAmalgamationTemplate, "dotnet_amalgamation.cpp")
    project.add_generated_header_file(DotnetAmalgamationHeaderTemplate, "dotnet_amalgamation.h")
    pyreflective.SPDLOG_INFO("PY: Added dotnet_amalgamation generated files.")

    config.set_header_template_footer_class(DotnetFooterHeaderTemplate)


if __name__ == '__main__' and pyreflective.context is not None:
    setup(pyreflective.context)
