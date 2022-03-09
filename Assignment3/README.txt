##########################README file##################################

###To build and run the files in this assignment use the following commands:
* make
* ./transfProg input_hw3.txt N


# N=Number of threads processing the transactions
# for example:
# ./transfProg input_hw3.txt 4



###transfProg program takes input from the input_hw3.txt file
* In transfProg the main initializes the account balances after reading from the input_hw3.txt file and creates N threads to process the transactions on these accounts
* The transactions are processed on the go just like any electronic fund transfer should occur. However it is ensured that while atleast one of the resources is not acquired, then the threads must be blocked.
* Only when all required resources are acquired the transaction is processed and the resources are set back to available (released)
* In this program number of slots in the buffer for each thread (numOfBufSlots) is set to a fixed size 10
* The program is implemented using mutex and conditional variable(s) 
* Apart from this README file, one Makefile and the source file by the name transfProg.c itself are included in the zip