using System.Text.Json.Serialization;

namespace Centauri.SourceGenerators.Reflective;

[JsonDerivedType(typeof(NamespaceDeclaration), "namespace")]
[JsonDerivedType(typeof(ClassDeclaration), "class")]
[JsonDerivedType(typeof(ClassTemplateDeclaration), "class_template")]
[JsonDerivedType(typeof(FunctionDeclaration), "function")]
[JsonDerivedType(typeof(MethodDeclaration), "method")]
[JsonDerivedType(typeof(FieldDeclaration), "field")]
[JsonDerivedType(typeof(ConstructorDeclaration), "constructor")]
[JsonDerivedType(typeof(EnumDeclaration), "enum")]
public class Declaration
{
    public string? Name { get; set; }

    public string? FullyQualifiedName { get; set; }

    public string? FullyQualifiedNameSlug { get; set; }

    public DeclarationClass Class { get; set; }

    public DeclarationVisibility Visibility { get; set; }

    public DeclarationContext? ParentContext { get; set; }

    public UInt64? ParentContextId { get; set; }

    public UInt64? Id { get; set; }

    public string? FilePath { get; set; }

    public UInt32? LineNumber { get; set; }

    public CommentBlock? CommentBlock { get; set; }

    public IList<Attribute> Attributes { get; set; } = new List<Attribute>();

    public bool ShouldBeReflected { get; set; }

    public bool IsTemplate { get; set; }

    public bool IsImported { get; set; }

    public string? ProjectName { get; set; }
}
