#!/bin/bash

for alg in FIFO LRU
do
    for type in sim_1 sim_segfault_1 sim_segfault_2
    do
        input_file_name="inputs/${type}"
        output_file_name="tests/${type}-output-${alg}"

        alg_param="-s $alg"

        for mode_param in -v
        do
            for max_frames in 5 10
            do
                soln_file=${output_file_name}${mode_param}-flag-${max_frames}-frames
                
                sim_command="./mem-sim -f ${max_frames} $mode_param $alg_param $input_file_name"
                # echo "${output_file_name}_${mode_param}_flag"
                echo "Executing $sim_command" 

                $sim_command &> my_output

                echo "    Comparing with ${soln_file}"
                diff_result=$(diff -b -B my_output ${soln_file})

                if [ "$diff_result" != "" ]; then
                    diff -b -B my_output ${soln_file} &> my_output.diff
                    echo "    Your output does not match ${soln_file}. Please check my_output and my_output.diff for details."
                    exit
                else
                    echo "    Test passed!"
                    echo
                fi
            done
        done
    done
done
