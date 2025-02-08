# wmml-reader
wmml Reader is a program designed to read files of the following type.wmml.

What is a .wmml file? File.wmml is a file that represents a table. The first 2 bits store the number of fields in the object. Then the objects written to the file go one after the other, thereby forming a list.

When launching the program, you must specify as an argument the file you are going to work with.
After that, you can activate 3 operating modes by entering 3 different symbols.
r - (read) Starts the recursive process of reading the file. This mode will read the file and output it to the console until it reaches the end of the file.
d - (delete) Deletes the selected file and terminates the program.
c - (create) Allows you to create your own file and write data to it.
