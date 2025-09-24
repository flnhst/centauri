using System.Reflection;

namespace Centauri.SourceGenerators.Reflective;

public static class RTypeHelper
{
    public static Type RTypeToType(RType rType)
    {
        if (rType.IsBuiltin)
        {
            switch (rType.Name)
            {
                case "float":
                    return typeof(float);
                case "char":
                    return typeof(Byte);
                case "int":
                    return typeof(Int32);
                case "unsigned long long":
                    return typeof(UInt64);
                case "void":
                    return typeof(void);
            }
        }

        if (rType is { IsPointer: true, InnerType: not null })
        {
            var innerType = RTypeToType(rType.InnerType);

            return innerType.MakePointerType();
        }

        if (rType is { Kind: TypeKind.TemplateSpecialization, Name: "::std::basic_string" })
        {
            return typeof(String);
        }

        if (rType.InnerType != null)
        {
            return RTypeToType(rType.InnerType);
        }

        throw new Exception($"Unknown Reflective '{rType.Kind}' type '{rType.Name}'.");
    }
}