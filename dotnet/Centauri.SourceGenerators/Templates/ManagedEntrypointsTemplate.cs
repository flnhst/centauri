using Centauri.SourceGenerators.Reflective;
using Microsoft.CodeAnalysis;
using static Centauri.SourceGenerators.DeclarationFunctions;
using static Centauri.SourceGenerators.StringFunctions;

namespace Centauri.SourceGenerators.Templates;

public class ManagedEntrypointsTemplate(TextWriter writer, IList<ClassDeclaration> scriptableClasses) : Template(writer)
{
    public override void Render(Database database)
    {
        EmitCommentHeader(database);
        EmitUsing("System.Runtime.InteropServices");
        EmitLine();
        EmitFileScopedNamespace($"{ToCamelCase(database.ProjectName)}.Generated");
        EmitLine();
        BeginClass("ManagedEntrypoints", Accessibility.Public, true);
        RenderInitializeBindingsMethod();
        EndClass();
    }

    private void RenderInitializeBindingsMethod()
    {
        EmitLine("[UnmanagedCallersOnly]");
        BeginMethod("void", "InitializeBindings", "IntPtr nativeCallbacksPtr", Accessibility.Public, true);
        EmitLine("Console.WriteLine(\"Hello, from generated .NET!\");");
        EmitLine();
        EmitLine("var handle = GCHandle.FromIntPtr(nativeCallbacksPtr);");
        EmitLine();
        EmitLine("if (handle.Target is not IDictionary<string, IntPtr> nativeCallbacks)");
        BeginScopeBlock();
        EmitLine("throw new Exception(\"Invalid argument.\");");
        EndScopeBlock();
        EmitLine();
        foreach (var scriptableClass in scriptableClasses)
        {
            BeginScopeBlock();
            EmitLine($"var fptrRaw = nativeCallbacks[\"{GenerateDotnetNewName(scriptableClass)}\"];");
            EmitLine($"var callbackDelegate = Marshal.GetDelegateForFunctionPointer<Callbacks.{GenerateDotnetDelegateName(scriptableClass)}>(fptrRaw);");
            EmitLine($"Callbacks.{GenerateDotnetCallbackName(scriptableClass)} = callbackDelegate;");
            EndScopeBlock();
            EmitLine();
        }
        EndMethod();
    }
}