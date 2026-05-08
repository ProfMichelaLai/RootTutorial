# RootTutorial
Short hands on getting started in ROOT Cern for undergrads - 2 hours
The macros are designed to give a set of useful examples to getting started in ROOT framework, with no previous experience in C++. To run the examples, please first follow the installation instructions. 

To run them, 
root -l macro.C+ 

for functions.cpp, first compile it with g++
g++ mycode.cpp `root-config --cflags --libs` -o mycode
then run the output executable
./mycode


