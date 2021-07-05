# C Project: 12-bit Assembler

# About

This project is an assignment in a C programming course at the open unvieristy (20465). The purpose of this project is to build an assembler for 15-instruction asssembly language, for an imaginary 12-bit CPU and 4096 bits of memory size. This project was done by @GoshaDo. For any issues, questions and ideas turn to the Issues tab.

# Source structure (Modules)

1.`assembler` - main function definition, argv & argc processing, single file processing.
2.`first_pass` - contains functions for processing a single line in the first pass and a code line in the first pass.
3.`second_pass` - contains function for processing a single line in second pass and replacing symbol by it's address.
4.`code` - contains useful function for processing code.
5.`instructions` - contains useful function for processing instruction.
6.`globals.h` - contains type and constant definitions.
7.`table` - contains definition of table-like linked list data structure.
8.`utils` - contains general-purposed, project-wide functions.
9.`writefiles `- contains methods for writing output files: *.ob, *.ext and *.ent

# Usage

`./assembler file1 file2 file3 ...` Will start the processing of each file (specify WITHOUT extension!). No support for double-quoting file names.

# Build

Using `make` command (Makefile)
