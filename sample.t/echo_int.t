        ssp 3
        lda 3
        mst
        cup 0 lread
        sto
        mst
        lda 3
        ind
        cup 1 lwrite
        stp
lwrite: ssp 3
        lod 3
        out
        retp
lread:  ssp 2
        in
        str 0
        retf
