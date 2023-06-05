.syntax unified

// quarterround(uint32 *a, uint32 *b, uint32 *c, uint32 *d)
.global quarterround
.type quarterround,%function
.align 2
quarterround:
    push {r4-r11, r14}

    pop {r4-r11, pc}