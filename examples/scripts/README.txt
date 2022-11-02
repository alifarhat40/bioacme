PERL AND R SCIPTS

Examples of Perl and R scripts for parsing probability distributions
corresponding to specified state space, and ploting figures. Functions of each
script in this directory are explained below.

1. parseResult-XY.pl

Function: 
  Calculating the marginal distribution of a probability landscape projected
  on a X-Y two species plane. 

Parameters: 
  --rs resultfile
    Specify the input probability landscape, which is the output results from
    steady state calculation by solving the CME, or the outputs from time
    evolutions calculations from matrix exponential using EXPOKIT. 

  --ss statespacefile
    The state space file, which should be the same state space file used to
    generate the transition rate matrix. 

  --sc scale
    The size of the X-Y plane to be parsed. The scale for X and Y is the same.
    The default value is 200. 

Output:
  The calculated marginal distribution of X-Y plane is outputed to the screen.
  Output redirection '>' can be used to save the results to a file. The output
  file is a three-column table, with the first two columns being the
  coordinates on X-Y plane with integer incremental grids, and the third
  column being the probability at each point. 


2. parseResult-ES.pl

Function: 
  Calculating the marginal distribution of a probability landscape projected
  on the 1-D BUFFER space. This could be an indicator whether the buffer size
  is large enough. The probability of BUFFER=0 is the error of the CME of
  using this BUFFER size. 

Parameters: 
  --rs resultfile
    Specify the input probability landscape, which is the output results from
    steady state calculation by solving the CME, or the outputs from time
    evolutions calculations from matrix exponential using EXPOKIT. 

  --ss statespacefile
    The state space file, which should be the same state space file used to
    generate the transition rate matrix. 

  --sc scale
    The size of the BUFFER space to be parsed. The default value is 100. 

Output:
  The calculated marginal distribution of the BUFFER is outputed to the
  screen.  Output redirection '>' can be used to save the results to a file.
  The output file is a two-column table, with the first column being the
  number of remaining BUFFER, and the second column being the probability. The
  probability value associated with BUFFER=0 is the error for using the
  current BUFFER size. 



parseResult-all.pl

plotls-nozlim-tabX-cl.R
plotls-nozlim-tabXY-cl.R
plotls-zlim-tabXY-cl.R

