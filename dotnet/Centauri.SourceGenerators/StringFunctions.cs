namespace Centauri.SourceGenerators;

public static class StringFunctions
{
    public static string ToCamelCase(string value)
        => value.Split('_')
            .Select(s => s.Substring(0, 1).ToUpper() + s.Substring(1))
            .Aggregate((a, b) => a + b);
}