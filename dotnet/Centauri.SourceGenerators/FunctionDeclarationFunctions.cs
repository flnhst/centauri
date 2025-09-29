using System.Text;
using Centauri.SourceGenerators.Reflective;

namespace Centauri.SourceGenerators;

public static class FunctionDeclarationFunctions
{
    public static string GenerateNativeInterfaceParameterList(IList<KeyValuePair<string, RType?>> parameters, bool beginWithComma = false)
        => GenerateNativeInterfaceParameterListCore((argType, argName) => $"{argType} {argName}", parameters, beginWithComma);

    public static string GenerateNativeInterfaceParameterIgnoredList(IList<KeyValuePair<string, RType?>> parameters,
        bool beginWithComma = false)
        => GenerateNativeInterfaceParameterListCore((_, _) => "_", parameters, beginWithComma);

    private static string GenerateNativeInterfaceParameterListCore(Func<string, string, string> transformFunc,
        IList<KeyValuePair<string, RType?>> parameters,
        bool beginWithComma = false)
    {
        var sb = new StringBuilder();

        if (parameters.Count > 0 && beginWithComma)
        {
            sb.Append(", ");
        }

        for (var i = 0; i < parameters.Count; i++)
        {
            var name = parameters[i].Key;
            var rType = parameters[i].Value;

            if (rType == null)
            {
                throw new Exception($"Parameter {name} type is null.");
            }

            var type = RTypeHelper.RTypeToType(rType);

            if (type == typeof(String))
            {
                sb.Append($"{transformFunc("IntPtr", $"argPtr{i}")}, {transformFunc("int", $"argLength{i}")}");
            }
            else
            {
                sb.Append($"{transformFunc(type.Name, $"arg{i}")}");
            }

            if (i < parameters.Count - 1)
            {
                sb.Append(", ");
            }
        }

        return sb.ToString();
    }
}