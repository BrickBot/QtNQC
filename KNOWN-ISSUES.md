1. Window geometry is not retained by Qt immediately after the geometry
   is set according to the contents of the qtnqc.conf file.
   This seems to be a Solaris problem.
2. Double-clicking on a loop structure will result in a segmentation fault.
   Solution: double-click is currently disabled.
