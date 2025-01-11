/**
 * @file       modbus_slave.c
 * @brief      file_brief
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include <string.h>

#include "mb_rtu_app.h"
#include "usbd_cdc_if.h"
#include "modbus_slave.h"

/* Private defines -----------------------------------------------------------*/

/**
 * Modbus broadcast address
 */
#define MODBUS_BROADCAST              0
/**
 * Modbus exception mask
 */
#define MODBUS_EXCEPTION_MASK         0x80
/**
 * Modbus header length
 */
#define MODBUS_HEADER_LENGTH          2
/**
 * Modbus minimal payload length
 */
#define MODBUS_MIN_PAYLOAD_LENGTH     4
/**
 * Modbus maximal data length
 */
#define MODBUS_DATA_LENGTH            252
/**
 * Modbus CRC length
 */
#define MODBUS_CRC_LENGTH             2

/**
 * Registers offset
 */
#define MODBUS_OFFSET_HOLDING               0
#define MODBUS_OFFSET_INPUT                 0

/**
 * Return exception codes
 */
#define MODBUS_EXCEPT_INVALID_COMMAND     1
#define MODBUS_EXCEPT_INVALID_ADDRESS     2
#define MODBUS_EXCEPT_INVALID_SIZE        3

/**
 * Modbus slave address range
 */
#define MODBUS_SLAVE_ADDR                22

/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

/**
 * Definition of Modbus function codes
 */
typedef enum
{
  MB_READ_COILS = 1,           //!< MB_READ_COILS
  MB_READ_DISCRETE_INPUTS,     //!< MB_READ_DISCRETE_INPUTS
  MB_READ_HOLDING_REGS,        //!< MB_READ_HOLDING_REGS
  MB_READ_INPUT_REGS,          //!< MB_READ_INPUT_REGS
  MB_WRITE_SINGLE_COIL,        //!< MB_WRITE_SINGLE_COIL
  MB_WRITE_SINGLE_REG,         //!< MB_WRITE_SINGLE_REG
  MB_WRITE_MULTIPLE_COILS = 15,//!< MB_WRITE_MULTIPLE_COILS
  MB_WRITE_MULTIPLE_REGS,      //!< MB_WRITE_MULTIPLE_REGS
} MbSlave_Functions_t;


/**
 * Definition of all private variables
 */
typedef struct
{
  uint8_t frameOk;
  uint8_t myAddress;
  uint16_t rxPtr;
} MbSlave_Private_t;

/**
 * A frame buffer
 */
typedef struct
{
  uint8_t slaveAddr;
  uint8_t funcCode;
  uint8_t data[MODBUS_DATA_LENGTH];
  uint8_t crc[2];
  uint16_t size;
} MbSlave_Frame_t;

/* Private constants ---------------------------------------------------------*/

/**
 * Pre-computed CRC16 High byte
 */
static const __attribute__((section (".sectionIsrConst"))) uint8_t tableCrcHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};

/**
 * Pre-computed CRC16 Low byte
 */
static const uint8_t tableCrcLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};

/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static MbSlave_Private_t mod;

/**
 * Instance for holding inbound and outbound frames
 */
static MbSlave_Frame_t inFrame;
static uint8_t outFram;
static MbSlave_Frame_t outFrame;

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static Status_t MbSlave_CheckFrame(void);

static Status_t MbSlave_ProcessFrame(void);

static Status_t MbSlave_CRC16(MbSlave_Frame_t *frame);

/* Functions -----------------------------------------------------------------*/

Status_t MbSlave_Init(void)
{
  Status_t ret = STATUS_OK;
  MbSlave_UpdateSlaveAddress();
  outFram = 1;
  return ret;
}


Status_t MbSlave_SettingsChanged(void)
{
  Status_t ret = STATUS_OK;

  MbSlave_UpdateSlaveAddress();

  /* default values */
  mod.frameOk = 0;

  /* reset bus and wait for first packet */
  MbSlave_BusReset();

  return ret;
}


Status_t MbSlave_Receive(uint8_t* chunk_data, uint32_t chunk_len)
{
  Status_t ret = STATUS_OK;

  if (chunk_len <= MODBUS_HEADER_LENGTH + MODBUS_DATA_LENGTH + MODBUS_CRC_LENGTH - mod.rxPtr)
  {
    /* Chunk is fit into the buffer, insert it and try to process */
    memcpy((uint8_t *)&inFrame + mod.rxPtr, chunk_data, chunk_len);
    mod.rxPtr += chunk_len;
    MbSlave_CheckFrame();
    if (mod.frameOk == 1)
    {
      /* Notify the waiting task */
    }
  }
  else
  {
    /* Prepare for next reception */
    MbSlave_BusReset();
  }
  return ret;
}


