#ifndef PF_RETVAL
#define PF_RETVAL

typedef enum
{
    RV_OK = 0,
    RV_NO_ECHO,
    RV_CANT_OPEN_PORT,
    RV_CANT_INSERT_DATA,
    RV_TIMEOUT
}retval_t;

#endif // PF_RETVAL

