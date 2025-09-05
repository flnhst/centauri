using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Centauri.SourceGenerators.Reflective;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;

namespace Centauri.SourceGenerators;

[Generator]
public class BindingsGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var reflectiveDbProvider = context.AdditionalTextsProvider.Where(file => file.Path.EndsWith("db.json"))
            .Select((a, c) => a.GetText(c)?.ToString());

        context.RegisterSourceOutput(reflectiveDbProvider, Generate);
    }

    private void Generate(SourceProductionContext context, string? reflectiveDbJson)
    {
        if (reflectiveDbJson == null)
        {
            return;
        }

        var db = DeserializeReflectiveDatabase(reflectiveDbJson);

        AddNativeEntrypointsSource(context, db);

        AddScriptableClassSources(context, db);
    }

    private static Database DeserializeReflectiveDatabase(string reflectiveDbJson)
    {
        var options = new JsonSerializerOptions()
        {
            PropertyNamingPolicy = JsonNamingPolicy.SnakeCaseLower,
            Converters =
            {
                new JsonStringEnumConverter(JsonNamingPolicy.SnakeCaseLower)
            }
        };

        var db = JsonSerializer.Deserialize<Database>(reflectiveDbJson, options);

        if (db == null)
        {
            throw new Exception("Could not deserialize database, returned null.");
        }

        DatabaseLinker.Link(db);

        return db;
    }

    private static void AddNativeEntrypointsSource(SourceProductionContext context, Database db)
    {
        var renderedCode = TemplateHelper.RenderTemplate("NativeEntrypoints", new
        {
            Db = db
        });

        context.AddSource($"NativeEntrypoints.g.cs", SourceText.From(renderedCode, Encoding.UTF8));
    }

    private static void AddScriptableClassSources(SourceProductionContext context, Database db)
    {
        var scriptableClasses = db.Classes
            .Where(clazz =>
                clazz.Attributes.Any(attr => attr.Name == "Scriptable") &&
                clazz.ShouldBeReflected);

        foreach (var scriptableClass in scriptableClasses)
        {
            if (scriptableClass.Name == null)
            {
                continue;
            }

            var name = StringFunctions.ToCamelCase(scriptableClass.Name);

            var renderedCode = TemplateHelper.RenderTemplate("ScriptableClass", new
            {
                Db = db,
                ScriptableClass = scriptableClass
            });

            context.AddSource($"{name}.g.cs", SourceText.From(renderedCode, Encoding.UTF8));
        }
    }
}