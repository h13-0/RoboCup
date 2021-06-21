#include "vl53l0x_easy_api.h"

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

/**
 * @group: Platform Configs
 */
#define HAL 0
#define LL  1

#define STM_GPIO_LIB_TYPE LL
#define STM_RCC_LIB_TYPE  HAL

//Platforms
//GPIO Ports
#if(STM_GPIO_LIB_TYPE == HAL)

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_hal.h"
#endif

#define setXshut(GPIOx, PinMask)   HAL_GPIO_WritePin(GPIOx, PinMask, GPIO_PIN_SET)
#define clearXshut(GPIOx, PinMask) HAL_GPIO_WritePin(GPIOx, PinMask, GPIO_PIN_RESET)

#elif(STM_GPIO_LIB_TYPE == LL)

#if defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f1xx_ll_gpio.h"
#endif

#define setXshut(GPIOx, PinMask)   LL_GPIO_SetOutputPin(GPIOx, PinMask)
#define clearXshut(GPIOx, PinMask) LL_GPIO_ResetOutputPin(GPIOx, PinMask)

#endif

//RCC Ports
#if(STM_RCC_LIB_TYPE == HAL)
#define delay_ms HAL_Delay
#elif(STM_RCC_LIB_TYPE == LL)
//Write your delay implementation function here.

#endif

typedef struct
{
    uint8_t  isApertureSpads;
    uint8_t  VhvSettings;
    uint8_t  PhaseCal;
    uint32_t XTalkCalDistance;
    uint32_t XTalkCompensationRateMegaCps;
    uint32_t CalDistanceMilliMeter;
    int32_t  OffsetMicroMeter;
    uint32_t refSpadCount;
} VL53l0X_Adjust_t;

static VL53l0X_Adjust_t vl53l0x_adjust;

/**
 * @brief: Working mode.
 */
typedef struct
{
    FixPoint1616_t signalLimit;
    FixPoint1616_t sigmaLimit;
    uint32_t timingBudget;
    uint8_t preRangeVcselPeriod;
    uint8_t finalRangeVcselPeriod;
} VL53l0X_Mode_t;

/**
 * @brief: Preset working mode configurations.
 */
static VL53l0X_Mode_t Mode_data[]=
{
	//Default:
    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(18*65536),
        33000,
        14,
        10
    },

	//High_Accuracy:
    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(18*65536),
        200000,
        14,
        10
    },

	//Long_Range:
    {   (FixPoint1616_t)(0.1*65536),
        (FixPoint1616_t)(60*65536),
        33000,
        18,
        14
    },

	//High_Speed:
    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(32*65536),
        20000,
        14,
        10
    },
};

static void vl53l0xReset(VL53L0X_Dev_t *dev);
static void vl53l0x_adjust_init(void);

/**
 * @brief: Init VL53L0X
 * @param:
 * 		I2C_HandleTypeDef *port:      I2C Port.
 *		VL53L0X_Dev_t     *dev:       Pointer of vl53l10x.
 *		GPIO_TypeDef      *XshutPort: GPIO Port of Xshut.
 *		uint32_t          XshutPin:   GPIO Pin of Xshut.
 *
 * @return: Result.
 * @note:
 * 		This function will reset your vl53l0x to facilitate the use of multiple devices.
 * 		If you need to use more than one vl53l0x, manually change the device address to another appropriate value after initialization.
 */
VL53L0X_Error VL53l0X_Init(I2C_HandleTypeDef *port , VL53L0X_Dev_t *dev, GPIO_TypeDef *XshutPort, uint32_t XshutPin)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    vl53l0xReset(dev);

    dev -> I2cDevAddr = 0x29;        //I2C Address (After Reset, the default value is 0x29).
    dev -> comms_type = 1;           //I2C Mode.
    dev -> comms_speed_khz = 400;    //I2C Speed.

    dev -> XshutPort = XshutPort;    //Xshut Port
    dev -> XshutPin  = XshutPin;     //Xshut Pin

    VL53L0X_SetI2C_Port(port);

    Status = VL53L0X_DataInit(dev);
    if(Status!=VL53L0X_ERROR_NONE) return Status;
    delay_ms(2);

    vl53l0x_adjust_init();

    return Status;
}

/**
 * @brief: Set the address of VL53L0X.
 * @param:
 * 		VL53L0X_Dev_t *dev:    Pointer of vl53l10x.
 * 		uint8_t       newAddr: New address.
 * @return: Result.
 */
