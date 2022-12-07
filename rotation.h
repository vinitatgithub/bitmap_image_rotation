extern FILE *fp1,*fp2,*fp3;
extern unsigned char filename[100];
extern unsigned char buffer[1000],rowdata[5000],buf[4];
extern unsigned int  colorintensity,paletteentrycount,i,j,bytestoberead,noofshades;
extern unsigned int  rotationside,ImageSize,count,ImageWidth,ImageHeight,col;
extern unsigned int  FirstTime,biWid,biHei,vertnum,horinum,paddingbytes,acthoriSize;
extern unsigned int  bytesperpel;

rotater90l270()
{
    while(ImageSize>0)
    {
        for(i=1;i<=ImageHeight;i++)
        {
            fseek(fp1,((ImageWidth-1)*bytesperpel),SEEK_CUR);
            fread(buffer,1,bytesperpel,fp1);
            fwrite(buffer,1,bytesperpel,fp2);
            vertnum=vertnum-bytesperpel;
            ImageSize=ImageSize-bytesperpel;

            for(j=0;j<acthoriSize-(ImageWidth*bytesperpel);j++)
            {
                fread(buffer,1,1,fp1);
            }

            if(ImageSize-paddingbytes==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }

        while(vertnum>0)
        {
            buffer[0]=0;
            fwrite(buffer,1,1,fp2);
            vertnum--;
        }

        fseek(fp1,-(bytesperpel*(acthoriSize/bytesperpel)*biHei),SEEK_CUR);
        ImageWidth--;
        vertnum=(int)(4*ceil((double)(biHei*bytesperpel)/4));
    }
    return 0;
}

rotatel90r270()
{
    col=0;
    fseek(fp1,(bytesperpel*(acthoriSize/bytesperpel)*biHei),SEEK_CUR);
    while(ImageSize>0)
    {
        FirstTime=1;
        for(i=1;i<=ImageHeight;i++)
        {
            if(FirstTime)
            {
                fseek(fp1,-((ImageWidth*bytesperpel)+(acthoriSize-(biWid*bytesperpel))),
                    SEEK_CUR);
                FirstTime=0;
            }
            else
            {
                fseek(fp1,-(acthoriSize),SEEK_CUR);
            }

            fread(buffer,1,bytesperpel,fp1);            
            fwrite(buffer,1,bytesperpel,fp2);
            vertnum=vertnum-bytesperpel;
            fseek(fp1,-bytesperpel,SEEK_CUR);
            ImageSize=ImageSize-bytesperpel;

            if(ImageSize-paddingbytes==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }

        while(vertnum>0)
        {
            buffer[0]=0;
            fwrite(buffer,1,1,fp2);
            vertnum--;
        }

        fseek(fp1,(bytesperpel*((biWid*(biHei-1))+(biWid-col))+paddingbytes),SEEK_CUR);
        vertnum=(int)(4*ceil((double)(biHei*bytesperpel)/4));
        col++;
        ImageWidth--;
    }
    return 0;
}

rotater180l180()
{
    fseek(fp1,(bytesperpel*(acthoriSize/bytesperpel)*biHei),SEEK_CUR);
    while(ImageSize>0)
    {
        for(i=1;i<=ImageHeight;i++)
        {
            fseek(fp1,-(acthoriSize),SEEK_CUR);
            fread(buffer,1,acthoriSize,fp1);
            for(i=0;i<biWid;i++)
            {
                for(j=0;j<bytesperpel;j++)
                {
                    buf[j]=buffer[((biWid-i)*bytesperpel)-(bytesperpel-j)];
                }                
                fwrite(buf,1,bytesperpel,fp2);
                horinum=horinum-bytesperpel;
                ImageSize=ImageSize-bytesperpel;
            }

            while(horinum>0)
            {
                buffer[0]=0;
                fwrite(buffer,1,1,fp2);
                horinum--;
            }
            fseek(fp1,-(acthoriSize),SEEK_CUR);

            if(ImageSize-paddingbytes==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
            horinum=(int)(4*ceil((double)(biWid*bytesperpel)/4));
        }
    }
    return 0;
}

rotater90andflip()
{
    ImageWidth=1;
    while(ImageSize>0)
    {
        for(i=1;i<=ImageHeight;i++)
        {
            fseek(fp1,((ImageWidth-1)*bytesperpel),SEEK_CUR);
            fread(buffer,1,bytesperpel,fp1);            
            fwrite(buffer,1,bytesperpel,fp2);
            vertnum=vertnum-bytesperpel;
            ImageSize=ImageSize-bytesperpel;

            for(j=0;j<acthoriSize-(ImageWidth*bytesperpel);j++)
            {
                fread(buffer,1,1,fp1);
            }

            if(ImageSize-paddingbytes==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }
            
        while(vertnum>0)
        {
            buffer[0]=0;
            fwrite(buffer,1,1,fp2);
            vertnum--;
        }
            
        fseek(fp1,-(bytesperpel*(acthoriSize/bytesperpel)*biHei),SEEK_CUR);
        vertnum=(int)(4*ceil((double)(biHei*bytesperpel)/4));
        ImageWidth++;
    }
    return 0;
}

flipwrthoriaxis()
{
    fseek(fp1,(bytesperpel*(acthoriSize/bytesperpel)*biHei),SEEK_CUR);
    while(ImageSize>0)
    {
        for(i=1;i<=ImageHeight;i++)
        {
            fseek(fp1,-(acthoriSize),SEEK_CUR);
            fread(buffer,1,acthoriSize,fp1);
            fwrite(buffer,1,acthoriSize,fp2);
            ImageSize=ImageSize-acthoriSize;

            fseek(fp1,-(acthoriSize),SEEK_CUR);

            if(ImageSize==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }
        ImageWidth--;
    }
    return 0;
}

flipwrtvertiaxis()
{
    while(ImageSize>0)
    { 
        for(i=1;i<=ImageHeight;i++)
        {
            fread(buffer,1,acthoriSize,fp1);

            for(i=0;i<biWid;i++)
            {
                for(j=0;j<bytesperpel;j++)
                {
                    buf[j]=buffer[((biWid-i)*bytesperpel)-(bytesperpel-j)];
                }
                fwrite(buf,1,bytesperpel,fp2);
                horinum=horinum-bytesperpel;
                ImageSize=ImageSize-bytesperpel;
            }

            while(horinum>0)
            {
                buffer[0]=0;
                fwrite(buffer,1,1,fp2);
                horinum--;
            }

            if(ImageSize-paddingbytes==0)
            {
                printf("End of File found...\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
            horinum=(int)(4*ceil((double)(biWid*bytesperpel)/4));
        }
    }
    return 0;
}
