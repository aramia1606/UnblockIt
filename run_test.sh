gcc -c -std=c90 console.c
gcc -c -std=c90 main1.c
gcc -o prog console.o main1.o
./prog
