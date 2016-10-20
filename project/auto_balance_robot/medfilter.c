/* medfilt_new.c    Median filter in C*/

#include <stdio.h>
#include <limits.h>
#include "medfilter.h"
#define NWIDTH 10 /* Size of the data buffer; length of the sequence. */
#define STOPPER INT_MIN /* Smaller than any datum */

int medfilter(int datum)
{
    unsigned int i;
    struct pair
    {
        struct pair   *point;  /* Pointers forming list linked in sorted order */
        int  value;  /* Values to sort */
    };
    static struct pair buffer[NWIDTH];  /* Buffer of nwidth pairs */
    static struct pair *datpoint={buffer}  /* pointer into circular buffer of data */;
    static struct pair small={NULL,STOPPER} ;  /* chain stopper. */
    static struct pair big={&small,0} ;  /* pointer to head (largest) of linked list.*/
    struct pair *successor   ;  /* pointer to successor of replaced data item */
    struct pair *scan        ;  /* pointer used to scan down the sorted list */
    struct pair *scanold     ;  /* previous value of scan */
    struct pair *median;     ;  /* pointer to median */

    if(datum == STOPPER) datum = STOPPER + 1; /* No stoppers allowed. */
    if( (++datpoint - buffer) >= NWIDTH) datpoint=buffer;  /* increment and wrap data in pointer.*/
    datpoint->value=datum        ;  /* Copy in new datum */
    successor=datpoint->point    ;  /* save pointer to old value's successor */
    median = &big                ;  /* median initially to first in chain */
    scanold = NULL               ;  /* scanold initially null. */
    scan = &big                  ;  /* points to pointer to first (largest) datum in chain */
  /* Handle chain-out of first item in chain as special case */
        if( scan->point == datpoint ) scan->point = successor;
        scanold = scan ;            /* Save this pointer and   */
        scan = scan->point ;        /* step down chain */
  /* loop through the chain, normal loop exit via break. */
    for( i=0 ;i<NWIDTH ; i++ )
    {
     /* Handle odd-numbered item in chain  */
        if( scan->point == datpoint ) scan->point = successor;  /* Chain out the old datum.*/
        if( (scan->value < datum) )        /* If datum is larger than scanned value,*/
        {
            datpoint->point = scanold->point;          /* chain it in here.  */
            scanold->point = datpoint;          /* mark it chained in. */
            datum = STOPPER;
        };
  /* Step median pointer down chain after doing odd-numbered element */
        median = median->point       ;       /* Step median pointer.  */
        if ( scan == &small ) break ;        /* Break at end of chain  */
        scanold = scan ;          /* Save this pointer and   */
        scan = scan->point ;            /* step down chain */
  /* Handle even-numbered item in chain.  */
        if( scan->point == datpoint ) scan->point = successor;
        if( (scan->value < datum) )
        {
            datpoint->point = scanold->point;
            scanold->point = datpoint;
            datum = STOPPER;
        };
        if ( scan == &small ) break;
        scanold = scan ;
        scan = scan->point;
    };
    return( median->value );
}
