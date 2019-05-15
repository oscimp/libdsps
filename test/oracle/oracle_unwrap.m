clear all;

addpath("functions/");

# The 1 block version
phi = mod([-1024:0.1:1024-0.1], 2*pi);

write_binary("oracle_unwrap_input.bin", phi);
write_binary("oracle_unwrap_output_1_bloc.bin", unwrap(phi));

# The 10 block version
un = [];
for it = 0:9
	un = [un unwrap(phi(it*2048+1:(it+1)*2048))];
endfor

write_binary("oracle_unwrap_output_10_bloc.bin", un);
