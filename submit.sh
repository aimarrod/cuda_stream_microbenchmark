#!/bin/bash

#@ job_name = cuda_stream_benchmark
#@ initialdir = .
#@ output = streams_%j.out
#@ error = streams_%j.err
#@ total_tasks = 1
#@ gpus_per_node = 1
#@ features = k80
#@ wall_clock_limit = 00:02:00

nvprof --profile-from-start off --export-profile non-unified-same.nvvp ./streams -n 50 -s 1000000 --shuffle --non-unified-same
