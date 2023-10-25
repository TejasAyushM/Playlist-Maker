#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

typedef struct node {
    struct node* next;
    struct node* prev;
    char song[1000];
    int ran;
    int pos;
} NODE;

typedef struct link {
    NODE* head;
} LINK;

NODE* createNode(char* s) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    strcpy(newNode->song, s);
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->pos = 0;
    newNode->ran = 0;
    return newNode;
}

void insertNode(LINK* p, char* s) {
    NODE* newNode = createNode(s);
    if (p->head == NULL) {
        p->head = newNode;
        newNode->pos = 1;
        newNode->next = newNode;
        newNode->prev = newNode;
    } 
    else 
    {
        NODE* t1 = p->head;
        NODE* last = t1->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = t1;
        t1->prev = newNode;
        newNode->pos = last->pos + 1;
    }
}

void deletebyName(LINK* p, char* s) {
    if (p->head == NULL) {
        printf("Playlist Empty");
    } else {
        NODE* t1 = p->head;
        do {
            if (strcmp(t1->song, s) == 0) {
                if (t1 == p->head && t1->next == p->head) {
                    printf("Deleted song %s", t1->song);
                    free(t1);
                    p->head = NULL;
                } else if (t1 == p->head) {
                    p->head = t1->next;
                    t1->prev->next = t1->next;
                    t1->next->prev = t1->prev;
                    printf("Deleted song %s", t1->song);
                    free(t1);
                } else {
                    t1->prev->next = t1->next;
                    t1->next->prev = t1->prev;
                    printf("Deleted song %s", t1->song);
                    free(t1);
                }
                return;
            }
            t1 = t1->next;
        } while (t1 != p->head);
        printf("Song not found in the playlist");
    }
}

void display(LINK* p) {
    if (p->head == NULL) {
        printf("Playlist Empty");
    } else {
        NODE* t = p->head;
        do {
            printf("%d. %s\n", t->pos, t->song);
            t = t->next;
        } while (t != p->head);
    }
}

int count_songs(LINK* p) {
    int count = 0;
    if (p->head != NULL) {
        NODE* t = p->head;
        do {
            count++;
            t = t->next;
        } while (t != p->head);
    }
    return count;
}

void play(NODE* p) {
    if (p != NULL) {
        printf("Now playing %s\n", p->song);
    } else {
        printf("No song to play\n");
    }
}

void playbyName(LINK* p, char* s) {
    if (p->head == NULL) {
        printf("Playlist Empty");
    } else {
        NODE* t1 = p->head;
        do {
            if (strcmp(t1->song, s) == 0) {
                play(t1);
                return;
            }
            t1 = t1->next;
        } while (t1 != p->head);
        printf("Song not found in the playlist");
    }
}

NODE* playnext(NODE* p) {
    if (p != NULL) {
        p = p->next;
        play(p);
    } else {
        printf("No song to play next\n");
    }
    return p;
}

NODE* playprev(NODE* p) {
    if (p != NULL) {
        p = p->prev;
        play(p);
    } else {
        printf("No song to play previous\n");
    }
    return p;
}

int generateRandom(int min, int max) 
{
    static int initialized = 0; // To ensure srand is called only once
    if (!initialized)
    {
        srand(time(NULL)); // Set the seed based on the current time
        initialized = 1;
    }
    return min + rand() % (max - min + 1);
}

void assign_random(LINK* p){
    NODE* t= p->head;
    do{
        t->ran=generateRandom(1,count_songs(p));
        t=t->next;
    }while(t!=p->head);
}

