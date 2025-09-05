namespace Centauri.SourceGenerators.Reflective;

public class ConstructorDeclaration : MethodDeclaration
{
    public bool IsDefault { get; set; }

    public bool IsMove { get; set; }

    public bool IsCopy { get; set; }

    public bool IsTrivial { get; set; }

    public bool IsExplicit { get; set; }
}
