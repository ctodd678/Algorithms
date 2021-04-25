Connor Todd

To run program type "make" and then "./main".

The program runs all parts in one go. Specify the desired txt files before Part 1 and Part 2 runs
(will be prompted for input).


Q2.4 ANALYSIS

WORDS               BRUTE-FORCE     HORSPOOLS       SHIFTS (B.F)    SHIFTS(H.P)     RATIO (time)    RATIO (SHIFTS)
-----               -----------     ---------       ------------    -----------     ------------    -------------
the                 19.0ms          7.0ms           3268252         1132821         0.37            0.35
data                18.0ms          6.0ms           3296577         908834          0.33            0.28
refer               16.0ms          4.0ms           3295371         724038          0.25            0.22
member              15.0ms          3.0ms           3294488         602967          0.2             0.18
ability             17.0ms          3.0ms           3296299         542977          0.18            0.16
requires            15.0ms          2.0ms           3296570         491548          0.13            0.15
necessary           21.0ms          3.0ms           3295678         446775          0.14            0.14
applicants          17.0ms          2.0ms           3296465         414048          0.12            0.13
Scholarship         18.0ms          3.0ms           3293298         380699          0.17            0.12
Undergraduate       14.0ms          2.0ms           3296407         327066          0.14            0.10

AVERAGES            17.0ms          3.5ms                                           0.203           0.183


With this data, we can see that the Horspools algorithm averages out to be around 80% faster. This is
due to the preprocessing that is used by the Horspools alogritm and the end to start checking. This allows 
for certain parts of the string to be skipped at every time a charcter does not match, the table that is 
generated is referenced to determine the number of indices to skip over in the string. It can be seen
that in the data acheived via the 10 word test cases, the horspool algoritm is faster with longer words. 
This makes sense as more character are checked less possible match cases are determined via the table.   