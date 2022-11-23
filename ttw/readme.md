# ttw - Time Trace Wrapper

`ttw` is a compiler launcher used to catch the flags that are passed to the
compiler.

It accepts a time-trace export path, and the compile command for a file.

If `-ftime-trace` is passed to the compiler, run the command and copy the
time-trace file to the export path.

Otherwise, it will run the command, measure its execution time using
`getrusage`, and generate a time-trace file with only the compiler execution
time. This allows comparing compiler execution time scaling between GCC and
Clang.
