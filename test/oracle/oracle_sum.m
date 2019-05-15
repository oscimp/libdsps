clear all;

addpath("functions/");

fs = 250e6;
f1 = 10e6;
f2 = 5e6;
f3 = 1e6;
f4 = 50e6;
f5 = 100e6;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * (1:n);

s1 = 2.5 * cos(2 * pi * f1 * t);
s2 = 2.5 * cos(2 * pi * f2 * t);
s3 = 2.5 * cos(2 * pi * f3 * t);
s4 = 2.5 * cos(2 * pi * f4 * t);
s5 = 2.5 * cos(2 * pi * f5 * t);

sum1 = s1 + s2;
sum2 = s1 + s2 + s3;
sum3 = s1 + s2 + s3 + s4 + s5;

write_binary("oracle_sum_input1.bin", s1);
write_binary("oracle_sum_input2.bin", s2);
write_binary("oracle_sum_input3.bin", s3);
write_binary("oracle_sum_input4.bin", s4);
write_binary("oracle_sum_input5.bin", s5);
write_binary("oracle_sum_2ch.bin", sum1);
write_binary("oracle_sum_3ch.bin", sum2);
write_binary("oracle_sum_5ch.bin", sum3);

