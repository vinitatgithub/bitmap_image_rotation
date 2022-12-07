littletobig(unsigned char temp1,unsigned char temp2, unsigned char temp3, 
            unsigned char temp4, unsigned int numofbytes)
{
    unsigned int number;
    if(numofbytes==2)
    {
        number=temp2;
        number=number<<8;
        number=number | temp1;
    }
    else if (numofbytes==4)
    {
        number=temp4;
        number=number<<8;
        number=number | temp3;
        number=number<<8;
        number=number | temp2;
        number=number<<8;
        number=number | temp1;
    }
    return (number);
}