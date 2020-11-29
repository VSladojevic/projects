

def ranac(kap, zap, vr):
    '''
    klasican problem ranca, sa beskonacnim zalihama predmeta
    kap - kapacitet ranca
    zap - niz zapremina predmeta
    vr - niz vrednosti predmeta
    '''
    a=[0]*(kap+1)
    p=[-1]*(kap+1)
    for c in xrange(1,kap+1):
        for i, z in enumerate(zap):
            if(z<=c):
                if(a[c]<a[c-z]+vr[i]):
                    a[c]=a[c-z]+vr[i]
                    p[c]=i
    print a


def ranacKonacno(kap, zap, vr):
    n=len(zap)
    h, w = kap+1, n+1
    a = [[0 for x in range(w)] for y in range(h)]
    for c in xrange(0,kap+1):
        a[c][0]=0
    for i in xrange(1, n+1):
        for c in xrange(0,kap+1):
            a[c][i]=a[c][i-1]
            if(zap[i-1]<=c):
                if(a[c][i]<a[c-zap[i-1]][i-1]+vr[i-1]):
                    a[c][i]=a[c-zap[i-1]][i-1]+vr[i-1]
    print kap,zap,vr
