/*
 * File: stm32l011f4p6_flash.ld
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

ENTRY(Reset_Handler)

_Min_Heap_Size = 0x000;
_Min_Stack_Size = 0x100;

MEMORY
{
	FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 16K
	RAM	(xrw)      : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{
	_bflash = ORIGIN(FLASH);
	_bram =  ORIGIN(RAM);

	_eflash = ORIGIN(FLASH) + LENGTH(FLASH);
	_eram =  ORIGIN(RAM) + LENGTH(RAM);
	
	_estack = _eram - 1;

	.isr_vector (ORIGIN(FLASH)):
	{
    	. = ALIGN(4);
    	_isr_vector_begin_address =  .;
    	_btext = .;
    
    	KEEP(*(.isr_vector))
    
		. = ALIGN(4);
	} >FLASH

	.text :
	{
		. = ALIGN(4);

    	*(.text.Main_Init)
		*(.text)
		*(.text*)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame)

		. = ALIGN(4);
		_etext = .;
	} >FLASH

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
		_erodata = .;
	} >FLASH

	_sidata = LOADADDR(.data);

	.data : 
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data*)

		. = ALIGN(4);
		_edata = .;
	} >RAM AT> FLASH

	_text_size = _erodata - _btext;
  
	. = ALIGN(4);
	.bss :
	{
	    _sbss = .;
    	__bss_start__ = _sbss;
    	*(.bss)
    	*(.bss*)
    	*(COMMON)

    	. = ALIGN(4);
    	_ebss = .;
    	__bss_end__ = _ebss;
	} >RAM

	._user_heap_stack :
	{
		. = ALIGN(8);
		PROVIDE ( end = . );
		PROVIDE ( _end = . );
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
	} >RAM

	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	.ARM.attributes 0 : { *(.ARM.attributes) }
}
