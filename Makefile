all:
	gcc clip.c -o clip -I/opt/homebrew/include -L/opt/homebrew/lib -lportaudio -lm	
	gcc callback.c -o callback -I/opt/homebrew/include -L/opt/homebrew/lib -lportaudio -lm	
