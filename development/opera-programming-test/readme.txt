Opera Programming Test

   Note: this programming test is confidential and copyrighted; DO NOT post
   it (or your answers) on public web-sites or distribute it to others !

For this test you are given responsibility of a couple of classes that
implement logical functions as given by a truth table.
The interface and implementation of these classes are found in files
logicalfunction.h and logicalfunction.cpp respectively.
In addition, there are: a small program that tests the basic usage of the
classes; and a file testcases.h intended for use in the last task.

However, as of now these classes have a number of shortcomings, and it is your
task to correct these.
First, take a look through the code and make a quick estimate of how much time
you think you will spend on each of the tasks given below.
Then, as you do the programming, record how much time you actually spend.

TASKS:

1) The class interfaces do not follow good object-oriented design principles,
   which makes them bug-prone and hard to extend/subclass properly.

   Improve the object-oriented design of the classes.

2) The class interfaces are almost completely undocumented.

   Include proper interface documentation in the .h-file.

3) There seems to be a bug somewhere, as the test program seems to end with a
   SEGV.

   Find the bug, explain what is wrong, and make a working fix.

4) The class LogicFunction only supports functions defined by a table.
   For some logical functions, especially with many inputs, the truth tables may
   be inconveniently large. As an example, a 10-input XOR would need 1024 rows,
   whereas you could implement it by a couple of lines of C++ code.

   Make modifications that enable client code to define their own LogicFunction
   objects, whose behaviour is determined by their own C++ code, that can be used
   with the standard LogicFunction and LogicProcessor classes.

5) Using your modified classes, define LogicProcessors that are able to
   inspect an 8x8 monochrome pixel grid, represented by 64 inputs, and report
   whether this grid has horizontal, vertical or rotational symmetry.

   Verify your LogicProcessors by using the grids given in testcases.h as
   input.

Please contact progtest@opera.com if you have questions, or if you don't expect
to have time to complete the test within two weeks.

We hope that you have fun!
