#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node
{
    struct node *next; 
    struct node *prev; 
    char song[1000];   
    int id;            
}NODE;

typedef struct link
{
    NODE *head;        
    NODE *cur; 
}LINK;

void printMenu()
{
    printf("\n\033[1;34m");
    printf("1. Add new song\n");
    printf("2. Delete a song by name\n");
    printf("3. Delete a song by position\n");
    printf("4. Display playlist\n");
    printf("5. Count number of songs\n");
    printf("6. Play song by name\n");
    printf("7. Play song by position\n");
    printf("8. Play next song\n");
    printf("9. Play previous song\n");
    printf("10. Move\n");
    printf("11. Shuffle\n");
    printf("12. Exit\n\n");
    printf("\033[0m"); 
}

NODE *createNode(char *s) 
{
    NODE *newNode =(NODE *)malloc(sizeof(NODE));
    if(newNode==NULL)
    {
        printf("Failed to create song\n");
    }
    else
    {
        strcpy(newNode->song, s);
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->id = 0;
        return newNode;
    }
}

void insertNewSong(LINK *p, char *s)
{
    NODE *newNode = createNode(s);
    if(p->head == NULL)
    {
        p->head = newNode;
        newNode->id = 1;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        NODE *t = p->head;
        NODE *last = t->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = t;
        t->prev = newNode;
        newNode->id = last->id + 1;
    }
}

int count_songs(LINK *p)
{
    int count = 0;
    if(p->head!= NULL)
    {
        NODE *t = p->head;
        do
        {
            count++;
            t = t->next;
        } while(t!=p->head);
    }
    return count;
}

void update(LINK *p)
{

    NODE *t = p->head;
    int n=count_songs(p);
    do
    {
        for(int i=1;i<=n;i++)
        {
            t->id = i;
            t=t->next;
        }
    }while(t!= p->head);
}

void deletebyName(LINK *p, char *s) 
{
    if(p->head == NULL) 
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        NODE *t = p->head;
        do
        {
            if(strcmp(t->song,s) == 0)
            {
                if(t==p->head && t->next==p->head)
                {
                    printf("Deleted song %s\n", t->song);
                    free(t);
                    p->head = NULL;
                }
                else if(t == p->head)
                {
                    p->head = t->next;
                    t->prev->next = t->next;
                    t->next->prev = t->prev;
                    printf("Deleted song %s\n",t->song);
                    free(t);
                }
                else
                {
                    t->prev->next = t->next;
                    t->next->prev = t->prev;
                    printf("Deleted song %s\n",t->song);
                    free(t);
                }
                return;
            }
            t = t->next;
        } while(t!=p->head);

        printf("Song not found in the playlist ");
    }
}

void deleteById(LINK *p, int id)
{
    if(p->head == NULL)
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        NODE *t = p->head;
        do
        {
            if(t->id == id)
            {
                if(t == p->head && t->next == p->head)
                {
                    printf("Deleted song %s\n", t->song);
                    free(t);
                    p->head = NULL;
                }
                else if(t == p->head)
                {
                    p->head = t->next;
                    t->prev->next = t->next;
                    t->next->prev = t->prev;
                    printf("Deleted song %s\n", t->song);
                    free(t);
                }
                else
                {
                    t->prev->next = t->next;
                    t->next->prev = t->prev;
                    printf("Deleted song %s\n", t->song);
                    free(t);
                }
                update(p);
                return;
            }
            t = t->next;
        } while(t!= p->head);

        printf("Song not found in the playlist ");
    }
}

void display(LINK *p)
{
    if(p->head == NULL)
    {
        printf("Playlist Empty ");
    }
    else
    {
        NODE *t = p->head;
        do
        {
            printf("%d. %s\n", t->id, t->song);
            t = t->next;
        }while(t != p->head);
    }
}


void play(NODE *p)
{
    if(p!=NULL)
    {
        printf("Now playing %s \n ", p->song);
    }
    else
    {
        printf("No song to play \n ");
    }
}

