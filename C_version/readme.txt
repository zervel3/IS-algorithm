there are total three steps to validate the IS algorithm's performance.
    step1: generate Influence and Susceptibility of individuals according to the unifrom distribution from 0 to 1,
              and calculate individuals f and g, finally get the estimated I, S, and estimated edge weight p_{ij}.
    step2: Run independent cascades model to disturb the edge weight p_{ij}, and get the disturbed p_{ij}, caculate disturbed f and g
    step3: take disturbed f and g as input to our IS-algorithm to get individuals' I and S.

    run the program in command line: ./main.sh

explanation of the prameters of three steps.
    ##  for program step1
    ##  parameter1: dataset name.
    ##  paraemter2: the infinitesimal to avoid dividing by zero.
    ##  parameter3: the iteration times.

    ## for program step2
    ##  parameter1: dataset name.
    paraemter2: the infinitesimal(just for find the file path, we don't use it in IC spreading process)
    paraemter3: realizations of Independent cascades process.

    ## for program step3
    ##  parameter1: dataset name.
    ##  realizations of Independent cascades process(here the parameter just compose the input filename)
    ##  the iteration times.

Tips:
     The effect of step2 is to simulate the noise of realistic situation.
     If you use the linux, please compile the program again by yourself and modify the ./main.sh file 
     Just use the following command to compile
     gcc -fopenmp -o step1 main_step1.c
     gcc -o step2 main_step2.c
     gcc -fopenmp -o step3 main_step3.c