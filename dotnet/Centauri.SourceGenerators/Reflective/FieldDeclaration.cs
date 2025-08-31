namespace Antares.SourceGenerators.Reflective;

public class FieldDeclaration : Declaration
{
    public RType? Type { get; set; }

    public UInt64? TypeId { get; set; }
}
