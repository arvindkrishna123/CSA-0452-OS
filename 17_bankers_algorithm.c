# OS Lab Programs 17 – 24

## Program List

| No. | File                          | Topic                                        |
|-----|-------------------------------|----------------------------------------------|
| 17  | 17_bankers_algorithm.c        | Banker's Algorithm – Deadlock Avoidance      |
| 18  | 18_producer_consumer.c        | Producer-Consumer using Semaphores           |
| 19  | 19_mutex_synchronization.c    | Process Synchronization using Mutex Locks    |
| 20  | 20_reader_writer.c            | Reader-Writer Problem using Semaphores       |
| 21  | 21_worst_fit.c                | Worst Fit Memory Allocation                  |
| 22  | 22_best_fit.c                 | Best Fit Memory Allocation                   |
| 23  | 23_first_fit.c                | First Fit Memory Allocation                  |
| 24  | 24_unix_file_syscalls.c       | UNIX System Calls for File Management        |

---

## Compilation Commands

### Standard (no special flags):
```bash
gcc 17_bankers_algorithm.c    -o 17_bankers_algorithm
gcc 21_worst_fit.c            -o 21_worst_fit
gcc 22_best_fit.c             -o 22_best_fit
gcc 23_first_fit.c            -o 23_first_fit
gcc 24_unix_file_syscalls.c   -o 24_unix_file_syscalls
```

### Requires `-lpthread` (POSIX Threads + Semaphores):
```bash
gcc 18_producer_consumer.c    -o 18_producer_consumer    -lpthread
gcc 19_mutex_synchronization.c -o 19_mutex_synchronization -lpthread
gcc 20_reader_writer.c        -o 20_reader_writer        -lpthread
```

---

## Key Concepts

### Program 17 – Banker's Algorithm
- Checks if system is in **safe state** before granting resources
- Finds a **safe sequence** using the Safety Algorithm
- Simulates **resource request** and rollback on unsafe state

### Program 18 – Producer-Consumer
- Bounded buffer of size 5
- Three semaphores: `mutex`, `empty`, `full`
- Producer sleeps 1s; consumer sleeps 2s (consumer slower)

### Program 19 – Mutex Synchronization
- Part A: Demonstrates **race condition** without mutex
- Part B: Shows correct result **with mutex**
- Part C: Bank account simulation with concurrent deposits/withdrawals

### Program 20 – Reader-Writer
- Multiple readers can read **simultaneously**
- Writers need **exclusive** access
- Uses First Readers-Writers solution (reader priority)

### Programs 21–23 – Memory Allocation
- **Worst Fit**: allocates to the largest sufficient block
- **Best Fit** : allocates to the smallest sufficient block
- **First Fit**: allocates to the first sufficient block (fastest)

### Program 24 – UNIX File System Calls
Demonstrates 12 system calls:
`creat`, `open`, `close`, `read`, `write`, `lseek`,
`stat`, `link`, `symlink`, `access`, `rename`, `unlink`

---

## Notes
- All programs target **Linux/Ubuntu**.
- Programs 18–20 require POSIX thread library (`-lpthread`).
- Program 24 creates and cleans up temporary files automatically.
