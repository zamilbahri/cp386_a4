# CP386 Assignment 4: Memory Allocation

## Features

This program simulates operating system memory management at an abstract level. The user may request allocate a certain amount of memory, and then assign portions of this memory pool to certain processes using one of three algorithmns: First Fit (F), Best Fit (B), and Worst Fit (W). The user may release processes, which releases the memory back to the pool and makes it available. To get around memory fragmentation, the user may also request to perform memory compaction, which would remove any holes in between used memory.

The following is a list of commands:
|   Description  | Command |                 Arguments                |
|:--------------:|:-------:|:----------------------------------------:|
| Request Memory | RQ      | \<Process Name> <Size (Bytes)> \<Method> |
| Release Process| RL      | \<Process Name>                          |
| Display Status | Status  |                                          |
| Compact Memory | C       |                                          |
| Exit Program   | Exit    |                                          |


## Examples and Screenshots

Compile and run the program: `make run`. By defualt, the amount allocated is 1 MB. To change this, edit line 12 in the Makefile with the desired amount of memory.

Request 200KB of memory for process P0 using first fit: `RQ P0 200000 F`

Request 350KB of memory for process P1 using first fit: `RQ P1 350000 F`

Request 300KB of memory for process P2 using first fit: `RQ P2 300000 F`

Release process P0: `RL P0`

Display the current `Status` of the total memory pool:

![Status](/screenshots/status.png)

Compact memory: `C`

![Compact](/screenshots/compact.png)

Example of entire process:

![Full Process](/screenshots/fullprocess.png)_

## Test cases

### 1. Request memory using any method:
```
RQ P0 200000 F
RQ P1 350000 B
RQ P2 300000 W
Status
```
Expected:
```
Partitions [Allocated memory = 850000]:
Address [0:199999] Process P0
Address [200000:549999] Process P1
Address [550000:849999] Process P2

Holes [Free memory = 198576]:
Address [850000:1048575] len = 198576
```

### 2. Release memory (between processes)
```
RQ P0 100000 F
RQ P1 100000 F
RQ P2 100000 F
RL P1
Status
```
Expected:
```
Partitions [Allocated memory = 200000]:
Address [0:99999] Process P0
Address [200000:299999] Process P2

Holes [Free memory = 848576]:
Address [100000:199999] len = 100000
Address [300000:1048575] len = 748576
```

### 3. Release memory preceding free partition
```
RQ P0 100000 F
RQ P1 100000 F
RQ P2 100000 F
RL P1
RL P0
Status
```
Expected:
```
Partitions [Allocated memory = 100000]:
Address [200000:299999] Process P2

Holes [Free memory = 948576]:
Address [0:199999] len = 200000
Address [300000:1048575] len = 748576
```

### 4. Release memory following free partition
```
RQ P0 100000 F
RQ P1 100000 F
RQ P2 100000 F
RL P0
RL P1
Status
```
Expected:
```
Partitions [Allocated memory = 100000]:
Address [200000:299999] Process P2

Holes [Free memory = 948576]:
Address [0:199999] len = 200000
Address [300000:1048575] len = 748576
```


### 5. First Fit (Successful)
```
RQ P0 100000 F
RQ P1 100000 F
RQ P2 100000 F
RL P1
RQ P3 200000 F
RQ P4 100000 F
Status
```
Expected:
```
Partitions [Allocated memory = 500000]:
Address [0:99999] Process P0
Address [100000:199999] Process P4
Address [200000:299999] Process P2
Address [300000:499999] Process P3

Holes [Free memory = 548576]:
Address [500000:1048575] len = 548576
```
### 6. First Fit (Unsuccessful)
```
RQ P0 100000 F
RQ P1 900000 F
RL P0
RQ P2 200000 F
Status
```
Expected:
```
allocator>RQ P2 200000 F
Not enough memory to allocate process P2 with size 200000
```
```
Partitions [Allocated memory = 900000]:
Address [100000:999999] Process P1

Holes [Free memory = 148576]:
Address [0:99999] len = 100000
Address [1000000:1048575] len = 48576
```

