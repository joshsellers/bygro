#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

enum class INSTRUCTION {
    NIL       = 0x00,
    LIT       = 0x01,
    STR       = 0x02,
    IF        = 0x03,
    WHILE     = 0x04,
    BRK       = 0x05,
    CONT      = 0x06,
    ASSIGN    = 0x07,
    READ      = 0x08,
    ENDIF     = 0x09,
    ADD       = 0x0A,
    SUB       = 0x0B,
    MUL       = 0x0C,
    DIV       = 0x0D,
    GRT       = 0x0E,
    LSS       = 0x0F,
    GRE       = 0x10,
    LSE       = 0x11,
    EQL       = 0x12,
    AND       = 0x13,
    OR        = 0x14,
    SGN       = 0x15,
    PRNT      = 0x16,
    STWH      = 0x17,
    NEQ       = 0x18,
    READSTR   = 0x19,
    ASSIGNSTR = 0x1A,
    PRNTLN    = 0x1B,
    NUMPRNT   = 0x1C,
    NUMPRNTLN = 0x1D,
    MOD       = 0x1E
};

#endif