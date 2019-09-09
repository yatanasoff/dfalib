# dfalib

## Description

dfalib - is a framework for fast search of minimal sequences which fit to specific patterns. These patterns are formulated in terms of regular expressions It's optimized in C++ and can be used by chemists in their work.

In dfalib the following algorithms are implemented:
  - create DFA from regular expression.
  - DFA minimization.
  - intersection of two DFAs.
  
## Program installatoin on Windows

https://drive.google.com/open?id=19Rz_kyeIng_fdLflV0QTvVyEa_5s6Q4P



## Strength vector description

Stength vector of sequence has the following format 
```(a, b, c, d)```

where a, b, c, d correspond to a number of GQD, IMT, HRP, TRP patterns found in the sequence.
If particular attribute is equal to -1 it means that it was not specified in search parameters and is out of interest.

In the output presented all sequences (19 in total) which are the smallest and contain at least one triplex. The size is 9 and all of them have only 1 triplex. Extra facts which are proved automatically:

1) There're no sequences of size lesser then 9 which have at least 1 triplex.
2) There're no suquences of size 9 which have more then 1 triplex.
