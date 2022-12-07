extern unsigned char buf[4];
bigtolittle(unsigned int number)
{
    buf[0]=number & 0x000000ff;
    number=number>>8;
    buf[1]=number & 0x000000ff;
    number=number>>8;
    buf[2]=number & 0x000000ff;
    number=number>>8;
    buf[3]=number & 0x000000ff;
}