SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS))
DEPREQ := $(patsubst src/%.c,depreq/%.d,$(SRCS))

INCLUDE := -I deps/raylib/src
CFLAGS := $(INCLUDE)

LFLAGS := -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

DEPS := deps/raylib/src/libraylib.a

.PHONY: clean_all clean_raylib clean

out/sudosolve: $(DEPREQ) $(OBJS) $(DEPS)
	@echo "creating executable"
	mkdir -p out
	gcc $(OBJS) $(LFLAGS) $(DEPS) -o $@

depreq/%.d: src/%.c
	@echo "creating dependency file $@"
	mkdir -p $(dir $@)
	rm -rf $@
	touch $@
	printf "$@ obj/$(patsubst depreq/%,%,$(dir $@))" >> $@
	gcc $(CFLAGS) -MM $< >> $@

-include $(DEPREQ)

obj/%.o: src/%.c depreq/%.d
	@echo "creating object file $@"
	mkdir -p $(dir $@)
	gcc -c $(CFLAGS) $< -o $@ -g

deps/raylib/src/libraylib.a:
	@echo "creating raylib"
	cd deps/raylib/src && \
	make

clean_raylib:
	@echo "cleaning raylib"
	cd deps/raylib && \
	git clean -d -x -f && \
	git reset --hard

clean:
	@echo "cleaning"
	rm -rf depreq obj out

clean_all: clean clean_raylib