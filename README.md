# Parallel Sorting

![Parallel Sorting](https://img.shields.io/badge/openmp-v6.0-green)

## Description

**Parallel Sorting** is a C-based application that explores sorting large arrays in parallel using frameworks such as OpenMP and Pthreads.

## Features

- 🧠 Implements parallel radix sort
- ⚙️ Uses OpenMP or Pthreads for multi-threaded execution
- 📊 Includes a benchmarking bash script (/benchmark) to get average runtimes across multiple runs

## Installation

To set up the Parallel Sorting, follow these steps:

1. **Clone the repository**:
  ```bash
  git clone https://github.com/JMcKesey/parallel-sorting.git
  cd parallel-sorting
  ```

  Example for Debian/Ubuntu systems:
  ```bash
  sudo apt-get build-essential
  ```

2. **Install the required dependencies:**
  ```bash
  openmp
  pthreads (availible by default on Linux)
  ```

## Build and Run
To run the application, use the following command in your terminal
  ```bash
  make build
  ./<output-file-name>
  ```

Replace <output-file-name> with the name of the actual compiled binary

## Benchmarking
Run the benchmarking script located in the benchmark directory:
```bash
cd benchmark
chmod +x benchmark.sh
./benchmark.sh ../<output-file-name>
```

This will execute the sorting algorithm multiple times and provide average execution times
