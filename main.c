#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 93

int rand_num();
void encrypt(char *pass, int key);
void decrypt(char *pass, int key);

int main(int argc, char *argv[])
{
    char *options = "sv";

    // Get flag and check validity
    char option = getopt(argc, argv, options);
    if (option == '?')
    {
        printf("Invalid option.\n");
        return 1;
    }

    // Ensure only one option
    if (getopt(argc, argv, options) != -1)
    {
        printf("Only one option allowed.\n");
        return 2;
    }

    // check for number of command-line arguments
    if (argc != 3)
    {
        printf("Usage: ./main [flag] file\n   -s   --  Save your password\n   -v   --  View saved passwords\n");
        return 1;
    }

    char name[50], pass[50];
    int key;
    FILE *file;
    // check flag and declare file open type
    // open file and check validity of file
    switch (option)
    {
    case 's':
        file = fopen(argv[2], "a");

        if (!file)
        {
            printf("Can't open file\n");
            return 1;
        }

        // get name and pass from the user
        printf("Name: ");
        scanf("%s", name);

        printf("Password: ");
        scanf("%s", pass);

        key = rand_num();

        encrypt(pass, key); 
        fprintf(file, "%s,%s,%i\n", name, pass, key);

        printf("The information was successfully saved!");
        break;

    case 'v':
        file = fopen(argv[2], "r");

        if (!file)
        {
            printf("Can't open file\n");
            return 1;
        }
        printf("\n\n______________________________________________\n\n\n");
        while ((fscanf(file, "%[^,],%[^,],%i\n", name, pass, &key)) != EOF)
        {
            decrypt(pass, key);   
            printf("\tName: %s\tPassword: %s\n\n", name, pass);
        }
        printf("\n\n______________________________________________\n\n\n");
    }

    fclose(file);

    return 0;
}


int rand_num()
{
    srand(time(0));
    int num = (rand() % ((int)MAX)) + 1;
    return num;
}

void encrypt(char *pass, int key)
{
    int asc;

    for (int i = 0; i < strlen(pass); i++) 
    {
        asc = toascii(pass[i]);
        if (asc >= 33 && asc <= 126) 
        {
            asc += key;

            // if ascii value goes over the printable limit
            if (asc >= 127)
            {
                asc -= 94;
            }
        }
        pass[i] = asc;
    }
}

void decrypt(char *pass, int key)
{
    for (int i = 0; i < strlen(pass); i++)
    {
        if (pass[i] >= 33 && pass[i] <= 126)
        {
            pass[i] -= key;

            // if ascii value goes over the printable limit
            if (pass[i] <= 32)
            {
                pass[i] += 94;
            }
        }
    }
}
