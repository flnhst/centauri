namespace Antares.SourceGenerators.Reflective;

public class DatabaseLinker
{
    public static void Link(Database database)
    {
        database.DeclarationsMap = database.Declarations
            .Where(decl => decl.Id != null)
            .ToDictionary(decl => decl.Id!.Value, decl => decl);

        database.TypesMap = database.Types
            .Where(type => type.Id != null)
            .ToDictionary(type => type.Id!.Value, type => type);

        database.Classes = database.Declarations
            .Where(decl => decl is ClassDeclaration)
            .Cast<ClassDeclaration>()
            .ToList();

        database.GlobalNamespace = database.Declarations
            .Where(decl => decl is NamespaceDeclaration { IsGlobalNamespace: true })
            .Cast<NamespaceDeclaration>()
            .SingleOrDefault();

        foreach (var declaration in database.Declarations)
        {
            LinkDeclaration(database, declaration);

            if (declaration is DeclarationContext declarationContext)
            {
                LinkDeclarationContext(database, declarationContext);
            }

            if (declaration is ClassDeclaration classDeclaration)
            {
                LinkClassDeclaration(database, classDeclaration);
            }

            if (declaration is NamespaceDeclaration namespaceDeclaration)
            {
                LinkNamespaceDeclaration(database, namespaceDeclaration);
            }

            if (declaration is FieldDeclaration fieldDeclaration)
            {
                LinkFieldDeclaration(database, fieldDeclaration);
            }

            if (declaration is FunctionDeclaration functionDeclaration)
            {
                LinkFunctionDeclaration(database, functionDeclaration);
            }

            // TODO: MethodDeclaration

            // TODO: ConstructorDeclaration
        }

        foreach (var type in database.Types)
        {
            LinkType(database, type);
        }
    }

    private static void LinkDeclaration(Database database, Declaration declaration)
    {
        if (declaration.ParentContextId != null &&
            database.DeclarationsMap.TryGetValue(declaration.ParentContextId.Value, out var parent) &&
            parent is DeclarationContext declarationContext)
        {
            declaration.ParentContext = declarationContext;
        }
    }

    private static void LinkDeclarationContext(Database database, DeclarationContext declarationContext)
    {
        foreach (var id in declarationContext.DeclarationIds)
        {
            if (database.DeclarationsMap.TryGetValue(id, out var declaration))
            {
                declarationContext.Declarations.Add(declaration);
            }
        }
    }

    private static void LinkClassDeclaration(Database database, ClassDeclaration classDeclaration)
    {
        foreach (var baseSpecifier in classDeclaration.BaseSpecifiers)
        {
            if (baseSpecifier.BaseId != null &&
                database.DeclarationsMap.TryGetValue(baseSpecifier.BaseId.Value, out var declaration) &&
                declaration is ClassDeclaration baseClassDeclaration)
            {
                baseSpecifier.Base = baseClassDeclaration;
            }
        }

        foreach (var childId in classDeclaration.ChildrenIds)
        {
            if (database.DeclarationsMap.TryGetValue(childId, out var declaration) &&
                declaration is ClassDeclaration childClassDeclaration)
            {
                classDeclaration.Children.Add(childClassDeclaration);
            }
        }

        foreach (var constructorId in classDeclaration.ConstructorIds)
        {
            if (database.DeclarationsMap.TryGetValue(constructorId, out var declaration) &&
                declaration is ConstructorDeclaration constructorDeclaration)
            {
                classDeclaration.Constructors.Add(constructorDeclaration);
            }
        }

        foreach (var methodId in classDeclaration.MethodIds)
        {
            if (database.DeclarationsMap.TryGetValue(methodId, out var declaration) &&
                declaration is MethodDeclaration methodDeclaration)
            {
                classDeclaration.Methods.Add(methodDeclaration);
            }
        }

        foreach (var fieldId in classDeclaration.FieldIds)
        {
            if (database.DeclarationsMap.TryGetValue(fieldId, out var declaration) &&
                declaration is FieldDeclaration fieldDeclaration)
            {
                classDeclaration.Fields.Add(fieldDeclaration);
            }
        }
    }

    private static void LinkNamespaceDeclaration(Database database, NamespaceDeclaration namespaceDeclaration)
    {
        foreach (var childId in namespaceDeclaration.ChildrenIds)
        {
            if (childId != null &&
                database.DeclarationsMap.TryGetValue(childId.Value, out var declaration) &&
                declaration is NamespaceDeclaration childNamespaceDeclaration)
            {
                namespaceDeclaration.Children.Add(childNamespaceDeclaration);
            }
        }
    }

    private static void LinkFieldDeclaration(Database database, FieldDeclaration fieldDeclaration)
    {
        if (fieldDeclaration.TypeId != null &&
            database.TypesMap.TryGetValue(fieldDeclaration.TypeId.Value, out var type))
        {
            fieldDeclaration.Type = type;
        }
    }

    private static void LinkFunctionDeclaration(Database database, FunctionDeclaration functionDeclaration)
    {
        if (functionDeclaration.ReturnTypeId != null &&
            database.TypesMap.TryGetValue(functionDeclaration.ReturnTypeId.Value, out var returnType))
        {
            functionDeclaration.ReturnType = returnType;
        }

        foreach (var parameterKeyValuePair in functionDeclaration.ParameterIds)
        {
            var key = parameterKeyValuePair.Key;
            var value = parameterKeyValuePair.Value;

            if (value != null)
            {
                database.TypesMap.TryGetValue(value.Value, out var parameterType);

                functionDeclaration.Parameters.Add(new KeyValuePair<string, RType?>(key, parameterType));
            }
        }
    }

    private static void LinkType(Database database, RType type)
    {
        if (type.ParentContextId != null &&
            database.DeclarationsMap.TryGetValue(type.ParentContextId.Value, out var declaration) &&
            declaration is DeclarationContext declarationContext)
        {
            type.ParentContext = declarationContext;
        }

        if (type.DeclId != null &&
            database.DeclarationsMap.TryGetValue(type.DeclId.Value, out var decl))
        {
            type.Decl = decl;
        }

        if (type.InnerTypeId != null &&
            database.TypesMap.TryGetValue(type.InnerTypeId.Value, out var innerType))
        {
            type.InnerType = innerType;
        }

        if (type.FunctionProtoReturnTypeId != null &&
            database.TypesMap.TryGetValue(type.FunctionProtoReturnTypeId.Value, out var functionProtoReturnType))
        {
            type.FunctionProtoReturnType = functionProtoReturnType;
        }

        foreach (var typeFunctionProtoArgumentId in type.FunctionProtoArgumentIds)
        {
            if (database.TypesMap.TryGetValue(typeFunctionProtoArgumentId, out var functionProtoArgument))
            {
                type.FunctionProtoArguments.Add(functionProtoArgument);
            }
        }

        foreach (var templateArgument in type.TemplateArguments)
        {
            if (templateArgument.Kind == TemplateArgumentKind.Type &&
                templateArgument.TypeId != null &&
                database.TypesMap.TryGetValue(templateArgument.TypeId.Value, out var templateType))
            {
                templateArgument.Type = templateType;
            }
        }
    }
}
