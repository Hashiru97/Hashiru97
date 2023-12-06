#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_JOBS 100

int preemptive = 0;

typedef struct {
  int arrival_time;
  int burst_time;  // Replace processing_time with burst_time
  int priority;
} Job;

int read_jobs(FILE *input_file, Job jobs[]) {
  int num_jobs = 0;
  while (fscanf(input_file, "%d %d %d", &jobs[num_jobs].arrival_time, &jobs[num_jobs].burst_time, &jobs[num_jobs].priority) == 3) {
    num_jobs++;
  }
  return num_jobs;
}

void fcfs(Job jobs[], int num_jobs) {
  int current_time = 0;
  int wait_time = 0;  // Declare wait_time here
  int turnaround_time = 0;  // Declare turnaround_time here
  for (int i = 0; i < num_jobs; i++) {
    printf("Processing job %d\n", i + 1);
    if (current_time < jobs[i].arrival_time) {
      current_time = jobs[i].arrival_time;
    }
    wait_time += current_time - jobs[i].arrival_time;
    turnaround_time += current_time - jobs[i].arrival_time + jobs[i].burst_time;
    current_time += jobs[i].burst_time;
  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);
}

void sjf(Job jobs[], int num_jobs) {
  int current_time = 0;
  int wait_time = 0;  // Declare wait_time here
  int turnaround_time = 0;  // Declare turnaround_time here
  while (1) {
    int shortest_job = -1;
    for (int j = 0; j < num_jobs; j++) {
      if (jobs[j].arrival_time <= current_time && jobs[j].burst_time > 0) {
        if (shortest_job == -1 || jobs[j].burst_time < jobs[shortest_job].burst_time) {
          shortest_job = j;
        }
      }
    }
    if (shortest_job == -1) {
      current_time++;
      continue;
    }
    printf("Processing job %d\n", shortest_job + 1);
    wait_time += current_time - jobs[shortest_job].arrival_time;
    turnaround_time += current_time - jobs[shortest_job].arrival_time + jobs[shortest_job].burst_time;
    current_time += jobs[shortest_job].burst_time;
    jobs[shortest_job].burst_time = 0;
    if (preemptive) {
      break;
    }
  }
  printf("Average wait time: %f\n", (double)wait_time / num_jobs);
  printf("Average turnaround time: %f\n", (double)turnaround_time / num_jobs);
}

// (similar changes for priority and round_robin functions)

int main(int argc, char **argv) {
  int opt;
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  while ((opt = getopt(argc, argv, "f:o:p")) != -1) {
    switch (opt) {
      case 'f':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'p':
        preemptive = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-f input_file] [-o output_file] [-p]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (input_file_name == NULL) {
    fprintf(stderr, "Error: No input file specified.\n");
    exit(EXIT_FAILURE);
  }

  FILE *input_file = fopen(input_file_name, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Error: Could not open input file %s.\n", input_file_name);
    exit(EXIT_FAILURE);
  }

  Job jobs[MAX_JOBS];
  int num_jobs = read_jobs(input_file, jobs);
  fclose(input_file);

  FILE *output_file = NULL;
  if (output_file_name != NULL) {
    output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
      fprintf(stderr, "Error: Could not open output file %s.\n", output_file_name);
      exit(EXIT_FAILURE);
    }
  }

  printf("First-Come First-Serve:\n");
  fcfs(jobs, num_jobs);
  // (similar changes for other scheduling functions)

  if (output_file != NULL) {
    fclose(output_file);
  }

  return 0;
}