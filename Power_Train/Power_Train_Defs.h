#ifndef POWER_TRAIN_DEFS_H_INCLUDED
#define POWER_TRAIN_DEFS_H_INCLUDED

#define INV_STATE_INITIALIZATION                    0x0
#define INV_STATE_STANDBY                           0x1
#define INV_STATE_STARUP                            0x2
#define INV_STATE_TEAR_DOWN                         0x3
#define INV_STATE_OPERATIONAL                       0x4
#define INV_STATE_FAILURE                           0x5
#define INV_STATE_CLOSE_DOWN                        0x6
#define INV_STATE_READY_TO_SHUTDOWN                 0x7
#define INV_STATE_WAITING_FOR_SAFE_CONDITION        0x8

#define INV_MODE_NEUTRAL    0x0
#define INV_MODE_EV         0x3

#define INV_CMD_TORQUE      0x1
#define INV_CMD_SPEED       0x2

#define INV_REQ_STATE_STANDBY       0x0
#define INV_REQ_STATE_OPERATIONAL   0x1
#define INV_REQ_STATE_SHUTDOWN      0x2

#define INV_REQ_MODE_NEUTRAL        0x0
#define INV_REQ_MODE_EV             0x3

#define INV_REQ_CMD_MODE_TORQUE     0x1
#define INV_REQ_CMD_MODE_SPEED      0x2

#define INV_CAN_BASEADDR_HIGH       0x040
#define INV_CAN_BASEADDR_LOW        0x440

#define INV_REVERSE_DERRATING_M     0.00333f
#define INV_REVERSE_DERRATING_C     2.0f

#define INV_STOP_SPEED_RPM          100

#endif // POWER_TRAIN_DEFS_H_INCLUDED
