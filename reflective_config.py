import pyreflective

pyreflective.SPDLOG_INFO("Hello, from Centauri!")

class DotnetAmalgamationTemplate(pyreflective.SourceTemplate):
    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit

    def render(self):
        pass


class DotnetAmalgamationHeaderTemplate(pyreflective.SourceTemplate):
    def __init__(self, context, translation_unit):
        pyreflective.SourceTemplate.__init__(self, context, translation_unit)

        self.translation_unit = translation_unit



    def render(self):
        pass


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
