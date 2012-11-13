/*
 *pscp sdap.pgc xwang27@linux-lab.cs.stevens.edu:/home/xwang27
 updatefiles in the linux-lab File - sdap.pgc (Need the .pgc extension!) 
 * Desc - This is a simple skeleton program using Embedded SQL to read all
 *        of the rows from the table "sales" and output to the console.
 *        Simple but it contains all of the essential features of ESQL 
 *        discussed in class!
 *
 * Steps to run this program : 
 *  1. Log in to PostgreSQL server (ssh [username]@postgres.cs.stevens.edu).
 *  2. In the program (L48), modify [dbname], [username], [password] to
 *     yours ([dbname] is the same as your [username] by default).
 *  3. Preprocessor - $ ecpg -I /usr/include/postgresql sdap.pgc
 *  4. Compile      - $ gcc -c -I /usr/include/postgresql/ sdap.c
 *  5. Link         - $ gcc -L /usr/include/postgresql/ -lecpg -o sdap sdap.o
 *  6. Execute      - $ ./sdap
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//----------------------------------------------------------------------
// HOST VARIABLES definitions
//----------------------------------------------------------------------
EXEC SQL BEGIN DECLARE SECTION;
struct {
	char	*cust;
	char	*prod;
	short    dd;
	short    mm;
	short	 yy;
	char	*state;
	long	 quant;
}	sale_rec;
EXEC SQL END DECLARE SECTION;
EXEC SQL INCLUDE sqlca;


//----------------------------------------------------------------------
int main(int argc, char* argv[])
//----------------------------------------------------------------------
{
  
   
   //----------------------------------------------------------------------
   // DATABASE CONNECTION
   //----------------------------------------------------------------------
   EXEC SQL CONNECT TO wwang20 USER wwang20 IDENTIFIED BY wwang20;
   EXEC SQL WHENEVER sqlerror sqlprint;


   //----------------------------------------------------------------------
   struct sale{
        char cust[20];
        char prod[20];
		char state1[10];
		char state2[10];
        short maxdd;
        short maxmm;
        short maxyy;
        short mindd;
        short minmm;
        short minyy;
        long max;
        long min;
        long avg;
		long total;
        int count;
        struct sale *next;
    };
            
    struct sale *head;
    struct sale *p_backup;
    struct sale *p;

 
   EXEC SQL DECLARE mycursor CURSOR FOR SELECT * FROM sales;
   EXEC SQL SET TRANSACTION read only;
   // Open cursor
   EXEC SQL OPEN mycursor;
   
   //int i = 0;
   //int n = 0;
   
   int curDataNum=0;
   p = (struct sale*)malloc(sizeof(struct sale));
   head=p;
   // Fetch Data
   /*
	EXEC SQL FETCH FROM mycursor INTO :sale_rec; //fetch the first row

				strcpy(p->cust, sale_rec.cust);
				strcpy(p->prod, sale_rec.prod);
                p->max = sale_rec.quant;
                p->maxdd = sale_rec.dd;
                p->maxmm = sale_rec.mm;
                p->maxyy = sale_rec.yy;
                strcpy(p->state1 , sale_rec.state);
				

                p->min = sale_rec.quant;
                p->mindd = sale_rec.dd;
                p->minmm = sale_rec.mm;
                p->minyy = sale_rec.yy;
				strcpy(p->state2 ,sale_rec.state);
				
				p->total = sale_rec.quant;
				p->count=1;
				curDataNum++;
		p->next = NULL;
		*/
   EXEC SQL FETCH FROM mycursor INTO :sale_rec;
int  i=0;

while (sqlca.sqlcode == 0)
{
		p_backup = p;
		p = head;
		for( i = 0; i< curDataNum; i++)
		{
          if (strcmp(p->cust, sale_rec.cust) == 0 && strcmp(p->prod, sale_rec.prod) == 0)
          {
             if (p->max <= sale_rec.quant)
             {

                p->max = sale_rec.quant;
                p->maxdd = sale_rec.dd;
                p->maxmm = sale_rec.mm;
                p->maxyy = sale_rec.yy;
                strcpy(p->state1 , sale_rec.state);
             }
             if (p->min >= sale_rec.quant)
             {
			
                p->min = sale_rec.quant;
                p->mindd = sale_rec.dd;
                p->minmm = sale_rec.mm;
                p->minyy = sale_rec.yy;
				strcpy(p->state2 ,sale_rec.state);
             }
			p->total += sale_rec.quant;
			p->count++;
			break;
		  }
			p = p->next;
		  
		}
		p = p_backup;
		
		if(i>=curDataNum)
		{
           strcpy(p->cust, sale_rec.cust);
           strcpy(p->prod, sale_rec.prod);
           p->maxyy = sale_rec.yy;
		   p->maxdd = sale_rec.dd;
           p->maxmm = sale_rec.mm;
           p->mindd = sale_rec.dd;
           p->minmm = sale_rec.mm;
           p->minyy = sale_rec.yy;
		
           strcpy(p->state1, sale_rec.state);
           strcpy(p->state2, sale_rec.state);
           p->max = sale_rec.quant;
           p->min = sale_rec.quant;
           p->avg = 0;
		   p->total = sale_rec.quant;
			p->count = 1;		
			curDataNum++;
			p->next = (struct sale *)malloc(sizeof(struct sale));	
			p = p->next;
			p->next = NULL;
		}     
	   EXEC SQL FETCH FROM mycursor INTO :sale_rec;
	   
}
	p->next = NULL;
   // Close cursor
   EXEC SQL CLOSE mycursor;
   
   
   printf("\n");
   //----------------------------------------------------------------------
   // PRINT TITLE
   //----------------------------------------------------------------------   
   printf(" CUST   | PROD    | MAX_Q |    DATE    | STATE | MIN_Q |    DATE    | STATE | AVG_Q\n");
   printf("--------+---------+-------+------------+-------+-------+------------+-------+------\n");
	p = head;
	while((p->next)!=NULL)
	{
       p->avg = p->total / p->count;
	   printf(" %-6s |",p->cust);   // Customer
       printf(" %-7s |",p->prod);   // Product
	   printf(" %5d |",p->max);   // max
 	   printf(" %02d/%02d/%4d |",p->maxdd, p->maxmm, p->maxyy);     // Date
  	   printf(" %-5s |",p->state1);  // State
   	   printf(" %5d |",p->min);   // min
   	   printf(" %02d/%02d/%4d |",p->mindd, p->minmm, p->minyy);     // Date
   	   printf(" %-5s |",p->state2);  // State
   	   printf(" %5d \n",p->avg);  // average
	   p=p->next;
	   	}
  
   return 0;
}


