namespace Antares.SourceGenerators.Reflective;

public class DeclarationContext : Declaration
{
    public IList<Declaration> Declarations { get; set; } = new List<Declaration>();

    public IList<UInt64> DeclarationIds { get; set; } = new List<UInt64>();
}
