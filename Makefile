# sudo apt-get install g++ binutils qemu-system-i386 grub-pc:i386 xorriso

# Sources and headers
CXX_SRC = $(shell find src/ -name "*.cpp")
S_SRC = $(shell find src/ -name "*.s")
HEADERS = $(shell find include/ -name "*.hpp")

# Compilers/Assemblers/Linkers
AS = i386-elf-as
GCC = i386-elf-gcc
GDB = i386-elf-gdb
LD = i386-elf-ld
NASM = i386-elf-nasm

# Change for Linux
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	AS = as
	GCC = gcc
	GDB = gdb
	LD = ld
	NASM = nasm
endif

# Compiler/Linker flags
GCC_FLAGS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector -Wno-write-strings -std=c++17
AS_FLAGS = --32
LD_FLAGS = -melf_i386

# Linker file
LINKER = src/boot/linker.ld

# All objects
OBJ = $(patsubst src/%.cpp, obj/%.o, $(CXX_SRC)) $(patsubst src/%.s, obj/%.o, $(S_SRC))
# Object directories, mirroring source
OBJ_DIRS = $(subst src, obj, $(shell find src -type d))

# Compile sources to objects
obj/%.o: src/%.cpp $(HEADERS)
	$(MAKE) obj_directories
	$(GCC) $(GCC_FLAGS) -c -o $@ $<

obj/%.o: src/%.s
	$(MAKE) obj_directories
	$(AS) $(AS_FLAGS) -o $@ $<

# Link objects into BIN
dist/panix.bin: $(LINKER) $(OBJ)
	@ mkdir -p dist
	$(LD) $(LD_FLAGS) -T $< -o $@ $(OBJ)

# Create bootable ISO
dist/panix.iso: dist/panix.bin
	@ echo Making iso directory...
	@ mkdir -p iso
	@ mkdir -p iso/boot
	@ mkdir -p iso/boot/grub
	@ cp $< iso/boot/
	@ echo Creating grub.cfg...
	@ echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	@ echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	@ echo ''                                  >> iso/boot/grub/grub.cfg
	@ echo 'menuentry "Panix" {'               >> iso/boot/grub/grub.cfg
	@ echo '  multiboot /boot/panix.bin'       >> iso/boot/grub/grub.cfg
	@ echo '  boot'                            >> iso/boot/grub/grub.cfg
	@ echo '}'                                 >> iso/boot/grub/grub.cfg
	@ echo Creating panix.iso...
	@ grub-mkrescue -o dist/panix.iso iso
	@ echo Cleaning up iso directory
	@ rm -rf iso

# Create object file directories
.PHONY: 
	obj_directories
obj_directories:
	mkdir -p $(OBJ_DIRS)

# Run bootable ISO
run: dist/panix.iso
	qemu-system-i386 -drive format=raw,file=$< 

# Install BIN file to local system
install: dist/panix.bin
	sudo cp $< /boot/panix.bin

dist: dist/panix.bin
	@ echo Building VDI image of Panix...
	@ qemu-img convert -f raw -O vdi dist/panix.bin dist/panix.vdi
	@ echo Done building VDI image of Panix!

	@ echo "\nBuilding VMDK image of Panix..."
	@ qemu-img convert -f raw -O vmdk dist/panix.bin dist/panix.vmdk
	@ echo Done building VMDK image of Panix!

# Open the connection to qemu and load our kernel-object file with symbols
debug: dist/panix.iso
	@ echo Booting from floppy...
	qemu-system-i386 -drive format=raw,file=$< 
	@ echo Setting up GDB with qemu...
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file panix.bin"

# Clear out objects and BIN
clean:
	@ echo Cleaning obj directory...
	@ rm -rf obj
	@ echo Cleaning bin files...
	@ rm -rf dist/*.bin
	@ echo "Done cleaning!"
