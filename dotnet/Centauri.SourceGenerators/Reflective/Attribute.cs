namespace Centauri.SourceGenerators.Reflective;

public class Attribute
{
    public string? RawAttribute { get; set; }

    public string? Name { get; set; }

    public IDictionary<string, string> Parameters { get; set; } = new Dictionary<string, string>();

    public bool Valid { get; set; }
}