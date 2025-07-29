MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 512K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
	ccm (rwx) : ORIGIN = 0x10000000, LENGTH = 32K
}

SECTIONS {
  .ccmram (NOLOAD) : ALIGN(4) {
    __ccmram_start__ = .;
    *(.ccmram)
    *(.ccmram*)
    . = ALIGN(4);
    __ccmram_end__ = .;
  } > ccm
}


INCLUDE cortex-m-generic.ld