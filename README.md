# BIOACME
Based on **FBS-dCME**: Finitely Buffered Optimal State space enumeration and
discrete Chemical Master Equation solution

**Credits for original FBS-dCME**:
Youfang Cao
May 13, 2013
University of Illinois at Chicago
Chicago, IL


## Usage Step by Step
*Solving dCME by optimal enumeration of finitely buffered state space.*

### Step 1: Input Files

#### SBML of the reaction network

SBML format of level 2 version 1 is currently supported. The `buildStateSpace`
programs receive a SBML file as input, and parse all reaction network
information, including molecular species, reactions, parameters, kinetic laws
for each reactions, as well as stoichiometry matrix of the network, for later
use in enumerating the state space. 
   
**IT IS IMPORTANT TO MAKE SURE THE SBML FILE FORMAT IS VALID AND CORRECT,** 
**IN ORDER FOR THE `buildStateSpace` PROGRAMS TO WORK PROPERLY.**

#### Initial condition file

Initial condition file contains the information of the initial states that the
`buildStateSpace` programs require in order to enumerate the state space,
applying the FBS-dCME method. The initial condition file is stored in a pure
text file, which should be organized as in the example bellow:

```bash
$ cat init.txt
4 1
10 0 0 0, 1
```

The first line always includes two integer numbers. The first number indicates
how many molecular species there are in the network, and the second number
indicates how many initial states there are in this initial condition file. In
principle the initial condition could be a distribution containing multiple
micro-states and each with different probabilities, so in that case the initial
condition file can contain all micro-states in the distribution with each state
and its corresponding probability occupies one line. But, in most cases, an
initial condition has only one micro-state in it, like the above example initial
file. 

The second line of the above initial file is the initial micro-state and its
probability. The numbers in micro-states are integers representing the copy
number of each species which is in the order that they appear in the SBML
species list. 

### Step 2: Finitely Buffered Optimal Statespace Enumeration

#### Algorithms

The detailed descriptions for the algorithm should be referred to following
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
same by using either of two programs. The only difference is the order of the
states when enumerated. One command line example of state space enumeration
using `buildStateSpace_DFS` is given below. The usage of the **BFS** version is
the same, but called as `buildStateSpace_BFS`. 

```
Usage:
 -h  --help                 Prints this help
 -m  --sbml <sbmlfile>      Input SBML file
 -i  --init <initfile>      Initial condition file
 -s  --statespace <ssfile>  Output state space file
 -v  --verbose              Switch for more output
```
Command-line example for using `buildStateSpace_DFS` to enumerate the state
space of the classical birth and death process: 

```bash
$ buildStateSpace_DFS -m mBirthDeath_P1.xml -i init_bd40.txt -s states_DFS.txt
```

Here, `mBirthDeath_P1.xml` is the SBML file of network; `init_bd40.txt` is the
initial condition file; and `states_DFS.txt` is the output state space file. 

### Step 3: Generating transition rate matrix from the state space

The corresponding transition rate matrix of the state space can be generated
using `net2matrix` program in the package. `net2matrix` reads the SBML file and
state space file, which has been enumerated in 
[step2](#step-2-finitely-buffered-optimal-statespace-enumeration), as inputs,
and calculate the transition rates between each pair of states in the whole
state space, and output the sparse transition rate matrix in coordinate list
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
straight-forward. The distribution is a vector with length same as the state
space size. Each line of the probability distribution file is the probability of
the corresponding state in the state space. Only the first line is exceptional,
which is always "1" indicating the summation of the rest of the probability
vector. 

### 5: Time evolution of dCME probability distribution (landscape) 

Not only steady-state distribution, but also the time evolution of dCME
probability landscape can be calculated. Program `mxexp` is provided with this
package to calculate the time evolution of dCME probability distribution.  The
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