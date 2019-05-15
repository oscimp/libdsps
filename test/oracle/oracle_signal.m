clear all;

addpath("functions/");

# Pretty FS / FC
fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 2.5 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_signal_pretty_output.bin", sig);

# Ugly FS / FC
fs = 125e6;
fc = 10e6 + 5e3;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 1.25 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_signal_ugly_output.bin", sig);

# Very ugly FS / FC
fs = 79.5e6;
fc = 897.254e4 + 5.98e2;
ts = 1/fs;
N = 2048;
n = 1000 * N;

sig = 0.25 * cos((2.0 * pi * fc * (1/fs)) * [1:n]);
write_binary("oracle_signal_very_ugly_output.bin", sig);

