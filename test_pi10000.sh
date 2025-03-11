#!/bin/bash

NUM_RUNS=100
TOTAL_TIME=0

for ((i = 1; i <= NUM_RUNS; i++)); do
  OUTPUT=$(./pi10000)
  TIME=$(echo "$OUTPUT" | grep "Time" | awk -F ':' '{print $2}' | sed 's/ sec//g' | xargs)
  printf "Run #%3d: %s sec\n" "$i" "$TIME"
  TOTAL_TIME=$(awk "BEGIN {print $TOTAL_TIME + $TIME}")
done

AVERAGE_TIME=$(awk "BEGIN {print $TOTAL_TIME / $NUM_RUNS}")

echo "Average Time: $AVERAGE_TIME sec"
