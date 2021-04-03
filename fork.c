#include <fcntl.h>    
#include <sys/types.h> 
#include <sys/stat.h>  
#include <stdio.h>     
#include <unistd.h>    
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/*This code divides the lines taken from the file according to the space character and opens the desired file at the desired location
 and it also works when there is more than one line in the file.
Beyza Sude Erol - 19120205009*/

int main()
{
    char *line, *word;
    int len = 0, len2 = 0;
    char **arr1 = malloc((1) * sizeof(char *)); //to initialize the array , one char size place is taken from memory (otherwise i was getting an error.)
    pid_t pid;

    int file = open("programs.txt", O_RDONLY); //file opens
    if (file == -1)
    {
        perror("cannot open file: "); //if the file does not open it gives an error.
        return -1;
    }
    ssize_t ret;
    char buf[512] = {0};
    ret = read(file, buf, sizeof(buf)); //read from file
    if (ret == -1)
    {
        perror("cannot read");
        close(file);
        return -1;
    }

    line = strtok(buf, "\n"); //read from file line by line

    while (line != NULL) //loop continues until line is null
    {
        pid = fork(); //create process

        if (pid == 0) // child process
        {
            printf("pid: %d, command: %s\n", getpid(), line); //print process id

            int c = 0;

            word = strtok(line, " "); //words in a line are separated by spaces

            while (word != NULL) //loop continues until word is null
            {
                len2 = strlen(word);                         // the length of the word is found
                arr1[c] = malloc((len2 + 1) * sizeof(char)); //the lenght of the word is allocated to arr1 by malloc.
                arr1[c] = word;                              //word assigned to arr1

                word = strtok(NULL, " "); //the other word on the same line is taken.
                len2 = 0;                 // len2 is reset for new word.
                c++;                      //the new word will assigned to the other element of the arr1.
            }

            arr1[c] = NULL;

            int result = execvp(arr1[0], arr1); //the words taken from the line are used directly for use in execvp

            if (result == -1)
                perror("execvp: ");

            exit(0); //for safety
        }
        else 
        {

            wait(NULL);                //wait for child process 
            line = strtok(NULL, "\n"); // move to next line.
        }
    }

    close(file); //if there are no more lines to read ,the file is closed.
    return 0;
}
