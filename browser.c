#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 10000

int main()
{
    char filename[100];
    printf("Enter HTML file name (e.g., page.html): ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    char *buffer = malloc(MAX_FILE_SIZE);
    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    size_t length = fread(buffer, 1, MAX_FILE_SIZE - 1, file);
    buffer[length] = '\0';

    printf("\n--- Rendered Content ---\n");

    char *ptr = buffer;
    while (*ptr)
    {
        if (*ptr == '<')
        {
            if (strncmp(ptr, "<p>", 3) == 0)
            {
                printf("\n");
                ptr += 3;
            }
            else if (strncmp(ptr, "<h1>", 4) == 0)
            {
                printf("\n# ");
                ptr += 4;
            }
            else if (strncmp(ptr, "</h1>", 5) == 0)
            {
                printf("\n");
                ptr += 5;
            }
            else if (strncmp(ptr, "</p>", 4) == 0)
            {
                ptr += 4;
            }
            else if (strncmp(ptr, "<a href='", 9) == 0)
            {
                ptr += 9;
                char link[100];
                int i = 0;
                while (*ptr && *ptr != '\'')
                {
                    link[i++] = *ptr++;
                }
                link[i] = '\0';
                while (*ptr && *ptr != '>')
                    ptr++; // skip to end of tag
                ptr++;     // skip '>'

                // Read link text
                char text[100];
                i = 0;
                while (*ptr && strncmp(ptr, "</a>", 4) != 0)
                {
                    text[i++] = *ptr++;
                }
                text[i] = '\0';
                ptr += 4; // skip "</a>"

                printf("[Link] %s -> %s\n", text, link);
            }
            else
            {
                // skip unknown tags
                while (*ptr && *ptr != '>')
                    ptr++;
                ptr++;
            }
        }
        else
        {
            printf("%c", *ptr++);
        }
    }

    fclose(file);
    free(buffer);

    return 0;
}
