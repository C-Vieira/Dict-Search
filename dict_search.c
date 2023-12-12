#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA_SIZE 26
#define BUFFER_SIZE 256

struct Node
{
	struct Node *child[ALPHA_SIZE];
	bool wordTerminator;
};

bool isLetter(char c){
    return (((int)c > 64) && ((int)c < 91)) || (((int)c > 96) && ((int)c < 123));
}

char toUpper(char c){
    if(((int)c > 64) && ((int)c < 91)) return c;
    return (int)c - 32;
}

struct Node *newNode(void)
{
	struct Node *No = NULL;
	No = (struct Node *)malloc(sizeof(struct Node));
	if (No)
	{
		int i;
		No->wordTerminator = false;
		for (i = 0; i < ALPHA_SIZE; i++)
			No->child[i] = NULL;
	}
	return No;
}

void insert(struct Node *root,  char *key)
{
	int level;
	int size = strlen(key);
	int i;
	struct Node *p = root;
	for (level = 0; level < size; level++)
	{
	    if(!isLetter(key[level])) return -1; //Invalid char, ignore
		i = ((int)key[level] - (int)'A');
		if (!p->child[i])
			p->child[i] = newNode();

		p = p->child[i];
	}
	p->wordTerminator = true;
}

void clear(struct Node *root){
    struct Node *p = root;
    if(!p){
        free(p);
        return;
    }
    for(int i = 0; i < ALPHA_SIZE; i++){
        //printf("   \nFREEING: %c\n\n", p->child[i]);
        clear(p->child[i]);
    }
}

bool search(struct Node *root,  char *key)
{
	int level;
	int size = strlen(key);
	int i;
	struct Node *p = root;
	for (level = 0; level < size; level++)
	{
		i = ((int)key[level] - (int)'A');
		if (!p->child[i])
			return false;
		p = p->child[i];
	}
	return (p != NULL && p->wordTerminator);
}

int main()
{
    char msg[][32] = {"IS NOT IN THE DICTIONARY", "IS IN THE DICTIONARY"};
	struct Node *root = newNode();

    FILE *input = fopen("words_alpha.txt", "r");
    if(ferror(input) || input == NULL){
        printf("\n\n   ERROR OPENING FILE...\n\n");
        system("pause");
        return -1;
    }

    //Load Dictionary
    printf("\n\n   L O A D I N G   D I C T I O N A R Y . . .\n");
    printf("\n\n   T H I S   C A N   T A K E   A   B I T . . .\n\n");

    char buffer[BUFFER_SIZE];

    while((fscanf(input, "%s", buffer)) != EOF){ //For each string (line)

        for(int i = 0; i < strlen(buffer); i++) //For each char in string
            if(isLetter(buffer[i])) buffer[i] = toUpper(buffer[i]);

        insert(root, buffer); //Insert word
    }
    system("cls");
    fclose(input);

	int op;
	char word[BUFFER_SIZE];
	do{
        printf("\n\n   - - - S E A R C H   T H E - - -   ");
        printf("\n   - - - D I C T I O N A R Y - - -   \n\n");
        printf("   1- SEARCH WORD\n\n   0- EXIT\n\n   >");
        scanf("%d",&op);
         switch(op){
            case 1:
                printf("\n   TYPE SOME WORD: ");

                //Read string
                fflush(stdin);
                scanf("%s", word);

                //Search tree
                for(int i = 0; i < strlen(word); i++) //For each char
                    if(isLetter(word[i])) word[i] = toUpper(word[i]);

                printf("\n\n   %s ===> %s\n", word, msg[search(root, word)]);

                printf("\n\n");
                system("pause");
                system("cls");
                break;
            case 0:
                printf("\n   QUITTING...\n\n");

                break;
            default:
                printf("\n   INVALID OPTION...\n\n");
                system("pause");
                system("cls");
                break;
         }
	}while(op!=0);

    printf("   FREEING UP MEMORY...\n\n");
    clear(root);
    printf("   ALL DONE!\n");

	return 0;
}
