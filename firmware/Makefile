CC      = sdcc
CFLAGS  = --code-loc 0x0000 --xram-loc 0x0000 --std-sdcc11

rom     = crtinfo.rom
romsize = 65536

objs    = crtinfo.rel interrupts.rel video.rel


.PHONY: all
all: $(rom)

$(rom:.rom=.ihx): $(objs)
	$(CC) $(CFLAGS) -o $@ $(objs)

.PHONY: clean
clean:
	rm -f	$(rom) \
		$(rom:.rom=.ihx) \
		$(objs) \
		$(objs:.rel=.d) \
		$(objs:.rel=.asm) \
		$(objs:.rel=.lst) \
		$(objs:.rel=.mem) \
		$(objs:.rel=.map) \
		$(objs:.rel=.lk) \
		$(objs:.rel=.rst) \
		$(objs:.rel=.rel) \
		$(objs:.rel=.sym)

QMUSICA = $(HOME)/mame/qmusica64
.PHONY: run
run: qmusica/2400.ci4
	$(QMUSICA) -rp . qmusica

qmusica/2400.ci4: $(rom)
	install -D -m644 $(rom) $@


.SUFFIXES: .rel .rom .ihx

.c.rel:
	$(CC) $(CFLAGS) -MMD -o $(@:.rel=.d) $<
	$(CC) $(CFLAGS) -c -o $@ $<

.ihx.rom:
	makebin -s $(romsize) $< $@


objdeps := $(objs:.rel=.d)
-include $(objdeps)
