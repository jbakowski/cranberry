#ifndef	_MM_H
#define	_MM_H

#define TABLE_SHIFT 			9
#define PAGE_SHIFT	 		    12

#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)
#define SECTION_SIZE			(1 << SECTION_SHIFT)
#define PAGE_SIZE   			(1 << PAGE_SHIFT)

#define LOW_MEMORY              (2 * SECTION_SIZE)
#define SECONDARY_STACK_START   (1 << 20)


#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned long n);

#endif

#endif  /*_MM_H */