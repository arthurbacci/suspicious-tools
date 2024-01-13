all: watch.bin timeout.bin errno.bin

libs/%.o: libs/%.c libs/%.h
	$(CC) -c -o $@ $< $(CFLAGS) -I. $(LIBS)

watch.bin: watch/watch.c libs/susargparse.o
	$(CC) -o $@ $^ $(CFLAGS) -Ilibs $(LIBS)

timeout.bin: timeout/timeout.c libs/susargparse.o
	$(CC) -o $@ $^ $(CFLAGS) -Ilibs $(LIBS)


errno/errnos_draft.def: errno/gen_errnos_draft.sh errno/include_errno.c
	sh -c 'cd errno && ./gen_errnos_draft.sh'
errno/errnos.def: errno/gen_errnos.c errno/errnos_draft.def
	$(CC) -o errno/gen_errnos.bin $< $(CFLAGS)
	sh -c 'cd errno && ./gen_errnos.bin'
	rm -f errno/gen_errnos.bin
	echo 'Please ensure the following outputs are equal:'
	wc -l errno/errnos_draft.def
	wc -l errno/errnos.def
errno.bin: errno/errno.c errno/errnos.def
	$(CC) -o $@ $< $(CFLAGS) -Ilibs $(LIBS)


.PHONY: clean

clean:
	rm -f libs/*.o
	rm -f *.bin
	rm -f errno/errnos_draft.def
	rm -f errno/errnos.def


