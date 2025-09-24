using Centauri.SourceGenerators.Reflective;
using Microsoft.CodeAnalysis;
using static Centauri.SourceGenerators.DeclarationFunctions;
using static Centauri.SourceGenerators.StringFunctions;

namespace Centauri.SourceGenerators;

public class ScriptableClassTemplate(TextWriter writer, ClassDeclaration scriptableClass) : Template(writer)
{
    public override void Render(Database database)
    {
        if (scriptableClass.Name == null)
        {
            throw new ArgumentException("Scriptable class must have a name.");
        }

        EmitCommentLine($"Reflective database was generated on: {database.GeneratedOn:O}");
        EmitLine();
        EmitUsing("Centauri");
        EmitLine();
        EmitFileScopedNamespace($"{ToCamelCase(database.ProjectName)}.Generated");
        EmitLine();
        BeginClass(ToCamelCase(scriptableClass.Name), inherits: "CentauriObject");
        EndClass();
    }
}