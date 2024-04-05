#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "music_functions.h"

int main()
{
    LINK obj;              
    obj.head = NULL;       
    NODE *hold = obj.head; 
    int choice; 
    int tomove; 
    int pos;    
    int posi;
    char song[1000];         
    char songDelete[1000];   
    char s[1000];
    char s2[1000];

    printf("\033[1;34m");
    printf("______________________________________________________________________________________\n\n\n");
    printf("                        WELCOME TO YOUR PERSONAL MUSIC PLAYER                         \n\n\n");
    printf("______________________________________________________________________________________\n\n\n");
    printf("\033[0m");

    do
    {
        printMenu();
        printf("\033[1;36m"); 
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\033[0m"); 

        switch(choice)
        {
        case 1: 
            printf("Enter the name of the song: ");
            scanf("%s",song);
            insertNewSong(&obj,song);
            readSongInfo(&obj);
            break;

        case 2: 
            printf("Enter the name of the song to be deleted: ");
            scanf("%s",songDelete);
            deletebyName(&obj,songDelete);
            readSongInfo(&obj);
            update(&obj);
            break;

        case 3: 
            
            printf("Enter the position of the song to be deleted: ");
            scanf("%d", &posi);
            deleteById(&obj,posi);
            readSongInfo(&obj);
            update(&obj);
            break;

        case 4: 
            display(&obj);
            break;

        case 5: 
            printf("The total number of songs are: %d\n",count_songs(&obj));
            break;

        case 6: 
            printf("Enter the name of the song: ");
            scanf("%s",s2);
            playbyName(&obj,s2);
            break;

        case 7:
            printf("Enter the position of the song: ");
            scanf("%d",&posi);
            playById(&obj,posi);
            break;

        case 8: 
            playnext(&obj);
            break;

        case 9: 
            playprev(&obj);
            break;

        case 10: 
            printf("Enter the position of the song you want to move: ");
            scanf("%d",&tomove);
            printf("Enter position ");
            scanf("%d",&pos);

            if(tomove<=0||tomove>count_songs(&obj)||pos<=0||pos>count_songs(&obj))
            {
                printf("Invalid Input");
            }
            else
            {
                if(count_songs(&obj)==1)
                {
                    printf("You must add more songs to the playlist before you can move them.");
                }

                if(tomove>pos)
                {
                    high_to_low_move(&obj,tomove,pos);
                    update(&obj);
                    readSongInfo(&obj);
                    printf("Moved song from position %d to position %d\n",tomove,pos);
                }
                else if(tomove<pos)
                {
                    low_to_high_move(&obj,tomove,pos);
                    update(&obj);
                    readSongInfo(&obj);
                    printf("Moved song from position %d to position %d\n",tomove,pos);
                }
                else if(tomove == pos)
                {
                    printf("This song is already at position %d",pos);
                }
            }
            break;

        case 11: 
            Shuffle(&obj, count_songs(&obj));
            update(&obj);
            readSongInfo(&obj);
            break;

        case 12: 
            destroy(&obj);
            exit(0);
            break;

        default:                  
            printf("\033[1;31m"); 
            printf("Invalid input. Please enter a valid option from this menu:\n");
            printMenu();
            printf("\033[0m"); 
            break;
        }

    }while(choice!= 12);

    return 0;
}