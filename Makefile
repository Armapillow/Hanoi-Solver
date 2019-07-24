
SOURCES = TowerOfHanoi.c
LIBS	= -lncurses
CFLAGS	= -Wall
PROGRAM	= TowerOfHanoi

$(PROGRAM): $(SOURCES)
	$(CC) $(CFLAGS) -g $< -o $@ $(LIBS)
