#!/bin/bash

# Specify the mpirun command
mpirun_command="mpirun --map-by core -np"

# Specify the OSU Micro-Benchmarks binary
osu_barrier_binary="../osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_barrier"

osu_bcast_input="-x 100 -f -i 1000"

# Specify the output text file
output_file="output.txt"

# Loop through different inputs
for input in 2 4 8 16 32 64 128 256
do
    # Execute the mpirun command with the specified input
    output=$( $mpirun_command $input $osu_barrier_binary $osu_bcast_input )

    # Append a line indicating the number of processes to the text file
    echo "Processes: $input" >> $output_file

    # Append the output to the text file
    echo "$output" >> $output_file

    # Add a separator line for better readability
    echo "====================" >> $output_file
done