VL53L0X_Error VL53l0X_SetAddr(VL53L0X_Dev_t *dev,uint8_t newaddr)
{
    uint16_t Id;
    uint8_t FinalAddress;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t sta=0x00;

    FinalAddress = newaddr;

    if(FinalAddress==dev->I2cDevAddr)
        return VL53L0X_ERROR_NONE;

    //Set to standard I2C mode(400Khz).
    Status = VL53L0X_WrByte(dev,0x88,0x00);
    if(Status!=VL53L0X_ERROR_NONE)
    {
        sta=0x01;
        return Status;
    }

    //Test current I2C communication.
    Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
    if(Status!=VL53L0X_ERROR_NONE)
    {
        sta=0x02;
        return Status;
    }

    if(Id == 0xEEAA)
    {
        //Set new address.
        Status = VL53L0X_SetDeviceAddress(dev,FinalAddress);
        if(Status!=VL53L0X_ERROR_NONE)
        {
            sta=0x03;
            return Status;
        }

        dev->I2cDevAddr = FinalAddress;

        //Test current I2C communication.
        Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
        if(Status!=VL53L0X_ERROR_NONE)
        {
            sta=0x04;
            return Status;
        }
    }

    if(sta!=0)
        printf("sta:0x%x\r\n",sta);
    return Status;
}

/**
 * @brief: Set VL5310X Mode.
 * @param:
 * 		VL53L0X_Dev_t *dev:  Pointer of vl53l10x.
 * 		VL5310X_Mode_t mode: mode.
 * @return: Result.
 */
VL53L0X_Error VL53l0X_SetMode(VL53L0X_Dev_t *dev,uint8_t mode)
{
    VL53L0X_Error status = VL53L0X_ERROR_NONE;

    vl53l0xReset(dev);               //Reset VL53L0X
    status = VL53L0X_StaticInit(dev);

    status = VL53L0X_SetReferenceSpads(dev,vl53l0x_adjust.refSpadCount,vl53l0x_adjust.isApertureSpads);
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetRefCalibration(dev,vl53l0x_adjust.VhvSettings,vl53l0x_adjust.PhaseCal);
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetOffsetCalibrationDataMicroMeter(dev,vl53l0x_adjust.OffsetMicroMeter);
    if(status!= VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetXTalkCompensationRateMegaCps(dev,vl53l0x_adjust.XTalkCompensationRateMegaCps);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);

    status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);
    if(status != VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);

    return status;
}

/**
 * @brief: Vl53l0x single measurement distance.
 * @param:
 * 		VL53L0X_Dev_t *dev:  Pointer of vl53l10x.
 * 		uint16_t *pDistance: Pointer of distance in millimeter.
 */
VL53L0X_Error VL53l0X_Ranging(VL53L0X_Dev_t *dev, uint16_t *pDistance)
{
	VL53L0X_Error result = VL53L0X_ERROR_NONE;
	VL53L0X_RangingMeasurementData_t tempData;
	uint16_t distance = 0;

	result = VL53L0X_PerformSingleRangingMeasurement(dev, &tempData);
	if(result == VL53L0X_ERROR_NONE)
	{
		distance = tempData.RangeMilliMeter;
		*pDistance = distance;
		return VL53L0X_ERROR_NONE;
	} else {
		return result;
	}
}

/**
 * @brief: Adjust VL53L0X.
 * @param: Pointer of vl53l10x.
 * @return: Result.
 * @note:
 * 		After calibration, the calibration variable results will be output from printf.
 * 		Copy output to `void vl53l0x_adjust_init()` in `vl53l0x_easy_api.c`.
 * 		Recalibration may be required when the temperature difference is greater than 8 degrees.
 */
