namespace Centauri.SourceGenerators.Reflective;

public class MethodDeclaration : FunctionDeclaration
{
    public bool IsConst { get; set; }

    public bool IsVirtual { get; set; }

    public bool IsPure { get; set; }

    public bool IsDeleted { get; set; }

    public bool IsDefaulted { get; set; }
}