set terminal png
set output 'Dump/BinTree/Graphic/2.png'
set grid
set xlabel "x"
set ylabel "y"
set xrange [-1:1]
f(x) = 1 + (-0.5) *x** 2 + 0.0416667 *x** 4 + (-0.00138889) *x** 6 + 2.48016e-05 *x** 8 
g(x) = 0 * x + 1
plot f(x), g(x)
