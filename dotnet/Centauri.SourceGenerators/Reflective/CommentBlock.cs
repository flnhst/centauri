namespace Antares.SourceGenerators.Reflective;

public class CommentBlock
{
    public IList<Comment> Comments { get; set; } = new List<Comment>();

    public UInt32 StartLine { get; set; }

    public UInt32 EndLine { get; set; }
}