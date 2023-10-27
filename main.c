#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mongoc/mongoc.h>

struct node
{
    char *song_name;
    struct node *next;
};

/**
 * Displays the menu of options.
 */
void display_menu()
{
    printf("1. Create a new playlist\n");
    printf("2. Add a song to the playlist\n");
    printf("3. Delete a song from the playlist\n");
    printf("4. Display the playlist\n");
    printf("5. Play/Pause the current song\n");
    printf("6. Play the next song\n");
    printf("7. Play the previous song\n");
    printf("8. Shuffle the playlist\n");
    printf("9. Exit\n");
    printf("\n");
}

/**
 * Creates a new playlist.
 *
 * @param client A pointer to the MongoDB client.
 * @return A pointer to the head of the new playlist.
 */
struct node *create_playlist(mongoc_client_t *client)
{
    printf("Enter playlist name: ");
    char *playlist_name = (char *)malloc(sizeof(char) * 100);
    scanf("%s", playlist_name);

    // Insert playlist name into MongoDB
    bson_t *doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", playlist_name);
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "mydb", "playlists");
    mongoc_collection_insert_one(collection, doc, NULL, NULL);
    bson_destroy(doc);

    struct node *head = NULL;
    return head;
}

/**
 * Adds a song to the playlist.
 *
 * @param client A pointer to the MongoDB client.
 * @param head A pointer to the head of the playlist.
 * @param song_name The name of the song to add.
 * @return A pointer to the head of the updated playlist.
 */
struct node *add_song(mongoc_client_t *client, struct node *head, char *song_name)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->song_name = song_name;
    new_node->next = NULL;

    if (head == NULL)
    {
        head = new_node;
    }
    else
    {
        struct node *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }

    // Insert song name into MongoDB
    bson_t *doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", song_name);
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "mydb", "songs");
    mongoc_collection_insert_one(collection, doc, NULL, NULL);
    bson_destroy(doc);

    return head;
}

/**
 * Deletes a song from the playlist.
 *
 * @param client A pointer to the MongoDB client.
 * @param head A pointer to the head of the playlist.
 * @param song_name The name of the song to delete.
 * @return A pointer to the head of the updated playlist.
 */
struct node *delete_song(mongoc_client_t *client, struct node *head, char *song_name)
{
    struct node *current = head;
    struct node *previous = NULL;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->song_name, song_name) == 0)
        {
            if (previous == NULL)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            found = 1;
            break;
        }
        previous = current;
        current = current->next;
    }

    if (!found)
    {
        printf("Song not found\n");
    }
    else
    {
        // Delete song name from MongoDB
        bson_t *doc = bson_new();
        BSON_APPEND_UTF8(doc, "name", song_name);
        mongoc_collection_t *collection = mongoc_client_get_collection(client, "mydb", "songs");
        mongoc_collection_delete_one(collection, doc, NULL, NULL);
        bson_destroy(doc);
    }

    return head;
}

/**
 * Displays the contents of the playlist.
 *
 * @param head A pointer to the head of the playlist.
 */
void display(struct node *head)
{
    if (head == NULL)
    {
        printf("Playlist is empty\n");
    }
    else
    {
        struct node *current = head;
        while (current != NULL)
        {
            printf("%s\n", current->song_name);
            current = current->next;
        }
    }
}

/**
 * Plays or pauses the current song.
 *
 * @param head A pointer to the head of the playlist.
 * @param current_song The name of the current song.
 * @param is_playing A pointer to a boolean indicating whether the song is currently playing.
 * @return The name of the current song.
 */
char *play_pause(struct node *head, char *current_song, int *is_playing)
{
    struct node *current = head;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->song_name, current_song) == 0)
        {
            if (*is_playing)
            {
                printf("Pausing %s\n", current->song_name);
                *is_playing = 0;
            }
            else
            {
                printf("Playing %s\n", current->song_name);
                *is_playing = 1;
            }
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Song not found\n");
    }

    return current_song;
}

