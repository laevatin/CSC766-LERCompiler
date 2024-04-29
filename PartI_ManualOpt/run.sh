#!/bin/bash

# List of executables
EXECUTABLES="bgd ccsd_multisize ccsd_multisize_opt ccsd_onesize ccsd_onesize_opt example1 example1_opt example2 example2_opt fmri fmri_opt fuse fuse_opt pde pde_opt priv2 priv2_opt ssymm ssymm_opt"

# Run each executable sequentially
for exe in $EXECUTABLES; do
    echo "Running $exe..."
    ./${exe}
    echo "Done."
done