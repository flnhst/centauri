using System.Text.Json;
using System.Text.Json.Serialization;
using Centauri.Generated;
using Centauri.SourceGenerators.Reflective;

const string dbJsonFilePath = @"..\..\..\..\..\build_debug\reflective\centauri_generated\db.json";

await using var stream = File.OpenRead(dbJsonFilePath);

var options = new JsonSerializerOptions()
{
    PropertyNamingPolicy = JsonNamingPolicy.SnakeCaseLower,
    Converters =
    {
        new JsonStringEnumConverter(JsonNamingPolicy.SnakeCaseLower)
    }
};

Console.WriteLine("Deserializing...");

var database = await JsonSerializer.DeserializeAsync<Database>(stream, options);

Console.WriteLine("Deserialized.");

if (database == null)
{
    Console.WriteLine("Database is null.");

    return 1;
}

Console.WriteLine("Linking database...");

DatabaseLinker.Link(database);

Console.WriteLine("Linked database.");

Console.WriteLine("Done.");

var entity = new Entity();

return 0;