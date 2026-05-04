_This project has been created as part of the 42 curriculum by r-selva-2_

# Description

The "philosophers" project serves as an introduction to threads and resource
sharing across threads and processes.

This aim is pursued through an exercise where a group of philosophers are
depicted sitting around a table. Each philosopher must eat using two forks which
are shared with other philosophers. The philosophers take turns eating,
sleeping, and thinking. If a philosopher goes too long without eating, they die
and the simulation ends.

The main learning goal of this project is understanding that sharing data among
threads or processes may be dangerous because simultaneous manipulation of data
can result in undefined behavior. Therefore, we must protect the resources to
prevent these issues.

## Mandatory part

Between each pair of adjacent philosophers there is a fork, and philosophers
must use the forks next to them.

Philosophers are represented by threads and forks are variables which need to be
protected with a mutex to ensure that two philosophers do not use the same fork
at the same time.

## Bonus part

In this case, all forks are shared among all philosophers.

Instead of threads, processes are now used to represent philosophers, and the
forks are represented by a single semaphore.

# Instructions

As usual, from the root directory, execute:

- `make philo` to compile the mandatory part program
- `make philo_bonus` to compile the bonus part program
- `make all` to compile both

To execute them, both programs expect four or five arguments, in the following
order:

- `number_of_philosophers`

  The number of philosophers and also the number of forks.

- `time_to_die`

  If a philosopher has not started eating within this time since the start of
  their last meal or the start of the simulation, they die.

- `time_to_eat`

  The time it takes for a philosopher to eat.

- `time_to_sleep`

  The time a philosopher spends sleeping.

- `number_of_times_each_philosopher_must_eat` (optional)

  If all philosophers have eaten at least this many times, the simulation stops.

All time arguments are given in milliseconds. If the last argument is not
specified, the simulation stops when a philosopher dies.

Usage examples:

```
./philo 4 410 200 200 50
```

```
./philo_bonus 5 610 200 200 50
```

```
./philo 8 390 200 200
```

# Death conditions

To understand the conditions under which a philosopher will die, one must
consider the parity of `number_of_philosophers`.

### Even number of philosophers

If `number_of_philosophers` is even, the following inequalities should hold:

```
time_to_die > 2*time_to_eat

time_to_die > time_to_eat + time_to_sleep
```

With an even number of philosophers, perfect parallelism is possible. For
example, with 4 philosophers, while philosopher 1 eats (using forks 1 and 2),
philosopher 3 can eat simultaneously (using forks 3 and 4). Philosophers 2 and 4
must wait. In the optimal schedule:

- Turn 1: Philosophers 1 and 3 eat
- Turn 2: Philosophers 2 and 4 eat

Since each philosopher must wait one full turn before eating again, the maximum
gap between meals is `time_to_eat + time_to_sleep`.

### Odd number of philosophers

If `number_of_philosophers` is odd, the following inequalities should hold:

```
time_to_die > 3*time_to_eat

time_to_die > time_to_eat + time_to_sleep
```

With an odd number, only `floor(N/2)` philosophers can eat simultaneously.
For example, with 5 philosophers and 5 forks, at most 2 can eat at once. The
optimal schedule requires rotating through 3 eating turns before a philosopher
can eat again:

- Turn 1: Philosophers 1 and 3 eat
- Turn 2: Philosophers 2 and 4 eat
- Turn 3: Philosopher 5 eats
- Repeat

Thus, waiting for 2 full eating cycles of others, which takes `2 × time_to_eat`
ms, plus their own `time_to_eat`, results in `3 × time_to_eat` ms of total
separation.

### Large numbers of philosophers

With larger quantities of philosophers, due to CPU limitations, the differences
between `time_to_die` and its respective multiple should be more pronounced.

# Testing

To test more accurately while avoiding the performance cost of terminal output
(which can delay philosopher execution and cause false deaths), use this
pipeline:

```
./philo 200 450 200 200 20 > outfile && cat outfile | grep died || echo "nobody died"
```

This redirects output to a file (faster than terminal rendering) and then checks
if any death occurred. If a death is found, you can examine the output:
- to see full output:
	```
	cat outfile
	```
- to track the logs of the dead (or any other) philosopher:
	```
	cat outfile | grep " 42 "
	```


# Resources

To understand the general concepts and expand my knowledge beyond this project's
limitations, I have watched the CodeVault course *Unix Threads in C* on YouTube.

For a deeper understanding of the allowed functions, I have read their
respective manuals on `https://man7.org` and `https://pubs.opengroup.org`, since
my PC did not have the manual entries for most of them.
