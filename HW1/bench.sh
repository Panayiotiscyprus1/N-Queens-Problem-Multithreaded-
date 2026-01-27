#!/usr/bin/env bash
set -euo pipefail

RUNS=10
exe=./queens   # change if needed

sum=0

for i in $(seq 1 "$RUNS"); do
  echo "Run $i..."
  out="$($exe)"
  echo "$out"

  # extract the number before " microseconds"
  t=$(echo "$out" | awk -F': ' '/WALL TIME \(pthreads\):/ {print $2}' | awk '{print $1}')
  sum=$((sum + t))
done

avg=$((sum / RUNS))
echo "Average WALL TIME (pthreads): $avg microseconds over $RUNS runs"
