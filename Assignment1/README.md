##########################README file##################################

###To build and run the files in this assignment use the following commands:
* make
* make test



###The input to the Mapper program is in the input.txt file
* Mapper can process upto 100 tuples 
* Mapper calculates the score for a given tuple with (userID,action,topic) input
* Mapper output is printed to the terminal as well as the Mapper_Output.txt file




###Mapper_Output.txt is the input to the Reducer program
* Reducer can process upto 100 tuples
* Reducer calculates the totalscore for a given tuple with (userID,topic,score) input
* Reducer output is printed to the terminal as well as the output.txt file




###Combiner program takes input from the input.txt file
* In Combiner the parent forks twice to create two child processes-namely the Mapper and the Reducer
* The Mapper writes data to the pipe while the Reducer reads data(scores) from the pipe.
* The Parent waits for both children to terminate , closes both ends of the pipe and then terminates
