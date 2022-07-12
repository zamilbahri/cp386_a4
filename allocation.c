// Authors: zamilbahri, Maskoochi
// GitHub Repository: https://github.com/zamilbahri/cp386_a4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char id[2];
	int size;
	int start_address;
} process_t;

typedef struct {
	int size;
	int num_partitions;
	process_t *processes;
} memory_t;

void readCommand(char *buffer);
char **cmdSplit(char *cmd, const char delimiter);
void displayStatus(memory_t *memory);
void cmdArrFree(char **command);

int main(int argc, char **argv) {

	memory_t memory;

	if (argc == 2)
	{
		memory.size = atoi(argv[1]);
		printf("Allocating %d bytes of memory\n", memory.size);
	}
	else {
		printf("Expected memory size as argumnent. Exiting program. \n");
		return 1;
	}

	while (1) {

		char buffer[50];
		readCommand(buffer);
		
		char **command = cmdSplit(buffer, ' ');

    printf("command: %s\n", command[0]);
		
		if (strcmp(command[0], "Exit") == 0) {
			printf("Exiting program.\n");
			for (int i = 0; command[i]; i++) {
				free(command[i]);
			}
			break;
		}

		else if (strcmp(command[0], "Status") == 0) {
			displayStatus(&memory);
		}

		else if (strcmp(command[0], "RQ") == 0) {
			// Allocate a process with the given id, size, and method
			printf("Allocating process %s with size %s using method %s\n", command[1], command[2], command[3]);
		}

		else if (strcmp(command[0], "RL") == 0) {
			// Release process
			printf("Releasing process %s\n", command[1]);
		}

		else if (strcmp(command[0], "C") == 0) {
			// Compact memory
			printf("Compacting memory\n");

		}

		else {
			printf("Invalid command.\n");
		}
		//free commands
		cmdArrFree(command);
	}

	return 0;
}

void displayStatus(memory_t *memory) {
	printf("Displaying status\n");
	
	// complete this function
}

void allocateMemory(memory_t *memory, process_t *process, char *method) {
	printf("Allocating memory\n");

	// complete this function
}

void releaseMemory(memory_t *memory, process_t *process) {
	printf("Releasing memory\n");

	// complete this function
}

void compactMemory(memory_t *memory) {
	printf("Compacting memory\n");

	// complete this function
}

void readCommand(char *buffer) {
	printf("allocator>");
	int numCommands = 0;
	fgets(buffer, 50, stdin);
	buffer[strcspn(buffer, "\r\n")] = 0;
}


// splits a string based on a delimiter. Caller must free the returned array.
char **cmdSplit(char *cmd, const char delimiter)
{
    char **result = NULL;
    int count = 0;
    char *tmp = cmd;
    char *last_delim = 0;
    char delim[] = { delimiter, 0 };

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count++;
            last_delim = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_delim < (cmd + strlen(cmd) - 1);

    /* Add space for terminating null string */
    count++;

		// allocate memory for array of strings
    result = malloc((size_t) count * sizeof(char *));

    if (result)
    {
        int idx = 0;
        char *token = strtok(cmd, delim);

        while (token)
        {
            result[idx++] = strdup(token);
            token = strtok(0, delim);
        }
        result[idx] = 0;
    }

    return result;
}

// frees an array of strings
void cmdArrFree(char **command) 
{
	for (int i = 0; command[i]; i++) 
	{
		free(command[i]);
	}
	free(command);
}