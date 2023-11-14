#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

typedef struct node {
    struct node* next;
    struct node* prev;
    char song[1000];
    int id;
} NODE;

typedef struct link {
    NODE* head;
	NODE* cur;
}LINK;

NODE* createNode(char* s) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    strcpy(newNode->song, s);
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->id = 0;
    return newNode;
}

void insertNode(LINK* p, char* s) {
    NODE* newNode = createNode(s);
    if (p->head == NULL) {
        p->head = newNode;
        newNode->id = 1;
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
        newNode->id = last->id + 1;
    }
}

void deletebyName(LINK* p, char* s) 
{
    if (p->head == NULL) {
        printf("Playlist Empty ");
    } else {
        NODE* t1 = p->head;
        do {
            if (strcmp(t1->song, s) == 0) {
                if (t1 == p->head && t1->next == p->head) {
                    printf("Deleted song %s \n", t1->song);
                    free(t1);
                    p->head = NULL;
                } else if (t1 == p->head) {
                    p->head = t1->next;
                    t1->prev->next = t1->next;
                    t1->next->prev = t1->prev;
                    printf("Deleted song %s \n", t1->song);
                    free(t1);
                } else {
                    t1->prev->next = t1->next;
                    t1->next->prev = t1->prev;
                    printf("Deleted song %s \n", t1->song);
                    free(t1);
                }
                return;
            }
            t1 = t1->next;
        } while (t1 != p->head);

        printf("Song not found in the playlist ");
    }
}

void display(LINK* p) 
{
    if (p->head == NULL) {
        printf("Playlist Empty ");
    }
    else {
        NODE* t = p->head;
        do {
            printf("%d. %s\n", t->id, t->song);
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
        printf("Now playing %s \n ", p->song);
    } else {
        printf("No song to play \n ");
    }
}

void playbyName(LINK* p, char* s) {
    if (p->head == NULL) {
        printf("Playlist Empty ");
    } else{
        NODE* t1 = p->head;
        do {
            if (strcmp(t1->song, s) == 0) {
                p->cur=t1;
				play(t1);
                return;
            }
            t1 = t1->next;
        } while (t1 != p->head);
        printf("Song not found in the playlist ");
    }
}

void playnext(LINK* p) {
    if (p->cur != NULL) {
        p->cur = p->cur->next;
        play(p->cur);
    } else {
        printf("No song to play next \n ");
    } 
}

void playprev(LINK* p) {
    if (p->cur != NULL) {
        p->cur = p->cur->prev;
        play(p->cur);
    } else {
        printf("No song to play previous \n ");
    }
}

void high_to_low_move(LINK*p,int tomove,int pos)
{
    NODE* thead,*phead,*pprev,*pnext,*tprev,*tnext;
    thead=p->head;
    phead=p->head;

    do{
        thead=thead->next;

    }while(thead->id!=pos);
    

    phead=p->head;
    do{
        phead=phead->next;

    }while(phead->id!=tomove);
	
	if(count_songs(p)==tomove&&pos==1){
		p->head=phead;
	}else{
    	pnext=phead->next;
    	pprev=phead->prev;
    	tnext=thead->next;
    	tprev=thead->prev;

    	pprev->next=pnext;
    	pnext->prev=pprev;

    	thead->next=phead;
    	phead->prev=thead;

    	phead->next=tnext;
    	tnext->prev= phead;
	}
    
}

void low_to_high_move(LINK*p,int tomove,int pos)
{
    NODE* thead,*phead,*pprev,*pnext,*tprev,*tnext;
    thead=p->head;
    phead=p->head;

    do{
        thead=thead->next;

    }while(thead->id!=pos);

    phead=p->head;
    do{
        phead=phead->next;

    }while(phead->id!=tomove);
	if(count_songs(p)==pos&&tomove==1){
		p->head=phead->next;
	}
    else{
    	pnext=phead->next;
    	pprev=phead->prev;
    	tnext=thead->next;
    	tprev=thead->prev;

    	pprev->next=pnext;
    	pnext->prev=pprev;

    	thead->next=phead;
    	phead->prev=thead;

    	phead->next=tnext;
    	tnext->prev= phead;
	}
}

void update(LINK*p){
    
    NODE* t = p->head;
    int n=count_songs(p);
    do {
        for(int i=1;i<=n;i++){
            t->id=i;
			t=t->next;
        }
    }while (t != p->head);

}

void Shuffle(LINK *p) 
{
    srand(time(NULL));

    int maxRange= count_songs(p);

    for (int i = 1; i <= count_songs(p); i++) {
        int num1 = rand() % maxRange + 1;
        int num2 = rand() % maxRange+ 1;

        
        printf("Pair %d: %d, %d\n", i, num1, num2);
        if(num1==num2){
            continue;
        }
        else if(num1<num2)
        {
            low_to_high_move(p,num1,num2);
        }
        else{
            high_to_low_move(p,num1,num2);        
        }
    }
}

int main()
{
    printf("WELCOME TO YOUR PERSONAL MUSIC PLAYER \n");
    LINK obj;
    obj.head= NULL;
    NODE* hold=obj.head;

    int choice;
    int tomove, pos;
    char s[1000];

    char s1[1000];
    char s2[1000];
    do
    {
        printf("\n1.Add New Song\n2.Delete Song\n3.Display Playlist\n4.Count number of songs\n5.Play Song\n6.Play next song\n7.Play previous song\n8.Move\n9.Shuffle\n10.Exit\n\n");
        scanf("%d",&choice);
        switch(choice)

        {
            case 1:
                
                printf("Enter the Song name ");
                scanf("%s", s);
                insertNode(&obj, s);
            break;

            case 2:
                
                printf("Enter song to be deleted ");
                scanf("%s",s1);
                deletebyName(&obj,s1);
                update(&obj);
            break;

            case 3:
                display(&obj);
            break;  

            case 4:
                printf("Total number of songs are %d \n",count_songs(&obj));
            break;

            case 5:
                
                printf("Enter song name ");
                scanf("%s",s2);
                playbyName(&obj,s2);
            break;

            case 6:
                playnext(&obj);
            break;

            case 7:
                playprev(&obj);
            break;

            case 8:
                printf("Enter position of song you want to move ");
                scanf("%d", &tomove);
                printf("Enter position ");
                scanf("%d", &pos);

                if(count_songs(&obj)==1){
                    printf("Add more songs to implement move ");
                }
                
                if(tomove>pos)
                {
                    high_to_low_move(&obj, tomove,pos);
                    update(&obj);
                    printf("Moved song from %d position to %d position \n ",tomove,pos);
					
                }
                else if(tomove<pos)
                {
                    low_to_high_move(&obj, tomove,pos);
                    update(&obj);
                    printf("Moved song from %d position to %d position \n ",tomove,pos);

                }
                else if(tomove==pos){
                    printf("Song is already at position %d ",pos);
                }
                else{
                    printf("Invalid input");
                }
            break;
            case 9:
                Shuffle(&obj);
                update(&obj);

            break;
            case 10:
                generate_random(&obj);
            break;

            case 11:
            exit(0);

        }
    } while (choice!=11);
}

