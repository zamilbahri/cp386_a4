# Allocation

## Features

This program simulates operating system memory management at an abstract level. The user may allocate a certain amount of memory, and then assign portions of the memory pool to certain processes using one of three algorithmns: First Fit (F), Best Fit (B), and Worst Fit (W). They may release processes, which releases the memory back to the pool and makes it available. To get around memory fragmentation, the user may also request to perform memory compaction, which would remove any holes in between used memory.

The following is a list of commands:
|   Description  | Command |                 Arguments                |
|:--------------:|:-------:|:----------------------------------------:|
| Request Memory | RQ      | \<Process Name> <Size (Bytes)> \<Method> |
| Release Process| RL      | \<Process Name>                          |
| Display Status | Status  |                                          |
| Compact Memory | C       |                                          |
| Exit Program   | Exit    |                                          |
|                |         |                                          |


## Examples and Screenshots

Compile and run the program: `make run`

Request 200KB of memory for process P0 using first fit: `RQ P0 200000 F`

Request 350KB of memory for process P1 using first fit: `RQ P1 350000 F`

Request 300KB of memory for process P2 using first fit: `RQ P2 300000 F`

Release process P0: `RL P0`

Display the current `Status` of the total memory pool:

![Status](/screenshots/status.png)

Compact memory: `C`

![Compact](/screenshots/compact.png)

## Test cases

Expected: Hole at [0:200000]

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


### 2. First Fit
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

## Makefile

`make run`: Compiles `allocation.c` using the gcc compiler to the target `allocation`, then runs the program with argument 1 MB (1,048,576 Bytes) using command `./allocation 1048576`.

`make clean`: Removes any object and target files that were generated.

## About Developers

### Zamil Bahri

### Amirhossein Kompanizare

## Individual Contributions

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
