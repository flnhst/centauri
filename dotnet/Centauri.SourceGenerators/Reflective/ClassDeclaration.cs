namespace Centauri.SourceGenerators.Reflective;

public class BaseSpecifier
{
    public ClassDeclaration? Base { get; set; }

    public UInt64? BaseId { get; set; }

    public DeclarationVisibility Visibility { get; set; }
}

public class TemplateParameter
{
    public RType? IntegerType { get; set; }

    public UInt64? IntegerTypeId { get; set; }

    public string? Name { get; set; }
}

public class ClassDeclaration : DeclarationContext
{
    public IList<BaseSpecifier> BaseSpecifiers { get; set; } = new List<BaseSpecifier>();

    public IList<ClassDeclaration> Children { get; set; } = new List<ClassDeclaration>();

    public IList<UInt64> ChildrenIds { get; set; } = new List<UInt64>();

    public IList<ConstructorDeclaration> Constructors { get; set; } = new List<ConstructorDeclaration>();

    public IList<UInt64> ConstructorIds { get; set; } = new List<UInt64>();

    public IList<MethodDeclaration> Methods { get; set; } = new List<MethodDeclaration>();

    public IList<UInt64> MethodIds { get; set; } = new List<UInt64>();

    public IList<FieldDeclaration> Fields { get; set; } = new List<FieldDeclaration>();

    public IList<UInt64> FieldIds { get; set; } = new List<UInt64>();

    public IList<TemplateParameter> TemplateParameters { get; set; } = new List<TemplateParameter>();

    public ClassDeclarationTag Tag { get; set; }

    public bool IsPod { get; set; }

    public bool IsAggregate  { get; set; }

    public bool IsAbstract { get; set; }
}