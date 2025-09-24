using Centauri.SourceGenerators.Reflective;
using static Centauri.SourceGenerators.DeclarationFunctions;
using static Centauri.SourceGenerators.StringFunctions;
using static Centauri.SourceGenerators.FunctionDeclarationFunctions;

namespace Centauri.SourceGenerators.Templates;

public class CallbacksTemplate(TextWriter writer, IList<ClassDeclaration> scriptableClasses) : Template(writer)
{
    public override void Render(Database database)
    {
        EmitCommentHeader(database);
        EmitFileScopedNamespace($"{ToCamelCase(database.ProjectName)}.Generated");
        EmitLine();
        BeginClass("Callbacks");
        foreach (var scriptableClass in scriptableClasses)
        {
            RenderScriptableClassCallbacks(scriptableClass);
        }
        EndClass();
    }

    private void RenderScriptableClassCallbacks(ClassDeclaration scriptableClass)
    {
        foreach (var constructor in scriptableClass.Constructors)
        {
            RenderScriptableClassConstructorCallbacks(scriptableClass, constructor);
        }
    }

    private void RenderScriptableClassConstructorCallbacks(ClassDeclaration scriptableClass, ConstructorDeclaration constructor)
    {
        if (!constructor.ShouldBeReflected)
        {
            return;
        }

        EmitLine($"public delegate IntPtr {GenerateDotnetDelegateName(scriptableClass)}(IntPtr objHandle{GenerateParameterList(constructor.Parameters, true)});");
        EmitLine($"public static {GenerateDotnetDelegateName(scriptableClass)} {GenerateDotnetCallbackName(scriptableClass)} = (_{GenerateParameterIgnoredList(constructor.Parameters, true)}) => throw new NotImplementedException();");
    }
}