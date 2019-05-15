clear all;

addpath("functions/");

fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 10* N;
t = ts * (1:n);

% Generate the signal
sig1 = 2.5 * cos(2 * pi * fc * t + pi/8);
write_binary("oracle_atan2_input1.bin", sig1);
sig2 = 2.5 * cos(2 * pi * fc * t + pi/3);
write_binary("oracle_atan2_input2.bin", sig2);

% Result
at1 = atan2(sig2, sig1);
write_binary("oracle_atan2_output_2_over_1.bin", at1);
at2 = atan2(sig1, sig2);
write_binary("oracle_atan2_output_1_over_2.bin", at2);
