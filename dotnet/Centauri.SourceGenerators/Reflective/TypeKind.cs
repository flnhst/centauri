namespace Centauri.SourceGenerators.Reflective;

public enum TypeKind
{
    Unknown,
    TemplateSpecialization,
    Elaborated,
    Record,
    LValueReference,
    RValueReference,
    Alias,
    Pointer,
    Builtin,
    EnumT,
    FunctionProto,
    Paren
}
