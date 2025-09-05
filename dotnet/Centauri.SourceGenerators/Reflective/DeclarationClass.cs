using System.Text.Json.Serialization;

namespace Centauri.SourceGenerators.Reflective;

public enum DeclarationClass
{
    Unknown,
    Namespace,
    Class,
    ClassTemplate,
    Function,
    Method,
    Field,
    Constructor,
    Enum,
}