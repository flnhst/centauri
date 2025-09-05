namespace Centauri.SourceGenerators.Reflective;

public class Comment
{
    public string? FilePath { get; set; }

    public string? Text { get; set; }

    public UInt32 LineNumber { get; set; }
}