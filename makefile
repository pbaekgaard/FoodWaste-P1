#MAKE file for smartfrapp
smartfrapp: app.c FileReading.c makeRecipes.c
	if [ -e smartfrapp.exe ]; then \
		rm smartfrapp.exe; \
	fi
	gcc -Wall -Werror -ansi -pedantic app.c FileReading.c makeRecipes.c \
		-o smartfrapp.exe
	if [ -e smartfrapp.exe ]; then \
		@cls||clear; \
		printf "SMARTFRAPP APPLICATION COMPILED\n"; \
	fi

clean:
	rm smartfrapp
