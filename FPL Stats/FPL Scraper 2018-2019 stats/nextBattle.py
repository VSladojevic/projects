def toRoman(num):
    i=num%3;
    num=num-i;

    v=num%5;
    num=num-v;
    
    x=num%10;
    num=num-x;

    l=num%50;
    num=num-l;

    c=num%100;
    num=num-c;

    d=num%500;
    num=num-d;

    m=num%1000;
    num=num-m;