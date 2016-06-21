### Description

A served-based tool with web interface providing basic exploit analysis.

Core system is in C++ while the web interface is designed in Django framework. Code emulation is provided by libemu library available here: http://libemu.carnivore.it/
Tool is being designed and implemented as a BSc Thesis.

### Main features:

#### Analyze:

 * system API called by GetProcAddress
 * DLLs loaded through LoadLibraryA
 * check for getPC (resulting in shellcode offset in file)
 * loops detection (start, vertexes, iterations)
 * search for similar code snippets among all samples
 * graphs comparison (with hash)

#### Input:

 * support for BIN files (plain x86 machine code)
 * support for PCAP files
 
#### Output:

 * generation of the execution graph
 * console output
 * database output

#### Analyze:

Grouping decides how samples should be grouped by comparing their loop hashes and computing their resemblence rate.

#### Infrastructure:

 * Django web interface
 * PostgreSQL database for results
 * Server-side program in C++ for analysis
 * Shared library of the program above to allow incorporating it into other projects
