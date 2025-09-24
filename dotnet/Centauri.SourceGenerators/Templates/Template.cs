using System.Reflection;
using Centauri.SourceGenerators.Reflective;
using Microsoft.CodeAnalysis;

namespace Centauri.SourceGenerators.Templates;

public abstract class Template(TextWriter writer) : IDisposable
{
    public const int IndentationWidth = 4;

    private int _indent;
    private bool _lineIndented;

    public abstract void Render(Database database);

    public void Dispose()
    {
        writer.Dispose();
    }

    protected void IndentLine()
    {
        if (_lineIndented)
        {
            return;
        }

        writer.Write(new string(' ', _indent));

        _lineIndented = true;
    }

    protected void AddIndent()
    {
        _indent += IndentationWidth;
    }

    protected void RemoveIndent()
    {
        if (_indent - IndentationWidth < 0)
        {
            throw new OverflowException("Indent out of range.");
        }

        _indent -= IndentationWidth;
    }

    protected void Emit(string text)
    {
        IndentLine();

        writer.Write(text);
    }

    protected void Emit(char ch)
    {
        IndentLine();

        writer.Write(ch);
    }

    protected void EmitLine(string text)
    {
        IndentLine();

        writer.WriteLine(text);

        _lineIndented = false;
    }

    protected void EmitLine()
    {
        EmitLine(string.Empty);
    }

    protected void BeginScopeBlock()
    {
        EmitLine("{");
        AddIndent();
    }

    protected void EndScopeBlock()
    {
        RemoveIndent();

        EmitLine("}");
    }

    protected void BeginCommentBlock()
    {
        EmitLine("/*");
    }

    protected void EndCommentBlock()
    {
        EmitLine("*/");
    }

    protected void EmitCommentLine(string text)
    {
        EmitLine($"// {text}");
    }

    protected void EmitUsing(string @namespace)
    {
        EmitLine($"using {@namespace};");
    }

    protected void EmitFileScopedNamespace(string @namespace)
    {
        EmitLine($"namespace {@namespace};");
    }

    protected void BeginClass(string className, Accessibility accessibility = Accessibility.Public, bool isStatic = false, string? inherits = null)
    {
        var accessibilityString = AccessibilityToString(accessibility);

        Emit($"{accessibilityString} " );

        if (isStatic)
        {
            Emit("static ");
        }

        Emit($"class {className}");

        if (!string.IsNullOrEmpty(inherits))
        {
            Emit(" : " + inherits);
        }

        EmitLine();
        BeginScopeBlock();
    }

    protected void EndClass()
    {
        EndScopeBlock();
    }

    protected void BeginMethod(string returnType, string methodName, string parameters = "", Accessibility accessibility = Accessibility.Public,
        bool isStatic = false)
    {
        var accessibilityString = AccessibilityToString(accessibility);

        Emit($"{accessibilityString} ");

        if (isStatic)
        {
            Emit("static ");
        }

        Emit($"{returnType} ");

        EmitLine($"{methodName}({parameters})");
        BeginScopeBlock();
    }

    protected void EndMethod()
    {
        EndScopeBlock();
    }

    protected void EmitCommentHeader(Database database)
    {
        EmitCommentLine($"Reflective database was generated on: {database.GeneratedOn:O}");
        EmitCommentLine($"This file was generated on:           {DateTime.UtcNow:O}");
        EmitLine();
    }

    private string AccessibilityToString(Accessibility accessibility) =>
        accessibility switch
        {
            Accessibility.Public => "public",
            Accessibility.Internal => "internal",
            Accessibility.Protected => "protected",
            Accessibility.ProtectedAndInternal => "protected internal",
            _ => throw new ArgumentOutOfRangeException(nameof(accessibility), accessibility, null)
        };
}