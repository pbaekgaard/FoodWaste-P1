#MAKE file for smartfrapp
.PHONY: smartfrapp clean
smartfrapp: app.c fileReading.c makeRecipes.c
	if [ -e smartfrapp.exe ]; then \
		rm smartfrapp.exe; \
	fi
	gcc -Wall -Werror -ansi -pedantic app.c fileReading.c makeRecipes.c \
		-o smartfrapp.exe
	if [ -e smartfrapp.exe ]; then \
		@cls||clear||cls; \
		printf "SMARTFRAPP APPLICATION COMPILED!\n"; \
		./smartfrapp.exe; \
	fi

clean:
	if [ -e smartfrapp.exe ]; then \
		@cls||clear||cls; \
		rm smartfrapp.exe; \
		printf "SMARTFRAPP REMOVED!\n"; \
	fi
