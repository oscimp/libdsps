clear all;

addpath("functions/");

# Pretty FS/FC
fs = 250e6;
f1 = 10e6;
f2 = 5e6;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * [1:n];

s1 = 2.5 * cos(2 * pi * f1 * t + pi / 8);
s2 = 2.5 * cos(2 * pi * f2 * t + pi / 2);

sig = s1 + s2;

# Without offset
[ibf, qbf] = demodulation(sig, f1, 0, t);
write_binary("oracle_demodulation_pretty_input.bin", sig);
write_binary("oracle_demodulation_pretty_wo_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_pretty_wo_offset_qbf.bin", qbf);

# With offset
[ibf, qbf] = demodulation(sig, f1, pi/7, t);
write_binary("oracle_demodulation_pretty_input.bin", sig);
write_binary("oracle_demodulation_pretty_wi_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_pretty_wi_offset_qbf.bin", qbf);

# Ugly FS/FC
fs = 125e6;
f1 = 10e6 + 5e3;
f2 = 5e6 + 5e3;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * [1:n];

s1 = 1.25 * cos(2 * pi * f1 * t + pi / 8);
s2 = 1.25 * cos(2 * pi * f2 * t + pi / 2);

sig = s1 + s2;

# Without offset
[ibf, qbf] = demodulation(sig, f1, 0, t);
write_binary("oracle_demodulation_ugly_input.bin", sig);
write_binary("oracle_demodulation_ugly_wo_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_ugly_wo_offset_qbf.bin", qbf);

# With offset
[ibf, qbf] = demodulation(sig, f1, pi/9, t);
write_binary("oracle_demodulation_ugly_input.bin", sig);
write_binary("oracle_demodulation_ugly_wi_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_ugly_wi_offset_qbf.bin", qbf);

# Very ugly FS/FC
fs = 69.35e6;
f1 = 63.57e4 + 12.9e2;
f2 = (63.57e4 * 0.5) + 12.9e2;
ts = 1/fs;
N = 2048;
n = 10 * N;
t = ts * [1:n];

s1 = 1.25 * cos(2 * pi * f1 * t + pi / 8);
s2 = 1.25 * cos(2 * pi * f2 * t + pi / 2);

sig = s1 + s2;

# Without offset
[ibf, qbf] = demodulation(sig, f1, 0, t);
write_binary("oracle_demodulation_very_ugly_input.bin", sig);
write_binary("oracle_demodulation_very_ugly_wo_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_very_ugly_wo_offset_qbf.bin", qbf);

# With offset
[ibf, qbf] = demodulation(sig, f1, pi/12, t);
write_binary("oracle_demodulation_very_ugly_input.bin", sig);
write_binary("oracle_demodulation_very_ugly_wi_offset_ibf.bin", ibf);
write_binary("oracle_demodulation_very_ugly_wi_offset_qbf.bin", qbf);


