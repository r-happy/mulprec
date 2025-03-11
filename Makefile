1000:
	gcc mulprec.c checkNewtonNoSqrt.c -O3 -o pi1000 -DKETA=135 -DBASE=125

10000:
	gcc mulprec.c checkNewtonNoSqrt.c -O3 -o pi10000 -DKETA=1160 -DBASE=1150

1000000:
	gcc mulprec.c checkNewtonNoSqrt.c -O3 -o pi1000000 -DBASE=115111 -DKETA=115121

all: 1000 10000 1000000 