VL53L0X_Error VL53l0X_Adjust(VL53L0X_Dev_t *dev)
{
    VL53L0X_DeviceError Status = VL53L0X_ERROR_NONE;

    uint32_t refSpadCount;
    uint8_t  isApertureSpads;
    uint32_t XTalkCalDistance = 100;
    uint32_t XTalkCompensationRateMegaCps;
    uint32_t CalDistanceMilliMeter = 100<<16;
    int32_t  OffsetMicroMeter;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    uint8_t i=0;

    VL53L0X_StaticInit(dev);
    //Spads Adjust:
spads:
    delay_ms(10);
    printf("The SPADS Calibration Start...\r\n");
    Status = VL53L0X_PerformRefSpadManagement(dev,&refSpadCount,&isApertureSpads);//执行参考Spad管理
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("refSpadCount = %ld\r\n",refSpadCount);
        vl53l0x_adjust.refSpadCount = refSpadCount;
        printf("isApertureSpads = %d\r\n",isApertureSpads);
        vl53l0x_adjust.isApertureSpads = isApertureSpads;
        printf("The SPADS Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i == adjustRetryLimit) return Status;
        printf("SPADS Calibration Error,Restart this step\r\n");
        goto spads;
    }
    //Reference Adjust:
ref:
    delay_ms(10);
    printf("The Ref Calibration Start...\r\n");
    Status = VL53L0X_PerformRefCalibration(dev,&VhvSettings,&PhaseCal);//Ref参考校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("VhvSettings = %d\r\n",VhvSettings);
        vl53l0x_adjust.VhvSettings = VhvSettings;
        printf("PhaseCal = %d\r\n",PhaseCal);
        vl53l0x_adjust.PhaseCal = PhaseCal;
        printf("The Ref Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjustRetryLimit) return Status;
        printf("Ref Calibration Error,Restart this step\r\n");
        goto ref;
    }
    //Offset Adjust:
offset:
    delay_ms(10);
    printf("Offset Calibration:need a white target,in black space,and the distance keep 100mm!\r\n");
    printf("The Offset Calibration Start...\r\n");
    Status = VL53L0X_PerformOffsetCalibration(dev,CalDistanceMilliMeter,&OffsetMicroMeter);//偏移校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("CalDistanceMilliMeter = %ld mm\r\n",CalDistanceMilliMeter);
        vl53l0x_adjust.CalDistanceMilliMeter = CalDistanceMilliMeter;
        printf("OffsetMicroMeter = %ld mm\r\n",OffsetMicroMeter);
        vl53l0x_adjust.OffsetMicroMeter = OffsetMicroMeter;
        printf("The Offset Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjustRetryLimit) return Status;
        printf("Offset Calibration Error,Restart this step\r\n");
        goto offset;
    }
    //Xtalk Adjust:
xtalk:
    delay_ms(20);
    printf("Cross Talk Calibration:need a grey target\r\n");
    printf("The Cross Talk Calibration Start...\r\n");
    Status = VL53L0X_PerformXTalkCalibration(dev,XTalkCalDistance,&XTalkCompensationRateMegaCps);//串扰校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("XTalkCalDistance = %ld mm\r\n",XTalkCalDistance);
        vl53l0x_adjust.XTalkCalDistance = XTalkCalDistance;
        printf("XTalkCompensationRateMegaCps = %ld\r\n",XTalkCompensationRateMegaCps);
        vl53l0x_adjust.XTalkCompensationRateMegaCps = XTalkCompensationRateMegaCps;
        printf("The Cross Talk Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjustRetryLimit) return Status;
        printf("Cross Talk Calibration Error,Restart this step\r\n");
        goto xtalk;
    }

    printf("Copy the following code to vl53l0x_easy_api.c's vl53l0x_adjust_init() function\r\n\r\n");

    printf("vl53l0x_adjust.isApertureSpads = %d;\r\n",isApertureSpads);
    printf("vl53l0x_adjust.VhvSettings = %d;\r\n",VhvSettings);
    printf("vl53l0x_adjust.PhaseCal = %d;\r\n",PhaseCal);
    printf("vl53l0x_adjust.XTalkCalDistance = %ld;\r\n",XTalkCalDistance);
    printf("vl53l0x_adjust.XTalkCompensationRateMegaCps = %ld;\r\n",XTalkCompensationRateMegaCps);
    printf("vl53l0x_adjust.CalDistanceMilliMeter = %ld;\r\n",CalDistanceMilliMeter);
    printf("vl53l0x_adjust.OffsetMicroMeter = %ld;\r\n",OffsetMicroMeter);
    printf("vl53l0x_adjust.refSpadCount = %ld;\r\n",refSpadCount);
    printf("\r\n");

    printf("All the Calibration has Finished!\r\n");
    printf("Calibration is successful!!\r\n");

    return Status;
}

/**
 * @brief: Reset VL53L0X
 * @param: Pointer of vl53l10x.
 */
static void vl53l0xReset(VL53L0X_Dev_t *dev)
{
    uint8_t addr;
    addr = dev->I2cDevAddr;     //Save Addr
    clearXshut(dev -> XshutPort, dev -> XshutPin);
    delay_ms(30);
    setXshut(dev -> XshutPort, dev -> XshutPin);
    delay_ms(30);
    dev->I2cDevAddr = 0x29;
    VL53l0X_SetAddr(dev,addr);  //Load Addr
    VL53L0X_DataInit(dev);
}

static void vl53l0x_adjust_init(void)
{
	vl53l0x_adjust.isApertureSpads = 1;
	vl53l0x_adjust.VhvSettings = 29;
	vl53l0x_adjust.PhaseCal = 1;
	vl53l0x_adjust.XTalkCalDistance = 100;
	vl53l0x_adjust.XTalkCompensationRateMegaCps = 0;
	vl53l0x_adjust.CalDistanceMilliMeter = 6553600;
	vl53l0x_adjust.OffsetMicroMeter = 22000;
	vl53l0x_adjust.refSpadCount = 10;
}
