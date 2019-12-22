.section .rodata

.global trigraph_AVv1;
.type   trigraph_AVv1,  @object;

.global trigraph_AVv1_size;
.type   trigraph_AVv1_size,  @object;

.balign 32
trigraph_AVv1:      .incbin "trigr.AVv1"
                    .byte   0
trigraph_AVv1_end:

.balign 4, 0
trigraph_AVv1_size: .int    trigraph_AVv1_end - trigraph_AVv1
