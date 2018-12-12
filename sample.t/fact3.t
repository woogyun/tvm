        ssp 6
        lda 4
        ldc 4
        sto
        lda 5
        mst
        lda 4
        ind
        ldc 1
        sub
        cup 1 lf
        sto
        mst
        lda 5
        ind
        cup 1 lwrite
        stp
lf:     ssp 7
        lda 3
        ind
        ldc 1
        equ
        fjp l1
        ldc 1
        str 0
        retf
        ujp l2
l1:     lda 3
        ind
        mst
        lda 3
        ind
        ldc 1
        sub
        cup 1 lf
        mul
        str 0
        retf
l2:     lda 4
        lda 7
        ind
        sto
        retf
lwrite: ssp 3
        lod 3
        out
        retp
lread:  ssp 2
        in
        str 0
        retf
