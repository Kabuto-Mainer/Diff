set terminal png
set output 'Dump/BinTree/Graphic/1.png'
set grid
set xlabel "x"
set ylabel "y"
set xrange [-2:2]
f(x) =sin(x)
f0(x) = (0) 
f1(x) =x
f2(x) =x
f3(x) =x+ (-0.166667) *x** 3 
f4(x) =x+ (-0.166667) *x** 3 
f5(x) =x+ (-0.166667) *x** 3 + 0.00833333 *x** 5 
plot \
    f(x) lt rgb "#000000" title "Сама функция",\
     f0(x) lt rgb "#e5e90e" title "Тейлор до 0",\
     f1(x) lt rgb "#11ee32" title "Тейлор до 1",\
     f2(x) lt rgb "#0dd6f6" title "Тейлор до 2",\
     f3(x) lt rgb "#1111f1" title "Тейлор до 3",\
     f4(x) lt rgb "#ec0dd2" title "Тейлор до 4",\
     f5(x) lt rgb "#f40c27" title "Тейлор до 5"