// Function to sort the circular doubly linked list using Selection Sort based on "ran" value
void selectionSort(LINK* p) {
    if (p->head == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    NODE* current = p->head;

    do {
        NODE* min = current;
        NODE* innerCurrent = current->next;

        do {
            if (innerCurrent->ran < min->ran) {
                min = innerCurrent;
            }
            innerCurrent = innerCurrent->next;
        } while (innerCurrent != p->head);

        // Swap the "ran" values
        int temp = current->ran;
        current->ran = min->ran;
        min->ran = temp;

        // Move the current node to the next position in the circular list
        current = current->next;
    } while (current != p->head);
}

void shuffle(LINK* p) {
  if (p->head == NULL) {
    printf("Playlist is empty.\n");
    return;
  }

  int num_songs = count_songs(p);

  // Generate a random number between 0 and the number of songs in the playlist.
  int random_index = rand() % num_songs;

  // Set the current node to the head of the playlist.
  NODE* current = p->head;

  // Move the current node forward the random number of steps.
  for (int i = 0; i < random_index; i++) {
    current = current->next;
  }

  // Swap the current node with the node at the current position.
  NODE* temp = current;
  current = current->next;
  current->next = temp;
  temp->next = current->prev;
  current->prev = temp->prev;
  temp->prev = current;

  // Move the current node to the head of the playlist.
  p->head = current;

  // Repeat the process until you have reached the end of the playlist.
  while (current->next != p->head) {
    // Generate a random number between 0 and the number of songs in the playlist.
    random_index = rand() % num_songs;

    // Move the current node forward the random number of steps.
    for (int i = 0; i < random_index; i++) {
      current = current->next;
    }

    // Swap the current node with the node at the current position.
    temp = current;
    current = current->next;
    current->next = temp;
    temp->next = current->prev;
    current->prev = temp->prev;
    temp->prev = current;
  }
}

void shuffle1(LINK* p) {
  if (p->head == NULL) {
    printf("Playlist is empty.\n");
    return;
  }

  // Generate a random number between 0 and the number of songs in the playlist.
  int num_songs = count_songs(p);
  int random_index = rand() % num_songs;

  // Set the current node to the head of the playlist.
  NODE* current = p->head;

  // Move the current node forward the random number of steps.
  for (int i = 0; i < random_index; i++) {
    current = current->next;
  }

  // Swap the current node with the node at the head of the playlist.
  NODE* temp = current;
  current = p->head;
  p->head = temp;

  // Update the pointers of the two nodes.
  current->next = temp->next;
  temp->next = current->prev;
  current->prev = temp->prev;
  temp->prev = current;

  // Repeat the process until you have reached the end of the playlist.
  while (current->next != p->head) {
    // Generate a random number between 0 and the number of songs in the playlist.
    random_index = rand() % num_songs;

    // Move the current node forward the random number of steps.
    for (int i = 0; i < random_index; i++) {
      current = current->next;
    }

    // Swap the current node with the node at the current position.
    temp = current;
    current = current->next;
    current->next = temp->next;
    temp->next = current->prev;
    current->prev = temp->prev;
    temp->prev = current;
  }
}

int main()
{
    printf("WELCOME TO YOUR PERSONAL MUSIC PLAYER");
    LINK obj;
    obj.head= NULL;
    NODE* hold=obj.head;
    int choice;
    char s[1000];
    int num;
    char s1[1000];
    char s2[1000];
    do
    {
        printf("\n1.Add New Song\n2.Delete Song\n3.Display Playlist\n4.Count number of songs\n5.Play Song\n6.Play next song\n7.Play previous song\n8.Shuffle playlist\n9.Exit\n\n");
        printf("Enter your choice");
        scanf("%d",&choice);
        switch(choice)

        {
            case 1:
                
                printf("Enter the Song name");
                scanf("%s", s);
                insertNode(&obj, s);
            break;

            case 2:
                
                printf("enter song to be deleted");
                scanf("%s",s1);
                deletebyName(&obj,s1);
            break;

            case 3:
                display(&obj);
            break;  

            case 4:
                printf("total number of songs are %d",count_songs(&obj));
            break;

            case 5:
                
                printf("Enter song name");
                scanf("%s",s2);
                playbyName(&obj,s2);
            break;

            case 6:
                playnext(obj.head);
            break;

            case 7:
                playprev(obj.head);
            break;

            case 8:
                shuffle1(&obj);
                printf("Shuffled your playlist");
                display(&obj);
            break;

            case 9:
            exit(0);
        }

    } while (choice!=9);
    
}

