% Lecture dans un fichier binaire de nombre complex
%
% file_name 		Nom du fichier
%
% cpx			Complex contenu dans le fichier

function cpx = load_complex_binary(file_name)
	f = fopen(file_name, "r");
	if (f == -1)
		error("The file '%s' is not found", file_name);
	endif

	l = fread(f, [2, Inf], "double");
	l = l';
	cpx = l(:,1) + l(:,2) * i;
	fclose(f);
endfunction

