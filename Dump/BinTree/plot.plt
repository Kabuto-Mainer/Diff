set terminal png
set output 'Dump/BinTree/Graphic/1.png'
set grid
set xlabel "x"
set ylabel "y"
f(x) = 2 + -50.5 *x** 2 
g(x) = -101 * x + 52.5
plot f(x), g(x)
pause -1
