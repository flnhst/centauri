namespace Centauri.SourceGenerators.Reflective;

public enum TemplateArgumentKind
{
    Unknown,
    Type,
    Integer,
    EnumT
}

public class TemplateArgument
{
    public TemplateArgumentKind Kind { get; set; }

    public UInt64? Integer { get; set; }

    public string? EnumName { get; set; }

    public RType? Type { get; set; }

    public UInt64? TypeId { get; set; }
}

public class RType
{
    public DeclarationContext? ParentContext { get; set; }

    public UInt64? ParentContextId { get; set; }

    public string? Name { get; set; }

    public TypeKind Kind { get; set; }

    public bool IsPointer { get; set; }

    public bool IsReference { get; set; }

    public bool IsConst { get; set; }

    public bool IsVolatile { get; set; }

    public bool IsBuiltin { get; set; }

    public Declaration? Decl { get; set; }

    public UInt64? DeclId { get; set; }

    public UInt64? Id { get; set; }

    public bool IsValid { get; set; }

    public bool IsImported { get; set; }

    public string? ProjectName { get; set; }

    public RType? InnerType { get; set; }

    public UInt64? InnerTypeId { get; set; }

    public RType? FunctionProtoReturnType { get; set; }

    public UInt64? FunctionProtoReturnTypeId { get; set; }

    public IList<RType> FunctionProtoArguments { get; set; } = new List<RType>();

    public IList<UInt64> FunctionProtoArgumentIds { get; set; } = new List<UInt64>();

    public IList<TemplateArgument> TemplateArguments { get; set; } = new List<TemplateArgument>();

    public IList<string> SourceFiles { get; set; } = new List<string>();

    public string? HashValueHex { get; set; }
}
