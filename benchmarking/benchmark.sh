#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <executable>"
  exit 1
fi

executable=$1

if [ ! -x "$executable" ]; then
  echo "Error: '$executable' is not an executable file or not found."
  exit 1
fi

total_time=0

echo "Running $executable 10 times..."

for i in {1..10}
do
  output=$(./$executable)
  time=$(echo "$output" | grep "Elapsed time" | awk '{print $3}')
  echo "Run $i: $time seconds"
  
  total_time=$(echo "$total_time + $time" | bc -l)
done

average=$(echo "$total_time / 10" | bc -l)

echo "Average elapsed time for $executable: $average seconds"