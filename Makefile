libs/%.o: libs/%.c libs/%.h
	$(CC) -c -o $@ $< $(CFLAGS) -I. $(LIBS)

watch.bin: watch/watch.c libs/susargparse.o
	$(CC) -o $@ $^ $(CFLAGS) -Ilibs $(LIBS)

.PHONY: clean

clean:
	rm -f libs/*.o
	rm -f *.bin

