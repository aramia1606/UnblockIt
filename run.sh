gcc -c -std=c90 console.c
gcc -c -std=c90 main.c
gcc -o prog console.o main.o
./prog
