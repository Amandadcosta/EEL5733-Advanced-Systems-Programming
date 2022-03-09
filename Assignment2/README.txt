##########################README file##################################

###To build and run the files in this assignment use the following commands:
* make
* ./combiner a b <input.txt
# a=Number of slots in the buffer
# b=Number of reducer threads (can be less than or equal to number of unique users)
# for example:
# ./combiner 10 7 <input.txt




###combiner program takes input from the input.txt file
* In combiner the main creates one mapper and N reducer threads
* It includes dynamic memory allocation if the number of unique userIDs is greater than the number of reducer threads
* Apart from this README file, one make file and the source file itself are included 