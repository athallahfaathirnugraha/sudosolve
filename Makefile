INCLUDE := -I deps/raylib/src

ifeq ($(RELEASE),1)
CFLAGS := $(INCLUDE)
TAR_DIR := release
else
CFLAGS := $(INCLUDE) -DDEBUG -g 
TAR_DIR := debug
endif

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,obj/$(TAR_DIR)/%.o,$(SRCS))
DEPREQ := $(patsubst src/%.c,depreq/$(TAR_DIR)/%.d,$(SRCS))

LFLAGS := -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

DEPS := deps/raylib/src/libraylib.a

.PHONY: clean_all clean_raylib clean

out/$(TAR_DIR)/sudosolve: $(DEPREQ) $(OBJS) $(DEPS)
	@echo "creating executable"
	mkdir -p $(dir $@)
	gcc $(OBJS) $(LFLAGS) $(DEPS) -o $@

depreq/$(TAR_DIR)/%.d: src/%.c
	@echo "creating dependency file $@"
	mkdir -p $(dir $@)
	rm -rf $@
	touch $@
	printf "$@ obj/$(patsubst depreq/%,%,$(dir $@))" >> $@
	gcc $(CFLAGS) -MM $< >> $@

-include $(DEPREQ)

obj/$(TAR_DIR)/%.o: src/%.c depreq/$(TAR_DIR)/%.d
	@echo "creating object file $@"
	mkdir -p $(dir $@)
	gcc -c $(CFLAGS) $< -o $@

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