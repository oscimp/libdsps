clear all;

addpath("functions/");

# Pretty FS/FC
fs = 250e6;
fc = 10e6;
ts = 1/fs;
N = 2048;
n = 10 * N;
vfsr = 5;

xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin")';
phi = 2 * pi * fc * xtt;
sig = (0.5 * vfsr) * cos((2.0 * pi * fc * (1/fs)) * [1:n] + phi);

write_binary("oracle_adc_pretty_input.bin", phi);
write_binary("oracle_adc_pretty_output.bin", sig);

# Ugly FS/FC
fs = 125e6;
fc = 10e6 + 5e3;
ts = 1/fs;
N = 2048;
n = 10 * N;
vfsr = 2.5;

xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin")';
phi = 2 * pi * fc * xtt;
sig = (0.5 * vfsr) * cos((2.0 * pi * fc * (1/fs)) * [1:n] + phi);

write_binary("oracle_adc_ugly_input.bin", phi);
write_binary("oracle_adc_ugly_output.bin", sig);

# Very ugly FS/FC
fs = 98.367e6;
fc = 15.369e5 + 37.6e2;
ts = 1/fs;
N = 2048;
n = 10 * N;
vfsr = 1.269;

xtt = load_binary("oracle--160-sr250-fc10-20480.xtt.bin")';
phi = 2 * pi * fc * xtt;
sig = (0.5 * vfsr) * cos((2.0 * pi * fc * (1/fs)) * [1:n] + phi);

write_binary("oracle_adc_very_ugly_input.bin", phi);
write_binary("oracle_adc_very_ugly_output.bin", sig);

