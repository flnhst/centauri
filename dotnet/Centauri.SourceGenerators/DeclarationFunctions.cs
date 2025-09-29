using Centauri.SourceGenerators.Reflective;

namespace Centauri.SourceGenerators;

public static class DeclarationFunctions
{
    // NOTE: Synchronize with C++.
    public static string GenerateDotnetNewName(ClassDeclaration classDeclaration)
        => $"dotnet_new_{classDeclaration.FullyQualifiedNameSlug}";

    public static string GenerateDotnetDelegateName(ClassDeclaration classDeclaration, ConstructorDeclaration constructor)
    {
        if (classDeclaration.Name == null)
        {
            throw new ArgumentException("Class name cannot be null.", nameof(classDeclaration));
        }

        return $"{StringFunctions.ToCamelCase(classDeclaration.Name)}NewDelegate";
    }

    public static string GenerateDotnetCallbackName(ClassDeclaration classDeclaration)
    {
        if (classDeclaration.Name == null)
        {
            throw new ArgumentException("Class name cannot be null.", nameof(classDeclaration));
        }

        return $"{StringFunctions.ToCamelCase(classDeclaration.Name)}NewCallback";
    }
}