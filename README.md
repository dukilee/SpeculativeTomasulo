# SpeculativeTomasulo
Simulation of speculative tomasulo algorithm in C++ with QT.

@ COMPILE

mkdir build
cd build
cmake ..
make

@ RUN

./speculativeTomasulo

@ASSEMBLY 

This is a list of the commands that the assembly interpreter understands:
(Obs: is case sensitive)

; use ';' to comment
lbl:            ;  this is a label
add  r1, r2, r3 ;  r1.val <- r2.val + r3.val
addi r1, r2, k  ;  r1.val <- r2.val + k
sub  r1, r2, r3 ;  r1.val <- r2.val - r3.val
mult r1, r2, r3 ;  r1.val <- r2.val * r3.val
div  r1, r2, r3 ;  r1.val <- r2.val / r3.val ; integer division
li   r1, k      ;  r1.val <- k
sw   r1, k(r2)  ;  mem[k + r2.val] <- r1.val
lw   r1, k(r2)  ;  r1.val <- mem[k + r2.val]
beq  r1, r2, lbl;  if r1.val is equal to r2.val branchs to lbl
ble  r1, r2, lbl;  if r1.val is less or equal to r2.val branchs to lbl
bne  r1, r2, lbl;  if r1.val is not equal to r2.val branchs to lbl
return          ; returns to the point where it branched
goto lbl        ; jumps to lbl

