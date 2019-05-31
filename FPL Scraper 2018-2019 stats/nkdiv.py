import math

def primeFactors(n):
    piles=[]
    ind=0
    if(n%2==0):
        ind+=1
        piles.append(0)

    while(n%2==0):
        piles[ind-1]=piles[ind-1]+1
        n=n/2

    for i in range(3,1+int(math.sqrt(n)),2):
        if(n%i==0):
            ind+=1
            piles.append(0)
        while(n%i==0):
            piles[ind-1]=piles[ind-1]+1
            n=n/i
    
    if(n>2):
        ind+=1
        piles.append(0)
        piles[ind-1]=piles[ind-1]+1

    return piles

def gameWinner(heaps,k):
    if(len(heaps)<=k):
        return 0
    
    max=0
    for i in range(len(heaps)):
        if(heaps[i]>max):
            max=heaps[i]
    
    sums=[]
    cur=0
    ind=0
    t=int(0)
    while(max>0):
        cur=0
        t=1
        for i in range(ind):
            t*=2
        for i in range(len(heaps)):
            cur += (heaps[i] & t) / t
        cur = cur % (k+1)
        sums.append(cur)
        max/=2
        ind+=1

    for i in range(len(sums)):
        if(sums[i]!=0):
            return 0
            
    return 1

rounds=int(input())
numbers=[]
k=[]
for i in range(rounds):
    tmpn, tmpk = map(int, input().split())
    numbers.append(tmpn)
    k.append(tmpk)

allPiles=[]
for i in range(rounds):
    allPiles.append(primeFactors(numbers[i]))

results=[]
player=["Mona","Alaa"]
first=0
count=[0,0]
winner=0
for i in range(rounds):
    winner=(first+gameWinner(allPiles[i],k[i])) % 2
    results.append(player[winner])
    first=winner
    count[winner]=count[winner]+1

for i in range(rounds):
    print(results[i])

print("")
if(count[1]>count[0]):
    print(player[1])
else:
    print(player[0])