# BIOACME
Based on **FBS-dCME**: Finitely Buffered Optimal State space enumeration and
discrete Chemical Master Equation solution.

**Credits for original FBS-dCME**:
Youfang Cao
May 13, 2013
University of Illinois at Chicago
Chicago, IL


## Requirements
We currently support windows, linux and macos on python 3.10 and
above. This package wraps old fortran code, that can be tricky
to get compiled, so when you install it, all the compiled binaries
are downloaded for you. If you want to build it yourself, read the 
[build](#building-from-source) section.

## Installation
We recomend using a virtual environment to install the package.
Use pip, pipx, poetry, conda, or any package manager that supports
pypi.org packages. To install with pip on the current environment:
```
$ pip install bioacme
```

## Usage Step by Step
*Solving dCME by optimal enumeration of finitely buffered state space.*

### Step 1: Input Files

#### SBML of the reaction network

SBML format of level 2 version 1 is currently supported. The `build_state_space`
programs receive an SBML file as input, and parse all reaction network
information, including molecular species, reactions, parameters, kinetic laws
for each reactions, as well as the stoichiometry matrix of the network, for later
use in enumerating the state space. 
   
**IT IS IMPORTANT TO MAKE SURE THAT THE SBML FILE FORMAT IS VALID AND CORRECT,** 
**IN ORDER FOR THE `build_state_space` PROGRAMS TO WORK PROPERLY.**

#### Initial condition file

This file contains the information of the initial states that the
`build_state_space` programs require in order to enumerate the state space,
applying the FBS-dCME method. It is a pure text file, which should be in the
same format as in the example bellow:

```bash
$ cat init.txt
4 1
10 0 0 0, 1
```

The first line always includes two integer numbers. The first number indicates
how many molecular species there are in the network, and the second number
indicates how many initial states there are in this initial condition.

The second line in this example represents the only initial micro-state and its
probability.  The numbers before the comma (the micro-state) are integers
representing the copy number of each species, in the same order they appear in
the SBML species list. 

This initial condition could also be a distribution of multiple micro-states,
each with different probabilities. In that case, the file would contain all
micro-states in the distribution, with each micro-state and its corresponding
probability occupying one line. But usually the initial condition has only one
micro-state in it, such as in this example.


### Step 2: Finitely Buffered Optimal Statespace Enumeration

#### Algorithms

The detailed descriptions for the algorithms can be found in the following
published papers:

> Youfang Cao and Jie Liang, Optimal enumeration of state space of finitely
> buffered stochastic molecular networks and accurate computation of steady state
> landscape probability. BMC Systems Biology 2008, 2:30.

> Youfang Cao, Hsiao-Mei Lu and Jie Liang. Probability landscape of heritable
> and robust epigenetic state of lysogeny in phage lambda. Proceedings of the
> National Academy of Sciences USA 2010, 107(43):18445-18450. 

#### Programs 

Two different flavors of state space enumeration are implemented by using
*Breadth-First-Search* (**BFS**) and *Depth-First-Search* (**DFS**) network
traversal algorithms, respectively. The enumerated state spaces are exactly the
same by using either one of the two programs. The only difference is the order
of the states when enumerated. One command line example of state space
enumeration using `build_state_space_dfs` is given below. The usage of the **BFS**
version is the same, but called as `build_state_space_bfs`. 

```
Usage:
 -h  --help                 Prints this help
 -m  --sbml <sbmlfile>      Input SBML file
 -i  --init <initfile>      Initial condition file
 -s  --statespace <ssfile>  Output state space file
 -v  --verbose              Switch for more output
```
Command-line example for using `build_state_space_dfs` to enumerate the state
space of the classical birth and death process: 

```bash
$ build_state_space_dfs -m mBirthDeath_P1.xml -i init_bd40.txt -s states_DFS.txt
```

Here, `mBirthDeath_P1.xml` is the SBML file of network; `init_bd40.txt` is the
initial condition file; and `states_DFS.txt` is the output state space file. 

### Step 3: Generating transition rate matrix from the state space

The corresponding transition rate matrix of the state space can be generated
using the `net2matrix` program included in the package. `net2matrix` reads the
input SBML and the state space files enumerated in 
[step 2](#step-2-finitely-buffered-optimal-statespace-enumeration), and
calculates the transition rates between each pair of states in the whole state
space, and outputs the sparse transition rate matrix in a coordinate list
(`COO`) format file. 

```
Usage:
 -h  --help                 Prints this help
 -m  --sbml <sbmlfile>      Input SBML file
 -s  --statespace <ssfile>  State space file
 -t  --omatrix <tmfile>     Output transition matrix file
 -r  --rlabel               Put reaction labels aside the matrix entries
 -x  --reaction <n>         About which single reaction you want the matrix to be
 -v  --verbose 0            Switch for more output
```

Command-line example for using `net2matrix` to generate the transition rate
matrix for the above birth and death example:

```bash
$ net2matrix -m mBirthDeath_P1.xml -s states_DFS.txt -t tm_DFS.txt
```

Here, `tm_DFS.txt` is the output transition rate matrix. 

### Step 4: Solving the steady-state probability distribution (landscape) of dCME

The *steady-state probability distribution of the discrete chemical master
equation* (**dCME**) can be solved from the transition rate matrix using multiple
linear solvers. In this package, we use the *Successive Over Relaxation* (**SOR**)
method for the steady state solution. The description for the SOR method can be
found in the book "Introduction to the Numerical Solution of Markov Chains" by
William J. Stewart. 

```
Usage:
 -h  --help                        Prints this help
 -m  --mtrx <matrix file>          Input matrix file
 -s  --steady <steady state file>  Output steady state distribution file
 -o  --omega <1.0>                 Omega value for Successive Over Relaxation
 -t  --tol <1E-10>                 Error tolerance
 -v  --verbose 0                   Switch for more output
 ```

Typical command-line example for using `ssor` program to solve the
steady-state distribution of dCME, using the birth and death as example: 

```bash
$ ssor -m tm_DFS.txt -s steady_DFS.txt
```
Here, `tm_DFS.txt` is the input transition rate matrix generated in 
[step 3](#step-3-generating-transition-rate-matrix-from-the-state-space), and
`steady_DFS.txt` is the output steady state probability distribution vector. 

The `ssor` program provides two more options. The option `-o` is for changing
the relaxation parameter omega for SOR method. The omega value should be between
(0, 2). The default value in `ssor` is 1.0. The other option `-t` is for
changing the convergence error tolerance. The default error tolerance is 
$1 \times 10^{-10}$ in `ssor`. The error in `ssor` is absolute error with infinity
norm. 

The format of probability distribution file `steady_DFS.txt` is
straight-forward. The distribution is a vector with the same length as the state
space size. Each line of the probability distribution file is the probability of
the corresponding state in the state space. Only the first line is exceptional,
which is always "1" indicating the summation of the rest of the probability
vector. 

### Step 5: Time evolution of dCME probability distribution (landscape) 

Not only steady-state distribution, but also the time evolution of dCME
probability landscape can be calculated. Program `mxexp` is provided with this
package to calculate the time evolution of dCME probability distribution.
`mxexp` is a Fortran program based on `EXPOKIT` (Sidje 1998). 

```
Usage:
 -m  --tm <matrixfile>  Input rate matrix file
 -t  --time 100         Total time for time evolution
 -d  --dt 10            Time step
 -p  --p0 <p0file>      Initial distribution file.
          0             Uniform distribution
          1             1-probability at the first state
          p0file        Initial distribution file
 -i  --idir <./>        Intermediate directory
```

The command-line example for using `mxexp` program to calculate time evolution
dCME probability landscape, using the birth and death as example: 

```bash
$ mxexp -m tm_DFS.txt -t 100 -d 1 -p 0 -i ./dynamics/
```

Here, the parameter `-t` specifies the total time that `mxexp` will calculate
for. The parameter `-d` specifies the fixed size of time step.  The parameter
`-p` indicates the initial distribution, which can take different values. The
initial distribution is uniform distribution if `-p 0` is set; the first state
in the space will be assigned probability 1 and all the rest 0, if `-p 1` is
set; otherwise, an initial distribution file has to be provided. The parameter
`-i` indicates the intermediate directory to store intermediate probability
landscapes at each intermediate steps during the calculation. 

## Examples
On the `examples` directory, you can find some usage examples taken from the
original fortran package. The programs are called exactly the same when invoked
by this python wrapper, so the examples should work without any modification.

## Python interface

Each program included in the package has an corresponding Pyhon function in the
`bioacme` module that wraps the function call. The Python interface is provided
in the `bioacme` module. They can be called with the same name as their terminal
command counterparts.

```python
from bioacme import (
    build_state_space_bfs,
    build_state_space_dfs,
    mxexp,
    net2matrix,
    ssor,
)
```

The parameters can be passed to the function as **positional arguments** or
**keyword arguments**. The positional arguments are the same as the command-line
arguments. The keyword arguments are the same as the command-line arguments, but
with the leading dash (`-`) removed. 

For example, the `net2matrix` program can be called in Python as follows:

```python
from bioacme import net2matrix

net2matrix('-m', 'mBirthDeath_P1.xml', '-s', 'states_DFS.txt', '-t', 'tm_DFS.txt')

net2matrix(m='mBirthDeath_P1.xml', s='states_DFS.txt', t='tm_DFS.txt')
```

If the function expects input from `stdin` during the execution, the input can
be passed as a string to the `input` keyword argument.

The programs can also be called from the command line using the `python -m bioacme <program> <args>` syntax. The program name is not case sensitive and disregards any underscores.

## Building from source

The source code is written in C++ and Fortran located in the `src` directory.
[libsbml](https://sbml.org/software/libsbml/) library and it's building
requirements (`zlib`,`libxml`,`bzip2`) are required to build the source code. To
build the source code, you need to have a C++ compiler and Fortran compiler
installed and the `cmake` build system. If you are using an Ubuntu-based Linux
distribution, you may need to build and install `libsbml` from source.

To build the source code, run the following commands in the root directory of
the project:

```bash
$ cmake -B build
$ cmake --build build
```

The built programs are located in the `bin` directory inside the `build`
directory.

### Generating python wheel package

The python package is built using [poetry](https://python-poetry.org/). To build
the python package just execute `poetry build` in the root directory of the
project. The built package will be located in the `dist` directory. Note that
build.py script also builds the source code and copies the built programs to the
`bioacme` package directory, so you don't need to build the source code
separately if you are building the python package.
