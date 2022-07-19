// Authors: zamilbahri, Maskoochi
// GitHub Repository: https://github.com/zamilbahri/cp386_a4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to define a process
typedef struct process_t
{
	char id[3];
	int size;
} process_t;

// Structure to define a partition that is used to store a process.
typedef struct partition_t
{
	int size;
	int base;
	int available;
	process_t process;
	struct partition_t *prev;
	struct partition_t *next;
} partition_t;

// Structre to define the memory block
typedef struct memory_t
{
	int size;
	int num_partitions;
	int total_allocated;
	partition_t *partitions;
} memory_t;

// Needs to be completed
void displayStatus(memory_t *memory);
int allocateMemory(memory_t *memory, char *process, int size, char *method);
void releaseMemory(memory_t *memory, char *process);
void compactMemory(memory_t *memory);

// Utility functions
char **cmdSplit(char *cmd, const char delimiter);
void readCommand(char *buffer);
void cmdArrFree(char **command);

int main(int argc, char **argv)
{

	memory_t memory;

	if (argc == 2)
	{
		// initialize memory_t structure
		memory.size = atoi(argv[1]);
		memory.num_partitions = 1;
		memory.total_allocated = 0;
		memory.partitions = malloc(sizeof(partition_t));

		// initialize paritions_t structure
		memory.partitions->size = memory.size;
		memory.partitions->base = 0;
		memory.partitions->available = 1;
		memory.partitions->prev = NULL;
		memory.partitions->next = NULL;
		printf("Allocating %d bytes of memory\n", memory.size);
	}

	else
	{
		printf("Expected memory size as argumnent. Exiting program. \n");
		return 1;
	}

	while (1)
	{

		char buffer[50];
		readCommand(buffer);

		char **command = cmdSplit(buffer, ' ');

		printf("command: %s\n", command[0]);

		if (strcmp(command[0], "Exit") == 0)
		{
			printf("Exiting program.\n");
			cmdArrFree(command);
			free(memory.partitions);
			break;
		}

		else if (strcmp(command[0], "Status") == 0)
		{
			displayStatus(&memory);
		}

		else if (strcmp(command[0], "RQ") == 0)
		{
			// Allocate a process with the given id, size, and method
			// printf("Allocating process %s with size %s using method %s\n", command[1], command[2], command[3]);
			allocateMemory(&memory, command[1], atoi(command[2]), command[3]);
		}

		else if (strcmp(command[0], "RL") == 0)
		{
			// Release process
			// printf("Releasing process %s\n", command[1]);
			releaseMemory(&memory, command[1]);
		}

		else if (strcmp(command[0], "C") == 0)
		{
			// Compact memory
			// printf("Compacting memory\n");
			compactMemory(&memory);
		}

		else
		{
			printf("Invalid command.\n");
		}
		// free commands
		cmdArrFree(command);
	}

	return 0;
}

void displayStatus(memory_t *memory)
{
	printf("Displaying status\n");
	// printf("Address [%d:%d] Process %s\n", memory->processes[i].base, memory->processes[i].limit, memory->processes[i].id);
	printf("Partitions [Allocated memory = %d]:\n", memory->total_allocated);
	partition_t *p = memory->partitions;
	while (p)
	{
		if (!(p->available))
			printf("Address [%d:%d] Process %s\n", p->base, p->base + p->size - 1, p->process.id);
		p = p->next;
	}

	p = memory->partitions;
	printf("Holes [Free memory = %d]:\n", memory->size - memory->total_allocated);
	while (p)
	{
		if (p->available)
			printf("Address [%d:%d] len = %d\n", p->base, p->base + p->size - 1, p->size);
		p = p->next;
	}
	// complete this function
}

