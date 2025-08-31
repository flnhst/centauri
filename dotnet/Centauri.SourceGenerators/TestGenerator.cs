using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using Antares.SourceGenerators.Reflective;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;

namespace Antares.SourceGenerators;

[Generator]
public class TestGenerator : IIncrementalGenerator
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

        var renderedCode = TemplateHelper.RenderTemplate("Placeholder", new
        {
            Db = db
        });

        context.AddSource($"Placeholder.g.cs", SourceText.From(renderedCode, Encoding.UTF8));
    }
}