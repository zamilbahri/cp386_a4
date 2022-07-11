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

void readCommand(char *command);
void displayStatus(memory_t *memory);

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
		char command[100];
		readCommand(command);

		if (strcmp(command, "exit") == 0) {
			printf("Exiting program.\n");
			break;
		}

		else if (strcmp(command, "Status") == 0) {
			displayStatus(&memory);
		}

		else {
			printf("Invalid command.\n");
		}

	}
	return 0;
}

void readCommand(char *command) {
	printf("allocator> ");
	fgets(command, 100, stdin);
	command[strcspn(command, "\r\n")] = 0;
}

void displayStatus(memory_t *memory) {
	printf("Displaying status\n");
}