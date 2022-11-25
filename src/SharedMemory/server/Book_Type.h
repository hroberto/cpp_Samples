#ifndef __BOOK_TYPE_H__
#define __BOOK_TYPE_H__



typedef struct  {
    char symbol[20];
    double top_bid[2];
    double top_ask[2];
    unsigned int sequence{};
} Book_Type;


#endif
