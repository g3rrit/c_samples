#include <stddef.h>

#ifndef __USER_LABEL_PREFIX__
#define INCBIN__PFX ""
#else
#define INCBIN__PFX INCBIN__PFX__0((__USER_LABEL_PREFIX__))
#define INCBIN__PFX__0(t_args)INCBIN__PFX__1(INCBIN__PFX__2)t_args
#define INCBIN__PFX__1(x)x
#define INCBIN__PFX__2(x...)#x
#endif

struct Image
{
    __const__ void *data;
    size_t size;
};

#define INCBIN(filename) \
    INCBIN__0((__COUNTER__,filename))
#define INCBIN__0(t_args)INCBIN__1(INCBIN__2)t_args
#define INCBIN__1(x)x
#define INCBIN__2(i, filename)\
    (__extension__({\
        __label__ INCBIN__dummy; \
        extern __const__ char INCBIN__img_ ## i[]; \
        extern __const__ char INCBIN__size_ ## i[]; \
        struct Image INCBIN__ret; \
    INCBIN__dummy: \
        __asm__ __volatile__(\
            "      .section .rodata\n" \
            INCBIN__PFX "INCBIN__img_" #i ":\n" \
            "      .incbin \"" filename "\"\n" \
            INCBIN__PFX "INCBIN__size_" #i " = . - " INCBIN__PFX "INCBIN__img_" #i "\n" \
            "      .byte 0\n" \
            "      .text\n" \
            "      .if 0\n" \
            "      .error \"%0\"\n" \
            "      .endif\n" \
            :: "X"(&&INCBIN__dummy)); \
        INCBIN__ret.data = INCBIN__img_ ## i; \
        INCBIN__ret.size = (size_t)INCBIN__size_ ## i; \
        INCBIN__ret; \
    }))

// ALTERNATIVE:
//
// #include <stdio.h>
// extern const char text[];
// __attribute__((__used__, __unused__))
// static void dummy(void) {
//     static __volatile__ int foo;
//     __asm__ __volatile__(
//         ".if 0\n"
//         ".print \"%0\"\n"
//         ".endif\n"
//         ".section .rodata\n"
//         "text:\n"
//         "    .incbin \"/etc/passwd\"\n"
//         "    .byte 0\n"
//         ".text"
//         : "=X"(foo));
// }
// int main(void) {
//     puts(text);
//     return 0;
// }
//
// EXAMPLE:
//
// #include <stdio.h>
// int main(void) {
//     puts(INCBIN("/etc/password").data);
//     return 0;
// }
