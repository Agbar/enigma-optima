.section .rodata

.global unigraph_AVv1;
.type   unigraph_AVv1,  @object;

.global unigraph_AVv1_size;
.type   unigraph_AVv1_size,  @object;

.balign 32
unigraph_AVv1:      .incbin "unigr.AVv1"
                    .byte   0
unigraph_AVv1_end:

.balign 4, 0
unigraph_AVv1_size: .int    unigraph_AVv1_end - unigraph_AVv1
