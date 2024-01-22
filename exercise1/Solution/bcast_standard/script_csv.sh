
#!/bin/bash

# Specify the mpirun command
mpirun_command="mpirun --map-by core -np"

# Specify the OSU Micro-Benchmarks binary
osu_bcast_binary="../osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_bcast"
osu_bcast_input="-x 100 -i 1000 -m 1:1 -f"

# Specify the output CSV file
output_csv="output.csv"
output_txt="output.txt"

# Add CSV header
echo "Processes,Size,AvgLatency(us),MinLatency,MaxLatency,Iterations" > $output_csv

# Loop through different inputs
for input in 2 4 8 16 32 64 128 256
do
    # Execute the mpirun command with the specified input
    output=$( $mpirun_command $input $osu_bcast_binary $osu_bcast_input )
    echo -n "$input," >> $output_csv
    echo "$output" |  grep '^1[[:space:]]' | awk -v RS='[[:space:]]+' '{ printf "%s%s", (NR==1 ? "" : ","), $0 } END { printf "\n"}'>>$output_csv

done

