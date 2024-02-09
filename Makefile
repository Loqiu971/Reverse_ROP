CFLAGS = -I.
MAXLEVEL = 8
OBJ = my_vm.o vuln.o
PRJ = my_vm

ifndef LEVEL
	LEVEL=1
endif

EXE = $(PRJ)$(LEVEL)

ifeq ($(LEVEL), 1)
	CFLAGS += -fno-stack-protector -z noexecstack -no-pie -Wl,-z,norelro -Wno-format-security -DLEVEL=$(LEVEL)
	GADGET += gadget.o
endif
ifeq ($(LEVEL), 2)
	CFLAGS += -fno-stack-protector -z noexecstack -no-pie -Wl,-z,norelro -Wno-format-security -DLEVEL=$(LEVEL)
	GADGET += gadget.o
endif
ifeq ($(LEVEL), 3)
	CFLAGS += -fstack-protector -z noexecstack -no-pie -Wl,-z,norelro -Wno-format-security -DLEVEL=$(LEVEL)
	GADGET += gadget.o
endif
ifeq ($(LEVEL), 4)
	CFLAGS += -fstack-protector -z noexecstack -Wl,-z,norelro -Wno-format-security -DLEVEL=$(LEVEL)
	GADGET += gadget.o
endif
ifeq ($(LEVEL), 5)
	CFLAGS += -fstack-protector -z noexecstack -Wl,-z,relro,-z,now -Wno-format-security -DLEVEL=$(LEVEL)
	GADGET += gadget.o
endif
ifeq ($(LEVEL), 6)
	CFLAGS += -fstack-protector -z noexecstack -Wl,-z,relro,-z,now -Wno-format-security -DLEVEL=$(LEVEL)
endif
ifeq ($(LEVEL), 7)
	CFLAGS += -fstack-protector -z noexecstack -Wl,-z,relro,-z,now -Wno-format-security -DLEVEL=$(LEVEL)
endif
ifeq ($(LEVEL), 8)
	CFLAGS += -fstack-protector -z noexecstack -Wl,-z,relro,-z,now -Wno-format-security -DLEVEL=$(LEVEL)
endif

ifdef DEBUG
$(info DEBUG defined)
	CFLAGS += -g
endif

ifdef ASAN
$(info ASAN defined)
	CFLAGS += -fsanitize=address
endif


#### RÉGLES nommées
all: ${OBJ}
	$(CC) $(CFLAGS) $(OBJ) $(GADGET) -o $(EXE)
	sudo chown root $(EXE)
	sudo chmod u+xs $(EXE)

all_levels: LVL:=$(shell echo `seq 1 $(MAXLEVEL)`)
all_levels:
	for i in $(LVL); do LEVEL=$$i make -C . clean all; done

clean:
	$(RM) $(OBJ) .*.swp

distclean: LEVEL_FILE:=$(foreach tgt,$(shell seq 1 $(MAXLEVEL)),$(PRJ)$(tgt))
distclean: clean
	$(RM) $(LEVEL_FILE)

#### RÉGLES Génériques
%.o:%.s
	nasm -f elf64 -o $@ $^


.PHONY: all clean distclean all_levels
