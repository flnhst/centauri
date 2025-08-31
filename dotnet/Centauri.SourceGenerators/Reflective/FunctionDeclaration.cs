namespace Antares.SourceGenerators.Reflective;

public class FunctionDeclaration : Declaration
{
    public RType? ReturnType { get; set; }

    public UInt64? ReturnTypeId { get; set; }

    public IList<KeyValuePair<string, RType?>> Parameters { get; set; } = new List<KeyValuePair<string, RType?>>();

    public IList<KeyValuePair<string, UInt64?>> ParameterIds { get; set; } = new List<KeyValuePair<string, UInt64?>>();

    public bool HasNoDiscard { get; set; }
}