// returns 0 if successful, 1 if not
int allocateMemory(memory_t *memory, char *process, int size, char *method)
{

	if (size > memory->size - memory->total_allocated)
	{
		printf("Not enough memory to allocate process %s with size %d\n", process, size);
		return 1;
	}

	// find the first available partition
	if (strcmp(method, "F") == 0)
	{
		partition_t *p = memory->partitions;

		while (p)
		{
			if (p->available)
			{
				if (size <= p->size)
				{
					// update current partition
					p->available = 0;
					strcpy(p->process.id, process);
					p->process.size = size;
					memory->total_allocated += size;

					// if there is a hole after this partition, create a new partition
					if (size < p->size)
					{
						memory->num_partitions++;
						partition_t *new_p = (partition_t *)malloc(sizeof(partition_t));
						new_p->size = p->size - size;
						new_p->base = p->base + size;
						p->size = size;
						new_p->available = 1;
						new_p->prev = p;
						new_p->next = p->next;
						p->next = new_p;
					}

					printf("Sucessfully allocated %d to process %s\n", size, process);

					return 0;
				}
				else
				{
					printf("No hole of sufficient size\n");
				}
			}
			p = p->next;
		}
	}

	// find the smallest available partition (best fit)
	else if (strcmp(method, "B") == 0)
	{
		partition_t *smallestP;
		partition_t *p = memory->partitions;
		// initializing the temp to free space available
		int temp = memory->size - memory->total_allocated + 1;
		while (p)
		{
			if (p->available)
			{
				if (p->size >= size)
				{
					if (temp > p->size)
					{
						// storing the details of the smallest partition in smallestP
						smallestP = (partition_t *)malloc(sizeof(partition_t));
						smallestP->base = p->base;
						smallestP->available = 1;
						smallestP->size = p->size;
						smallestP->prev = p->prev;
						smallestP->next = p->next;

						// storing the smallest partition size so far in varable temp
						temp = p->size;
					}
				}
			}
			p = p->next;
		}

		if (size <= smallestP->size)
		{
			// checking if the prev was head of the list and needs to be updated
			if (smallestP->prev != NULL)
			{
				smallestP->prev->next = smallestP;
			}
			else
			{
				memory->partitions = smallestP;
			}
			// update current partition
			smallestP->available = 0;
			strcpy(smallestP->process.id, process);
			smallestP->process.size = size;
			memory->total_allocated += size;

			// if there is a hole after this partition, create a new partition
			if (size < smallestP->size)
			{
				memory->num_partitions++;
				partition_t *new_p = (partition_t *)malloc(sizeof(partition_t));
				new_p->size = smallestP->size - size;
				new_p->base = smallestP->base + size;
				smallestP->size = size;
				new_p->available = 1;
				new_p->prev = smallestP;
				new_p->next = smallestP->next;
				smallestP->next = new_p;
			}
			printf("Sucessfully allocated %d to process %s\n", size, process);
		}
		else
		{
			printf("No hole of sufficient size\n");
		}
	}
	// find the largest available partition (worst fit)
	else if (strcmp(method, "W") == 0)
	{

		partition_t *worseP;
		partition_t *p = memory->partitions;
		// initializing the temp to free space available
		int temp = memory->size - memory->total_allocated - 1;
		while (p)
		{
			if (p->available)
			{
				if (p->size >= size)
				{
					if (temp < p->size)
					{
						// storing the details of the largest partition in worseP
						worseP = (partition_t *)malloc(sizeof(partition_t));
						worseP->base = p->base;
						worseP->available = 1;
						worseP->size = p->size;
						worseP->prev = p->prev;
						worseP->next = p->next;
						temp = p->size;
					}
				}
			}
			p = p->next;
		}
		p = memory->partitions;

		if (size <= worseP->size)
		{
			// checking if the prev was head of the list and needs to be updated
			if (worseP->prev != NULL)
			{
				worseP->prev->next = worseP;
			}
			else
			{
				memory->partitions = worseP;
			}

			// update current partition
			worseP->available = 0;
			strcpy(worseP->process.id, process);
			worseP->process.size = size;
			memory->total_allocated += size;

			// if there is a hole after this partition, create a new partition
			if (size < worseP->size)
			{
				memory->num_partitions++;
				partition_t *new_p = (partition_t *)malloc(sizeof(partition_t));
				new_p->size = worseP->size - size;
				new_p->base = worseP->base + size;
				worseP->size = size;
				new_p->available = 1;
				new_p->prev = worseP;
				new_p->next = worseP->next;
				worseP->next = new_p;
			}
			printf("Sucessfully allocated %d to process %s\n", size, process);
		}
		else
		{
			printf("No hole of sufficient size\n");
		}
	}

	else
	{
		printf("Invalid method.\n");
		return 1;
	}

	return 0;
}

void releaseMemory(memory_t *memory, char *process)
{

	partition_t *p = memory->partitions;
	while (p)
	{
		if (!(p->available))
		{
			if (strcmp(p->process.id, process) == 0)
			{
				p->available = 1;
				int size = p->process.size;
				memory->total_allocated -= p->process.size;

				// case: next partition is free: merge and free next partition

				if (p->next && p->next->available)
				{
					p->size += p->next->size;
					partition_t *tmp = p->next;
					p->next = p->next->next;
					free(tmp);
					memory->num_partitions--;
				}

				// case: previous partition is free: merge and free current partition
				if (p->prev && p->prev->available)
				{
					p->prev->size += p->size;
					p->prev->next = p->next;
					free(p);
					memory->num_partitions--;
				}
				printf("Sucessfully released %d from process %s\n", size, process);
				return;
			}
		}
		p = p->next;
	}
}

void compactMemory(memory_t *memory)
{
	printf("Compacting memory\n");
	partition_t *p = memory->partitions;
	int i = 0;
	while (p)
	{
		if (p->available)
		{
			if (p->prev == NULL)
			{
				p->next->prev = NULL;
				memory->partitions = p->next;
				i += p->size;
			}
			else if (p->next == NULL)
			{
				p->size += i;
			}
			else
				p->prev->next = p->next;
		}
		p = p->next;
	}
	p = memory->partitions;
	while (p->next != NULL)
	{
		if (p->prev == NULL)
		{
			p->base = 0;
		}
		else
		{
			p->base = p->prev->base + p->prev->size;
		}

		p = p->next;
	}
	printf("Compaction proccess is successful\n");
}
// ========================= Utility Functions =========================

void readCommand(char *buffer)
{
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
	char delim[] = {delimiter, 0};

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
	result = malloc((size_t)count * sizeof(char *));

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