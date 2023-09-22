#ifndef AOA_MSG_H_
#define AOA_MSG_H_
typedef struct tag_AOA_msg
{
    short AOA;
    unsigned short CAS;
    short p_Alt;
    short p_RamAdc;
    short p_AoaAdc;
} AOA_msg;


#endif /*AOA_MSG_H_*/
