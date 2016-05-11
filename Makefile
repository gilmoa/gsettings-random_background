CC = gcc
CFLAGS = -Wall

random_background : random_background.c
	$(CC) $^ -o $@ $(CFLAGS)

install : random_background
	cp $^ /bin/$^
	chmod a+x /bin/$^

.PHONY : clean

clean :
	@rm -f *~ random_background
