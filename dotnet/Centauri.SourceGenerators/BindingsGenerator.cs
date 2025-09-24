using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Centauri.SourceGenerators.Reflective;
using Centauri.SourceGenerators.Templates;
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

        AddTemplateSource<ManagedEntrypointsTemplate>(context, db, "ManagedEntrypoints", GetScriptableClasses(db).ToList());

        AddScriptableClassSources(context, db);

        AddTemplateSource<CallbacksTemplate>(context, db, "Callbacks", GetScriptableClasses(db).ToList());
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

    private static IEnumerable<ClassDeclaration> GetScriptableClasses(Database db) =>
        db.Classes
            .Where(clazz =>
                clazz.Attributes.Any(attr => attr.Name == "Scriptable") &&
                clazz.ShouldBeReflected);

    private static void AddScriptableClassSources(SourceProductionContext context, Database db)
    {
        var scriptableClasses = GetScriptableClasses(db);

        foreach (var scriptableClass in scriptableClasses)
        {
            if (scriptableClass.Name == null)
            {
                continue;
            }

            var name = StringFunctions.ToCamelCase(scriptableClass.Name);

            AddTemplateSource<ScriptableClassTemplate>(context, db, name, scriptableClass);
        }
    }

    private static void AddTemplateSource<T>(SourceProductionContext context, Database db, string name, params object[] parameters) where T : Template
    {
        using var stringWriter = new StringWriter();

        var parametersList = new List<object> { stringWriter };

        parametersList.AddRange(parameters);

        var template = Activator.CreateInstance(typeof(T), parametersList.ToArray()) as T;

        template!.Render(db);

        stringWriter.Flush();

        context.AddSource($"{name}.g.cs", SourceText.From(stringWriter.ToString(), Encoding.UTF8));
    }
}