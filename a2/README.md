# Notes on Makefiles and C
This Makefile will compile the various examples in this assignment to executable files. The files are as follows:

* `two`    (Figure 1 in the handout)
* `lock`   (Figure 3 in the handout)
* `sema`   (Figure 4 in the handout)
* `pd`     (Figure 5 in the handout)
* `birds`  (Figures 7 and 8 in the handout)
* `gpus`   (Figures 9 and 10 in the handout)


These are compiled using different Makefile rules. For example, `make two` will generate the "two" executable file, which can be run by calling it directly in a terminal `<filepath_to_two>/two` (assuming you are in the same directory). For example, if you are in the same directory as two_threads.c, `make two` followed by `./two` will compile and then run the code.

`make all` will attempt to make all targets. `gpus` and `birds` will fail at first because they are not yet implemented, but the rest should compile fine.

`make clean` will attempt to remove all build files and executables. It may generate warnings and errors if some of the build files or executables do not exist.

Please read here for more info:
https://www.gnu.org/software/make/manual/html_node/Introduction.html