void playbyName(LINK *p, char *s)
{
    if(p->head == NULL)
    {
        printf("Playlist Empty ");
    }
    else
    {
        NODE *t = p->head;
        do
        {
            if(strcmp(t->song,s) == 0)
            {
                p->cur = t;
                play(t);
                return;
            }
            t = t->next;
        }while(t!=p->head);
        printf("Song not found in the playlist ");
    }
}

void playById(LINK* p, int id) {
    if(p->head == NULL) {
        printf("Playlist Empty ");
    } else {
        NODE* t1 = p->head;
        do {
            if(t1->id == id) {
                p->cur = t1;
                play(t1);
                return;
            }
            t1 = t1->next;
        }while(t1!=p->head);

        printf("Song not found in the playlist ");
    }
}


void playnext(LINK *p)
{
    if(p->head!=NULL)
    {
        if(p->cur!=NULL)
        {
            p->cur = p->cur->next;
            play(p->cur);
        }
        else
        {
            printf("No song to play next \n");
        }
    }
    else
    {
        printf("Playlist Empty\n");
    }
}

void playprev(LINK *p)
{
    if(p->head!=NULL)
    {
        if(p->cur != NULL)
        {
            p->cur = p->cur->prev;
            play(p->cur);
        }
        else
        {
            printf("No song to play previous \n ");
        }
    }
    else
    {
        printf("Playlist Empty");
    }
    
}

void high_to_low_move(LINK *p, int tomove, int pos)
{
    if(p->head==NULL)
    {
        printf("Empty Playlist\n");

    }
    else
    {
        NODE *thead, *phead, *pprev, *pnext, *tprev, *tnext;
        thead = p->head;
        phead = p->head;

        do
        {
            thead = thead->next;

        }while(thead->id!=pos);

        do
        {
            phead = phead->next;
        }while(phead->id!=tomove);

        if(count_songs(p)==tomove && pos==1)
        {
            p->head = phead;
        }
        else
        {
            pnext = phead->next;
            pprev = phead->prev;

            tnext = thead->next;
            tprev = thead->prev;

            pprev->next = pnext;
            pnext->prev = pprev;

            tprev->next = phead;
            phead->prev = tprev;
            phead->next = thead;
            thead->prev = phead;
        }
    }
}

void low_to_high_move(LINK *p, int tomove, int pos)
{   
    if(p->head==NULL)
    {
        printf("Empty Playlist\n");

    }
    else{

        NODE *thead, *phead, *pprev, *pnext, *tprev, *tnext;
        thead = p->head;
        phead = p->head;

        do
        {
            thead = thead->next;

        }while(thead->id!=pos);

        phead = p->head;
        do
        {
            phead = phead->next;

        }while(phead->id!=tomove);

        if(count_songs(p) == pos && tomove == 1)
        {
            p->head = phead->next;
        }
        else
        {
            pnext = phead->next;
            pprev = phead->prev;

            tnext = thead->next;
            tprev = thead->prev;

            pprev->next = pnext;
            pnext->prev = pprev;

            thead->next = phead;
            phead->prev = thead;
            phead->next = tnext;
            tnext->prev = phead;
        }
    }
}

void Shuffle(LINK *p, int maxRange) 
{
    if(p->head==NULL){
        printf("Empty Playlist");
    }
    else
    {
        srand(time(0));

        for(int i=1;i<=15;i++)
        {
            int num1 = rand() % maxRange + 1; 
            int num2 = rand() % maxRange + 1;

            while(num1 == num2)
            {
                num2 = rand() % maxRange + 1;
            }

            if(num1 < num2)
            {
                low_to_high_move(p, num1, num2);
                update(p);
            }
        }
        printf("Shuffled your playlist\n");
    }
}

void readSongInfo(LINK *p)
{
    FILE *file = fopen("playlist.txt", "w");
    if(file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    NODE *t = p->head;
    do
    {
        fprintf(file,"%s\n",t->song);
        t = t->next;
    }while(t!=p->head);

    fclose(file);
}

void destroy(LINK *p)
{
    NODE *t = p->head;
    NODE *temp = NULL;
    do
    {
        temp = t;
        t = t->next;
        free(temp);
    }while(t!=p->head);
    
    p->head = NULL;
}