#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

typedef uint32_t dict_t;

// In general on every processor after Pentium Pro MOVZX is as fast as MOV.
// At most one cycle more when load is from memory on AMDs.
typedef uint8_t text_t;


#endif // CONFIG_TYPES_H
