.section .rodata

.global bigraph_cur;
.type   bigraph_cur,  @object;

.global bigraph_cur_size;
.type   bigraph_cur_size,  @object;

.balign 32
bigraph_cur:        .incbin "bigr.cur"
                    .byte   0
bigraph_cur_end:

.balign 4, 0
bigraph_cur_size:   .int    bigraph_cur_end - bigraph_cur
