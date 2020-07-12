# Benchmark different ways to validate a string and count occurrences

One of the exercises in [Exercism](https://exercism.io) is "nucleotide-count". The task is to create a `class counter` with a constructor that takes a strand of DNA. `counter` must provide methods to return the number of occurrences of a given nucleotide, or to return a `std::set` with counts for all nucleotides. The methods must also validate their arguments.

There are many ways to validate a string of nucleotides and to count them. I want to benchmark them to figure out how their performance differs.

On my AMD Ryzen Threadripper 1920X 12-Core Processor running Ubuntu 20.04 Focal Fossa with GCC-9.3.0 the most common approach was about 18 times slower than the fastest one.
