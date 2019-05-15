clear all;

addpath("functions/")

fc = 10e6;
N = 2048;
M = 10;

% To double test
real_vec = unifrnd(-50, 50, N * M, 1);
real_abs = abs(real_vec);

write_binary("oracle_abs_double_input.bin", real_vec);
write_binary("oracle_abs_double_output.bin", real_abs);

% To complex test
complex_vec = unifrnd(-50, 50, N * M, 1) + unifrnd(-50, 50, N * M, 1)*i;
complex_abs = abs(complex_vec);

write_complex_binary("oracle_abs_complex_input.bin", complex_vec);
write_binary("oracle_abs_complex_output.bin", complex_abs);

