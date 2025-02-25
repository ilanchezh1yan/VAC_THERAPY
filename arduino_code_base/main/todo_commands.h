#ifndef TODO_COMMANDS_H
#define TODO_COMMANDS_H
#define PAUSE                       0x0017       // SAME DATA FOR ALERT POPUP TOO          
#define RESUME                      0x0015
#define STOP                        0x0016

#define CONTINUOUS_MODE             0X0000
#define INTERMITTEN_MODE            0X0001
#define CONTINUOUS_MODE_INSTIL      0X0100
#define INTERMITTEN_MODE_INSTIL     0X0101

/* VP address of the respective buttons in display */
#define CHANGE_CANISTER             0x10D0 
#define CHECK_CANISTER              0x1300    // DATA FROM OO24-NOT INSERTED AND 0025- INSERTED

#define LEAKAGE_SEND                0X1200    // DATA FROM 0001 TO 0020 FOR IMAGE CHANGING IN 5% RATE

#define SET_CONST_PRESSURE          0x2000
#define PRESSURE_INSTILL            0X4000
#define VOLUME_INSTILL              0X4005 
#define SOAKTIME_INSTIL             0X4010
#define SUCTIONTIME_INSTILL         0X4015
#define SET_ON_TIME                 0x3005
#define SET_OFF_TIME                0x3010

#define MODE_INTERRUPTION           0X5000
#define RESTORE_DEFAULT             0X2005    // DATA VALUE 0000 - CONTINUOUS, 0001 - INTERMITTENT, 0100 - INSTILL      
#define SEND_PRESSURE               0X2100
#define NPWT_MODES                  0x2010    // DATA VALUE 0000 - CONTINUOUS, 0001 - INTERMITTENT, 0100 - INSTILL

#define TIMER_HRS                   0X6000
#define TIMER_MINUTES               0X6005

/**BATTERY PERCENTAGE DETAILS:
    FOR FRONT PAGE: VP-1100 DATA: 0001 TO 0020 (INC IN 5% RANGE)
    FOR REMAINING PAGE: VP-1100 DATA: 0021 TO 0040 **/
#define ORGANIZE_COMMAND(x) x << 8 | x >> 8
#endif