
SOURCES = TowerOfHanoi.c
LIBS	= -lncurses
CFLAGS	= -Wall -Wextra
PROGRAM	= TowerOfHanoi

$(PROGRAM): $(SOURCES)
	$(CC) $(CFLAGS) -g $< -o $@ $(LIBS)
