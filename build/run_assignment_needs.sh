#!/bin/bash

if [[ $# -ne 1 ]]
then
    echo "Correct usage: $0 <genetic|hill-climbing>"
    exit 1
fi

if [[ $1 != "genetic" ]] && [[ $1 != "hill-climbing" ]]
then
    echo "Incorrect algorithm provided. Use \"genetic\" or \"hill-climbing\"."
    exit 1
fi

if [[ -d ./solutions ]]
then
    rm -rf ./solutions
fi

for n in 8 16 32
do
    mkdir -p ./solutions/$1/$n
    for executions in {1..100..1}
    do
        echo "Executing $1 with a board size of ($n) for the #$executions time..."
        ./n_queen_problem_solver $n $1 ./solutions/$1/$n/$executions.txt
    done
done

exit 0
