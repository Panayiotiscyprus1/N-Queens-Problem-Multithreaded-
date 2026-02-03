#!/usr/bin/env bash
set -euo pipefail

RUNS=10

exe="${1:-queens}"

# If no path given, assume current directory
if [[ "$exe" != */* ]]; then
  exe="./$exe"
fi

if [[ ! -x "$exe" ]]; then
  echo "Error: '$exe' is not executable"
  exit 1
fi

# Detect timing label based on executable name
case "$(basename "$exe")" in
  *seq*)
    label="sequential"
    ;;
  *omp*)
    label="omp"
    ;;
  *pth*|*pthread*)
    label="pthreads"
    ;;
  *)
    echo "Error: could not infer timing label from '$exe'"
    exit 1
    ;;
esac

sum=0

for i in $(seq 1 "$RUNS"); do
  echo "Run $i..."
  out="$("$exe")"
  echo "$out"

  t=$(echo "$out" \
      | awk -F': ' "/WALL TIME \\($label\\):/ {print \$2}" \
      | awk '{print $1}')

  if [[ -z "$t" ]]; then
    echo "Error: could not extract timing for label '$label'"
    exit 1
  fi

  sum=$((sum + t))
done

avg=$((sum / RUNS))
echo "Average WALL TIME ($label): $avg microseconds over $RUNS runs"
