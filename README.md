# LER Loop Redundancy Optimizer

## Part I - Manual Optimization

I manually optimized all of the files in PartI_ManualOpt except `bgd.cpp` in this part. I cannot figure out the optimization opportunity in `bgd.cpp` so I left it as it is. The code could be compiled with the following command:

```bash
make -j
```

By changing the CFLAGS in the Makefile, one could apply different optimization levels. 

| Task              | -O0 Time(s) | -O0 w/Opt Time(s) | -O3 Time(s) | -O3 w/Opt Time(s) |
|-------------------|-------------|-------------------|-------------|-------------------|
| bgd               | 3.846742     | 3.846742       | 0.529367     | 0.529367       |
| ccsd_multisize    | 52.711799    | 0.058377       | 4.668965     | 0.000924       |
| ccsd_onesize      | 193.096414   | 14.385481      | 9.692131     | 1.039084       |
| example1          | 0.332301     | 0.007836       | 0.015520     | 0.000615       |
| example2          | 0.187272     | 0.000082       | 0.050787     | 0.000062       |
| fmri              | 0.098553     | 0.000081       | 0.024657     | 0.000036       |
| fuse              | 0.418496     | 0.000043       | 0.100847     | 0.000020       |
| pde               | 1.190869     | 0.416554       | 0.400554     | 0.123040       |
| priv2             | 2.019114     | 0.011125       | 0.117982     | 0.005746       |
| ssymm             | 0.234815     | 0.002363       | 0.071617     | 0.000209       |

The original and optimized LER formula is in the formula.pdf. 

## Part II - Compiler Optimization

To compile the code, one could use the following command:

```bash
javac Glory*.java
javac Glory.java DirectiveListener.java LEROptimizeListener.java
```

I implemented the LER optimizer in the `LEROptimizeListener.java` file as a listener. I implemented one optimization that removes the constant redundant loops that no array access is dependent on. It utilizes the relLoops sets described in the paper. Now it works for the ccsd_multisize case. For an input:

```
Γa∫0,V∫Γb∫0,V∫Γc∫0,V∫Γi∫0,O∫Γj∫0,O∫Γk∫0,O∫Γe∫0,V∫Γm∫0,1000∫T2[c,e,i,j]*O2[a,b,e,k]=X[a,b,c,i,j,k]
```

It will output the optimized formula:

```
1000=tmp0
Γa∫0,V∫Γb∫0,V∫Γc∫0,V∫Γi∫0,O∫Γj∫0,O∫Γk∫0,O∫Γe∫0,V∫tmp0*T2[c,e,i,j]*O2[a,b,e,k]=X[a,b,c,i,j,k]
```