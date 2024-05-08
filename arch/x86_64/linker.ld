OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ;
    rodata  PT_LOAD    FLAGS((1 << 2)) ;
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ;
    dynamic PT_DYNAMIC FLAGS((1 << 1) | (1 << 2)) ;
}

SECTIONS
{
    . = 0xffffffff80000000;
	
	__kernel_start = .;

    .text : {
        __text_start = .;
		*(.text .text.*)
		__text_end = .;
	} :text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
		__rodata_start = .;
        *(.rodata .rodata.*)
		__rodata_end = .;
	} :rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
		__data_start = .;
        *(.data .data.*)
		__data_end = .;
    } :data

    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    .bss : {
		__bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
		__bss_end = .;
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
	
	__kernel_end = .;
}
