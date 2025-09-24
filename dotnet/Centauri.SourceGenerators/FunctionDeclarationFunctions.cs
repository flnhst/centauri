using System.Text;
using Centauri.SourceGenerators.Reflective;

namespace Centauri.SourceGenerators;

public static class FunctionDeclarationFunctions
{
    public static string GenerateParameterList(IList<KeyValuePair<string, RType?>> parameters, bool beginWithComma = false)
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
                sb.Append($"IntPtr argPtr{i}, int argLength{i}");
            }
            else
            {
                sb.Append($"{type.Name} arg{i}");
            }

            if (i < parameters.Count - 1)
            {
                sb.Append(", ");
            }
        }

        return sb.ToString();
    }

    public static string GenerateParameterIgnoredList(IList<KeyValuePair<string, RType?>> parameters,
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
                sb.Append($"_, _");
            }
            else
            {
                sb.Append($"_");
            }

            if (i < parameters.Count - 1)
            {
                sb.Append(", ");
            }
        }

        return sb.ToString();
    }
}