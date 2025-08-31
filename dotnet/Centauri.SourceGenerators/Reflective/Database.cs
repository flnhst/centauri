using System.Text.Json.Serialization;
using Antares.SourceGenerators.Convertors;

namespace Antares.SourceGenerators.Reflective;

public class Database
{
    public string ProjectName { get; set; } = string.Empty;

    [JsonConverter(typeof(UnixTimestampConverter))]
    public DateTimeOffset GeneratedOn { get; set; }

    public IList<Declaration> Declarations { get; set; } = new List<Declaration>();

    public IList<RType> Types { get; set; } = new List<RType>();

    [JsonIgnore]
    public NamespaceDeclaration? GlobalNamespace { get; set; }

    [JsonIgnore]
    public IList<ClassDeclaration> Classes { get; set; } = new List<ClassDeclaration>();

    [JsonIgnore]
    public IDictionary<UInt64, Declaration> DeclarationsMap { get; set; } = new Dictionary<UInt64, Declaration>();

    [JsonIgnore]
    public IDictionary<UInt64, RType> TypesMap { get; set; } = new Dictionary<UInt64, RType>();
}