/**
 * Plays the next song in the playlist.
 *
 * @param head A pointer to the head of the playlist.
 * @param current_song The name of the current song.
 * @param is_playing A pointer to a boolean indicating whether the song is currently playing.
 * @return The name of the current song.
 */
char *play_next(struct node *head, char *current_song, int *is_playing)
{
    struct node *current = head;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->song_name, current_song) == 0)
        {
            if (current->next != NULL)
            {
                printf("Playing next song\n");
                current = current->next;
                printf("Playing %s\n", current->song_name);
                found = 1;
            }
            else
            {
                printf("End of playlist\n");
            }
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Song not found\n");
    }

    return current->song_name;
}

/**
 * Plays the previous song in the playlist.
 *
 * @param head A pointer to the head of the playlist.
 * @param current_song The name of the current song.
 * @param is_playing A pointer to a boolean indicating whether the song is currently playing.
 * @return The name of the current song.
 */
char *play_previous(struct node *head, char *current_song, int *is_playing)
{
    struct node *current = head;
    struct node *previous = NULL;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->song_name, current_song) == 0)
        {
            if (previous != NULL)
            {
                printf("Playing previous song\n");
                printf("Playing %s\n", previous->song_name);
                found = 1;
            }
            else
            {
                printf("Beginning of playlist\n");
            }
            break;
        }
        previous = current;
        current = current->next;
    }

    if (!found)
    {
        printf("Song not found\n");
    }

    return previous->song_name;
}

/**
 * Shuffles the playlist.
 *
 * @param head A pointer to the head of the playlist.
 */
void shuffle(struct node *head)
{
    int count = 0;
    struct node *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    srand(time(NULL));
    for (int i = 0; i < count; i++)
    {
        int j = rand() % count;
        int k = rand() % count;

        struct node *node1 = head;
        struct node *node2 = head;
        for (int x = 0; x < j; x++)
        {
            node1 = node1->next;
        }
        for (int x = 0; x < k; x++)
        {
            node2 = node2->next;
        }

        char *temp = node1->song_name;
        node1->song_name = node2->song_name;
        node2->song_name = temp;
    }

    printf("Shuffled playlist\n");
}

/**
 * Main function that runs the program.
 *
 * @return 0 if the program runs successfully.
 */
int main()
{
    mongoc_init();

    const char *uri_string = "mongodb://localhost:27017";
    mongoc_uri_t *uri = mongoc_uri_new_with_error(uri_string, NULL);
    mongoc_client_t *client = mongoc_client_new_from_uri(uri);

    struct node *head = NULL;
    char *current_song = NULL;
    int is_playing = 0;

    int choice;
    do
    {
        display_menu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            head = create_playlist(client);
            break;

        case 2:
            printf("Enter song name: ");
            char *song_name = (char *)malloc(sizeof(char) * 100);
            scanf("%s", song_name);
            head = add_song(client, head, song_name);
            break;

        case 3:
            printf("Enter song name: ");
            char *song_name_to_delete = (char *)malloc(sizeof(char) * 100);
            scanf("%s", song_name_to_delete);
            head = delete_song(client, head, song_name_to_delete);
            break;

        case 4:
            display(head);
            break;

        case 5:
            if (current_song == NULL)
            {
                printf("No song playing\n");
            }
            else
            {
                current_song = play_pause(head, current_song, &is_playing);
            }
            break;

        case 6:
            if (current_song == NULL)
            {
                if (head == NULL)
                {
                    printf("Playlist is empty\n");
                }
                else
                {
                    current_song = head->song_name;
                    printf("Playing %s\n", current_song);
                }
            }
            else
            {
                current_song = play_next(head, current_song, &is_playing);
            }
            break;

        case 7:
            if (current_song == NULL)
            {
                printf("No song playing\n");
            }
            else
            {
                current_song = play_previous(head, current_song, &is_playing);
            }
            break;

        case 8:
            shuffle(head);
            break;

        case 9:
            exit(0);
        }

    } while (choice != 9);

    mongoc_client_destroy(client);
    mongoc_uri_destroy(uri);
    mongoc_cleanup();

    return 0;
}
