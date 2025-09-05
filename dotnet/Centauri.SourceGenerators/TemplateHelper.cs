using System;
using System.Reflection;
using Scriban;
using Scriban.Runtime;

namespace Centauri.SourceGenerators;

public static class DateTimeOffsetFunctions
{
    public static string Iso8601(DateTimeOffset value)
    {
        return value.ToString("O");
    }
}

public static class StringFunctions
{
    public static string ToCamelCase(string value)
    {
        return value.Split('_').Select(s => s.Substring(0, 1).ToUpper() + s.Substring(1)).Aggregate((a, b) => a + b);
    }
}

public static class TemplateHelper
{
    private static string GetTemplateSource(string name)
    {
        var assembly = Assembly.GetExecutingAssembly();

        var resourceName = $"Centauri.SourceGenerators.Templates.{name}.scriban";

        using var resourceStream = assembly.GetManifestResourceStream(resourceName);

        if (resourceStream == null)
        {
            throw new Exception($"Could not find template with name '{name}'.");
        }

        using var textReader = new StreamReader(resourceStream);

        return textReader.ReadToEnd();
    }

    public static string RenderTemplate(string name, object model)
    {
        var globalObject = new ScriptObject();
        globalObject.Import(model);

        var functionsObject = new ScriptObject();
        functionsObject.Import(typeof(DateTimeOffsetFunctions));
        functionsObject.Import(typeof(StringFunctions));

        var context = new TemplateContext();
        context.PushGlobal(functionsObject);
        context.PushGlobal(globalObject);

        var template = Template.Parse(GetTemplateSource(name));

        if (template.HasErrors)
        {
            foreach (var error in template.Messages)
            {
                Console.Error.WriteLine(error);
            }

            throw new Exception("Template has errors.");
        }

        return template.Render(context);
    }
}
