/*Tyler Cooper
1/31/18
creating a shell program*/

#include <stdio.h>
#include <stdlib.h>       //malloc
//#include <sys/types.h>    //for fork //appeareanty not used
#include <unistd.h>       //for fork and execvp
#include <sys/wait.h>     //for waitpid
#include <string.h>       //string libraries
#define BUFFER_LEN 256
#define MAX_NUM_ARGS 256
#define CHILD 0

int main(void)
{
    int pid = 0;
    int childvalue = 0;
    char **input_args = NULL; //point to an array of character pointers
    int i = 0;
    int p = 0;
    int b = 0;  //'bool' to check if too many chars entered
    int j = 0;
    char buffer[BUFFER_LEN] = "";
    char line[BUFFER_LEN] = "";

    while(1)
    {
        printf("tritonshell>\n");
        p = 0;
        j = 0;
        b = 0;
        //free memory from input_args
        if(input_args != NULL)
        {
            for(i=0; i< MAX_NUM_ARGS; i++)
            {
                free(input_args[i]);
            }
            free(input_args);
        }
        input_args = NULL;

        //allocate memory for input_args[]
        input_args = (char **) malloc(sizeof(char *) * MAX_NUM_ARGS);

        //check for errors from malloc
        if(input_args == NULL)
        {
            exit(0);
            printf("Out of memory error\n");
        }

        //get memory for strings, make this look like notes
        for(i=0; i<MAX_NUM_ARGS; i++)
        {
            input_args[i] = NULL; //clear out garbage
            input_args[i] = (char*) malloc(sizeof(char) * BUFFER_LEN);
            if(input_args == NULL)
            {
                printf("error allocating memory\n");
            }
            strcpy(input_args[i], ""); //initialize
        }

        while(fgets(buffer, sizeof(buffer), stdin))
        {

            //get user input
            if(line == NULL)
            {
                printf("Error, nothing was entered\n");
                return 1; //exit with error
            }
            i++;
            //parse the input
            for(i = 0; i< strlen(buffer); i++)
            {
                if (i == 254 && buffer[i] != '\n')  //we have filled our buffer with no end of line
                {
                    printf("Too many characters entered.\n");
                    while (getc(stdin) != '\n');    //will loop clearing stdin
                    b = 1;
                    break;
                }
                if(buffer[i] == ' ' || buffer[i] == '\t')
                {
                    if (j != 0)
                    {
                        p += 1;
                        j = 0;
                    }
                }
                else if(buffer[i] == '\n')
                {
                    if(buffer[i-1] == ' ' || buffer[i-1] == '\t')
                    {
                        free(input_args[p]);
                        input_args[p] = NULL;
                        break;
                    }
                    p += 1;
                    free(input_args[p]);
                    input_args[p] = NULL;
                    break;
                }
                else
                {
                    input_args[p][j] = buffer[i];
                    input_args[p][j+1] = '\0';
                    j++;
                }
            }
            break;
        } //close while(fgets....) loop
        //create child
        if (b != 1) {
            if (strcmp(input_args[0], "exit") == 0) {
                exit(0);
            }
            pid = fork();
            //if parent
            if (pid != CHILD) {
                //waiting on child
                pid = waitpid(-1, &childvalue, 0);
            } else {
                //this is child
                //execute the command given by the user
                execvp(input_args[0], input_args);
                if (strcmp(input_args[0], "") != 0) {
                    printf("error: %s is not a valid command.\n", input_args[0]);
                }
                exit(0);


            }
        }
    } //close while(1) loop
} //end of program
