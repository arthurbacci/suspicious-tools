all: clean

watch: watch/*.c libs/*.o
	$(CC) $^ $(CFLAGS) -Ilibs $(LIBS) -o watch/watch

libs/*.o: libs/*.c
	cd libs;\
	$(CC) -c ../$^ $(CFLAGS) -I. $(LIBS)
	cd ..

clean:
	rm -f libs/*.o
	rm -f watch/watch

