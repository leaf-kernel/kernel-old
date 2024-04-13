#include <fs/path.h>

PathComponent *split_path(const char *path, int *num_components)
{
    if (path == NULL || num_components == NULL)
        return NULL;

    char *path_copy = strdup(path);
    if (path_copy == NULL)
        return NULL;

    PathComponent *components = (PathComponent *)kmalloc(sizeof(PathComponent));
    if (components == NULL)
        return NULL;

    int count = 0;
    char *token = strtok(path_copy, "/");

    while (token != NULL)
    {
        PathComponent *new_components = krealloc(components, (count + 1) * sizeof(PathComponent));
        if (new_components == NULL)
        {
            kfree(path_copy);
            kfree(components);
            *num_components = 0;
            return NULL;
        }

        components = new_components;

        components[count].name = strdup(token);
        if (components[count].name == NULL)
        {
            for (int i = 0; i < count; i++)
                kfree(components[i].name);
            kfree(components);
            kfree(path_copy);
            *num_components = 0;
            return NULL;
        }

        char *next_token = strtok(NULL, "/");
        components[count].directory = (next_token != NULL);

        if (path[strlen(path) - 1] == '/')
            components[count].directory = true;

        token = next_token;
        count++;
    }

    kfree(path_copy);
    *num_components = count;
    return components;
}

void free_components(PathComponent *components, int num_components)
{
    if (components == NULL)
        return;

    for (int i = 0; i < num_components; i++)
    {
        kfree(components[i].name);
    }

    kfree(components);
}