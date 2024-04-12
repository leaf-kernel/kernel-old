#include <fs/path.h>

PathComponent *split_path(const char *path, int *num_components)
{
    if (path == NULL || num_components == NULL)
        return NULL;

    char *path_copy = strdup(path);
    if (path_copy == NULL)
        return NULL;

    char *token = strtok(path_copy, "/");
    PathComponent *components = NULL;
    int count = 0;

    while (token != NULL)
    {
        components = krealloc(components, (count + 1) * sizeof(PathComponent));
        if (components == NULL)
        {
            kfree(path_copy);
            return NULL;
        }

        components[count].name = strdup(token);

        char *next_token = strtok(NULL, "/");
        if (next_token != NULL)
            components[count].directory = 1;
        else
            components[count].directory = 0;

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
        kfree(components[i].name);
    kfree(components);
}