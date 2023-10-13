#!bin//bash

#export PATH=$PATH:$(pwd)
rm error_file.txt
rm schedule_matrix.txt
rm debug.txt

g++ -o simulator simulator.cpp
#./simulator -arrivals 10 -life 1500 -critical 70 -maximum 280 -prob 20 -semaph 20
#./simulator -arrivals 5 -life 10 -critical 10 -maximum 50 -prob 20 -semaph 2
./simulator -arrivals 1 -life 15 -critical 2 -maximum 50 -prob 60 -semaph 2
echo '______ LOG FILE _______________ '
echo ' '
cat schedule_matrix.txt

echo '______ ERROR FILE _____________ '
echo ' ' 
cat error_file.txt
echo ' '
echo '______ DEBUG FILE _____________ '
echo ' ' 
cat debug.txt

echo ' '

