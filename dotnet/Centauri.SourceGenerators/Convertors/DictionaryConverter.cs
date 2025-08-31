using System.Text.Json;
using System.Text.Json.Serialization;

namespace Antares.SourceGenerators.Convertors;

public class DictionaryConverter<T> : JsonConverter<IDictionary<UInt64, T>>
{
    public override IDictionary<UInt64, T>? Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
    {
        var jsonElement = JsonDocument.ParseValue(ref reader).RootElement;

        if (jsonElement.ValueKind == JsonValueKind.Null)
        {
            return null;
        }

        if (jsonElement.ValueKind != JsonValueKind.Object)
        {
            throw new JsonException($"Failed to deserialize dictionary, unexpected JsonValueKind: '{jsonElement.ValueKind}'.");
        }

        var dictionary = new Dictionary<UInt64, T>();

        foreach (var jsonProperty in jsonElement.EnumerateObject())
        {
            var key = UInt64.Parse(jsonProperty.Name);
            var value = jsonProperty.Value.Deserialize<T>(options);

            if (value == null)
            {
                throw new JsonException($"Failed to deserialize dictionary, null item encountered at key '{key}'.");
            }

            dictionary.Add(key, value);
        }

        return dictionary;
    }

    public override void Write(Utf8JsonWriter writer, IDictionary<UInt64, T> value, JsonSerializerOptions options)
    {
        throw new NotImplementedException();
    }
}