Status_t MbSlave_BusReset(void)
{
  Status_t ret = STATUS_OK;
  mod.rxPtr = 0;
  memset((uint8_t *)&inFrame, 0, MODBUS_HEADER_LENGTH + MODBUS_DATA_LENGTH + MODBUS_CRC_LENGTH);
  return ret;
}


Status_t MbSlave_Handle(void)
{
  Status_t ret = STATUS_OK;

  if (mod.frameOk)
  {
    mod.frameOk = 0;

    /* Process frame */
    MbSlave_ProcessFrame();

    /* Prepare and send response for unicast messages */
    if (inFrame.slaveAddr == mod.myAddress)
    {
      /* Compose response and send it */
      outFrame.slaveAddr = mod.myAddress;
      MbSlave_CRC16(&outFrame);
      outFrame.data[outFrame.size] = outFrame.crc[0];
      outFrame.data[outFrame.size + 1] = outFrame.crc[1];

      /* send packet response */
      CDC_Transmit_FS((uint8_t*) &outFrame, outFrame.size + MODBUS_CRC_LENGTH + MODBUS_HEADER_LENGTH);
      ret = STATUS_BUSY;
    }
    else
    {
      /* Start next reception */
      MbSlave_BusReset();
    }
  }

  return ret;
}


Status_t MbSlave_UpdateSlaveAddress(void)
{
  Status_t ret = STATUS_OK;

  mod.myAddress = MODBUS_SLAVE_ADDR;
  return ret;
}


static Status_t MbSlave_CheckFrame(void)
{
  Status_t ret = STATUS_OK;
  uint8_t command;
  uint8_t supported = 0;

  /* Check address */
  if (inFrame.slaveAddr == mod.myAddress || inFrame.slaveAddr == MODBUS_BROADCAST)
  {
    command = inFrame.funcCode;
    /* Calculate size of data and check supported operation codes */
    if (command >= MB_READ_COILS && command <= MB_WRITE_SINGLE_REG)
    {
      inFrame.size = 0;
      supported = 1;
    }
    else if (command == MB_WRITE_MULTIPLE_REGS || command == MB_WRITE_MULTIPLE_COILS)
    {
      inFrame.size = inFrame.data[2];
      inFrame.size <<= 8;
      inFrame.size |= inFrame.data[3];

      if (command == MB_WRITE_MULTIPLE_COILS)
      {
        if (inFrame.size % 8)
        {
          inFrame.size = (inFrame.size / 8) + 2; /* 1 extra byte is for unaligned data, 1 byte for size */
        }
        else
        {
          inFrame.size = (inFrame.size / 8) + 1; /* 1 byte for size */
        }
      }
      else if (command == MB_WRITE_MULTIPLE_REGS)
      {
        inFrame.size = (inFrame.size * 2) + 1; /* registers are 16b, 1 byte for size */
      }
      supported = 1;
    }

    /* Add header of command to size variable */
    inFrame.size += MODBUS_MIN_PAYLOAD_LENGTH;
    /* Check maximal length of modbus frame */
    if (inFrame.size < MODBUS_DATA_LENGTH - 2)  // CRC must fit into received data
    {
      /* compute CRC16 from received packet */
      MbSlave_CRC16(&inFrame);
      /* we have original CRC16 in received data */
      if (inFrame.crc[0] == inFrame.data[inFrame.size] && inFrame.crc[1] == inFrame.data[inFrame.size + 1] && supported)
      {
        /* move to next step */
        mod.frameOk = 1;
      }
    }
  }

  return ret;
}