### 7. Best Fit (Successful)
```
RQ P0 100000 B
RQ P1 100000 B
RQ P2 75000 B
RQ P3 100000 B
RL P0
RL P2
RQ P5 50000 B
Status
```
Expected:
```
allocator>RQ P5 50000 B
index = 0 delta = 50000 best_delta = 848576
index = 1 delta = 25000 best_delta = 50000
index = 2 delta = 623576 best_delta = 25000
```
```
Partitions [Allocated memory = 250000]:
Address [100000:199999] Process P1
Address [200000:249999] Process P5
Address [275000:374999] Process P3

Holes [Free memory = 798576]:
Address [0:99999] len = 100000
Address [250000:274999] len = 25000
Address [375000:1048575] len = 673576
```
### 8. Best Fit (Unsuccessful)
```
RQ P0 100000 B
RQ P1 900000 B
RL P0
RQ P2 200000 B
Status
```
Expected:
```
allocator>RQ P2 200000 B
Not enough memory to allocate process P2 with size 200000
```
```
Partitions [Allocated memory = 900000]:
Address [100000:999999] Process P1

Holes [Free memory = 148576]:
Address [0:99999] len = 100000
Address [1000000:1048575] len = 48576
```

### 9. Worst Fit (Successful)
```
RQ P0 100000 W
RQ P1 100000 W
RQ P2 75000 W
RQ P3 700000 W
RL P0
RL P2
RQ P5 50000 W
Status
```
Expected:
```
allocator>RQ P5 50000 W
index = 0 delta = 50000 worst_delta = 0
index = 1 delta = 25000 worst_delta = 50000
index = 2 delta = 23576 worst_delta = 50000
```
```
Partitions [Allocated memory = 850000]:
Address [0:49999] Process P5
Address [100000:199999] Process P1
Address [275000:974999] Process P3

Holes [Free memory = 198576]:
Address [50000:99999] len = 50000
Address [200000:274999] len = 75000
Address [975000:1048575] len = 73576
```
### 10. Worst Fit (Unsuccessful)
```
RQ P0 100000 W
RQ P1 900000 W
RL P0
RQ P2 200000 W
Status
```
Expected:
```
allocator>RQ P2 200000 W
Not enough memory to allocate process P2 with size 200000
```
```
Partitions [Allocated memory = 900000]:
Address [100000:999999] Process P1

Holes [Free memory = 148576]:
Address [0:99999] len = 100000
Address [1000000:1048575] len = 48576
```

### 11. Compaction (Start)
```
RQ P0 100000 F
RQ P1 100000 F
RL P0
C
Status
```
Expected:
```
Partitions [Allocated memory = 100000]:
Address [0:99999] Process P1

Holes [Free memory = 948576]:
Address [100000:1048575] len = 948576
```

### 12. Compaction (Middle)
```
RQ P0 100000 B
RQ P1 100000 B
RQ P2 75000 B
RQ P3 100000 B
RL P0
RL P2
RQ P5 50000 B
C
Status
```
Expected:
```
Partitions [Allocated memory = 250000]:
Address [0:99999] Process P1
Address [100000:149999] Process P5
Address [150000:249999] Process P3

Holes [Free memory = 798576]:
Address [250000:1048575] len = 798576
```

### 13. Compaction (End - redundant)
```
RQ P0 100000 F
C
Status
```
Expected:
```
Partitions [Allocated memory = 100000]:
Address [0:99999] Process P0

Holes [Free memory = 948576]:
Address [100000:1048575] len = 948576
```

## Makefile

`make run`: Compiles `allocation.c` using the gcc compiler to the target `allocation`, then runs the program with argument 1 MB (1,048,576 Bytes) using command `./allocation 1048576`.

`make clean`: Removes any object and target files that were generated.

## About Developers

**Zamil Bahri**: A third year student at Wilfrid Laurier University taking the course CP386 - Operating Systems.

**Amirhossein Kompanizare**: A third year student at Wilfrid Laurier University taking the course CP386 - Operating Systems.

## Individual Contributions

### Zamil
* General setup and utility functions
  * Structure definitions
  * main()
  * readCommand()
  * cmdSplit()
  * cmdArrFree()
* releaseMemory()
* displayStatus()
* README.md

### Amir
* allocateMemory()
* compactMemory()

## License

Copyright (c) 2022 Zamil Bahri and Amirhossein Kompanizare

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
