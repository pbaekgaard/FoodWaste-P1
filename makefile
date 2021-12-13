#MAKE file for smartfrapp

smartfrapp: app.c FileReading.c makeRecipes.c
	gcc -Wall -ansi -pedantic app.c FileReading.c makeRecipes.c \
		-o smartfrapp.exe

clean:
	rm smartfrapp
