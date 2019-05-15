% Démodulation d'un signal donné avec un décalage de phase donné
%
% signal		Signal à démoduler
% fc			Fréquence de la porteuse
% phi_offset	Valeur du décalage de phase
% t             Vecteur temps
%
% signal        Signal produit

function [Ibf, Qbf] = demodulation (signal, fc, phi_offset, t)
	sine = sin(2*pi*fc*t+phi_offset);
	cosine = cos(2*pi*fc*t+phi_offset);

	Ibf = signal .* cosine;
	Qbf = signal .* sine;
endfunction

