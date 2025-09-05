namespace Centauri.SourceGenerators.Reflective;

public class NamespaceDeclaration : DeclarationContext
{
    public IList<NamespaceDeclaration> Children { get; set; } = new List<NamespaceDeclaration>();

    public IList<UInt64?> ChildrenIds { get; set; } = new List<UInt64?>();

    public bool IsGlobalNamespace { get; set; }
}