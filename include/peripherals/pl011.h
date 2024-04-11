#ifndef	_P_PL011_H
#define	_P_PL011_H

// clock base frequency is 48 MHz (and unchangeable, for now)
#define PL011_BASE          (PBASE+0x201000)
#define PL011_DR            (PL011_BASE)
#define PL011_RSRECR        (PL011_BASE+0x4)
#define PL011_FR            (PL011_BASE+0x18)
#define PL011_ILPR          (PL011_BASE+0x20) // not in use
#define PL011_IBRD          (PL011_BASE+0x24)
#define PL011_FBRD          (PL011_BASE+0x28)
#define PL011_LCRH          (PL011_BASE+0x2C)
#define PL011_CR            (PL011_BASE+0x30)
#define PL011_IFLS          (PL011_BASE+0x34)
#define PL011_IMSC          (PL011_BASE+0x38)
#define PL011_RIS           (PL011_BASE+0x3C)
#define PL011_MIS           (PL011_BASE+0x40)
#define PL011_ICR           (PL011_BASE+0x44)
#define PL011_DMACR         (PL011_BASE+0x48)
#define PL011_ITCR          (PL011_BASE+0x80)
#define PL011_ITIP          (PL011_BASE+0x84)
#define PL011_ITOP          (PL011_BASE+0x88)
#define PL011_TDR           (PL011_BASE+0x8C)

#endif  /*_P_PL011_H */