static Status_t MbSlave_ProcessFrame(void)
{
  Status_t ret = STATUS_OK;
  uint32_t i;
  uint16_t addr;
  uint16_t count;
  uint16_t size;
  uint16_t offset = 0;
  uint16_t val;

  /* prepare address for outframe */
  outFrame.funcCode = inFrame.funcCode;
  /* default size for error */
  outFrame.size = 1;
  /* parse start address and count */
  addr = inFrame.data[0];
  addr <<= 8;
  addr |= inFrame.data[1];
  count = inFrame.data[2];
  count <<= 8;
  count |= inFrame.data[3];

  switch (inFrame.funcCode)
  {
    case MB_READ_HOLDING_REGS:
      offset = MODBUS_OFFSET_HOLDING;
      size = MB_HOLD_LAST;
      break;
    case MB_READ_INPUT_REGS:
      offset = MODBUS_OFFSET_INPUT;
      size = MB_INPUT_LAST;
      break;
    case MB_WRITE_MULTIPLE_REGS:
      offset = MODBUS_OFFSET_HOLDING;
      size = MB_HOLD_LAST;
      break;
    default:
      /* prepare exception */
      size = 0;
      outFrame.funcCode = inFrame.funcCode + MODBUS_EXCEPTION_MASK;
      outFrame.data[0] = MODBUS_EXCEPT_INVALID_COMMAND; /* invalid function code */
      break;
  }

  /* some value checking */
  if (outFrame.funcCode < MODBUS_EXCEPTION_MASK && (count < 1 || count > 0x7b))
  {
    outFrame.funcCode = inFrame.funcCode + MODBUS_EXCEPTION_MASK;
    outFrame.data[0] = MODBUS_EXCEPT_INVALID_SIZE; /* invalid size/value */
  }

  if (outFrame.funcCode < MODBUS_EXCEPTION_MASK
      && (addr > (offset + size) || (addr) < offset || (addr + count) > (offset + size + 1)))
  {
#ifdef MODBUS_UPGRADE
    if (addr < MB_UPGR_BASE_ADDRESS || addr + count > MB_UPGR_END_ADDRESS + 1)
#endif
    {
      outFrame.funcCode = inFrame.funcCode + MODBUS_EXCEPTION_MASK;
      outFrame.data[0] = MODBUS_EXCEPT_INVALID_ADDRESS; /* invalid address */
    }
  }

  /* it looks ok so far */
  if (outFrame.funcCode < MODBUS_EXCEPTION_MASK)
  {
    /* Subtract offset from address */
    addr -= offset;

    /* do the register stuff */
    switch (inFrame.funcCode)
    {
      case MB_READ_HOLDING_REGS:
        outFrame.data[0] = count * 2;
        outFrame.size = outFrame.data[0] + 1;
        for (i = 0; i < count; i++)
        {
          MbRtu_ReadHoldingRegCallback(addr + i, &val);
          memcpy((outFrame.data + i * 2 + 1), &val, 2);
        }
        break;

      case MB_READ_INPUT_REGS:
        outFrame.data[0] = count * 2;
        outFrame.size = outFrame.data[0] + 1;
        for (i = 0; i < count; i++)
        {
          MbRtu_ReadInputRegCallback(addr + i, &val);
          memcpy((outFrame.data + i * 2 + 1), &val, 2);
        }

        break;

      case MB_WRITE_MULTIPLE_REGS:
        size = addr + count;
        for (i = 0; i < count; i++)
        {
          memcpy(&val, (inFrame.data + i * 2 + 1 + MODBUS_MIN_PAYLOAD_LENGTH), 2);
          MbRtu_WriteHoldingRegCallback(addr + i, __REV16(val));
        }

        addr += offset;
        outFrame.data[0] = (addr >> 8) & 0xff;
        outFrame.data[1] = addr & 0xff;
        outFrame.data[2] = (count >> 8) & 0xff;
        outFrame.data[3] = count & 0xff;
        outFrame.size = 4;
        break;
    }
  }

  return ret;
}


/**
 * Computation of CRC16 using pre-computed tables.
 *
 * @param frame - data frame
 * @return Status - standard status code (0 - OK)
 */
static Status_t MbSlave_CRC16(MbSlave_Frame_t *frame)
{
  uint8_t index;
  frame->crc[0] = 0xFF;
  frame->crc[1] = 0xFF;
  uint16_t i;

  /* calculate the CRC */
  index = frame->crc[0] ^ frame->slaveAddr;
  frame->crc[0] = frame->crc[1] ^ tableCrcHi[index];
  frame->crc[1] = tableCrcLo[index];
  index = frame->crc[0] ^ frame->funcCode;
  frame->crc[0] = frame->crc[1] ^ tableCrcHi[index];
  frame->crc[1] = tableCrcLo[index];

  for (i = 0; i < frame->size; i++)
  {
    index = frame->crc[0] ^ frame->data[i];
    frame->crc[0] = frame->crc[1] ^ tableCrcHi[index];
    frame->crc[1] = tableCrcLo[index];
  }

  return STATUS_OK;
}

/* Private Functions ---------------------------------------------------------*/

/** @} */
