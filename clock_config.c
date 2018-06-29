/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/
/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v4.0
processor: MK66FN2M0xxx18
package_id: MK66FN2M0VLQ18
mcu_data: ksdk2_0
processor_version: 3.0.1
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

#include "fsl_smc.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCG_PLL_DISABLE                                   0U  /*!< MCGPLLCLK disabled */
#define SIM_CLKOUT_SEL_FLEXBUS_CLK                        0U  /*!< CLKOUT pin clock select: FlexBus clock */
#define SIM_ENET_1588T_CLK_SEL_CLKIN_CLK                  3U  /*!< SDHC clock select: CLKIN (External bypass clock) */
#define SIM_ENET_RMII_CLK_SEL_CLKIN_CLK                   1U  /*!< SDHC clock select: CLKIN (External bypass clock) */
#define SIM_LPUART_CLK_SEL_OSCERCLK_CLK                   2U  /*!< LPUART clock select: OSCERCLK clock */
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
#define SIM_PLLFLLSEL_MCGFLLCLK_CLK                       0U  /*!< PLLFLL select: MCGFLLCLK clock */
#define SIM_SDHC_CLK_SEL_OSCERCLK_CLK                     2U  /*!< SDHC clock select: OSCERCLK clock */
#define SIM_TPM_CLK_SEL_OSCERCLK_CLK                      2U  /*!< TPM clock select: OSCERCLK clock */
#define SIM_TRACE_CLK_DIV_1                               0U  /*!< Trace clock divider divisor: divided by 1 */
#define SIM_TRACE_CLK_FRAC_1                              0U  /*!< Trace clock divider fraction: multiplied by 1 */
#define SIM_TRACE_CLK_SEL_CORE_SYSTEM_CLK                 1U  /*!< Trace clock select: Core/system clock */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetFllExtRefDiv
 * Description   : Configure FLL external reference divider (FRDIV).
 * Param frdiv   : The value to set FRDIV.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetFllExtRefDiv(uint8_t frdiv)
{
    MCG->C1 = ((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(frdiv));
}

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockHSRUN **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockHSRUN
outputs:
- {id: Bus_clock.outFreq, value: 60 MHz}
- {id: CLKOUT.outFreq, value: 60 MHz}
- {id: Core_clock.outFreq, value: 120 MHz}
- {id: ENET1588TSCLK.outFreq, value: 50 MHz}
- {id: Flash_clock.outFreq, value: 24 MHz}
- {id: FlexBus_clock.outFreq, value: 60 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUARTCLK.outFreq, value: 8 MHz}
- {id: MCGFFCLK.outFreq, value: 250 kHz}
- {id: MCGIRCLK.outFreq, value: 2 MHz}
- {id: OSCERCLK.outFreq, value: 8 MHz}
- {id: OSCERCLK_UNDIV.outFreq, value: 8 MHz}
- {id: RMIICLK.outFreq, value: 50 MHz}
- {id: SDHCCLK.outFreq, value: 8 MHz}
- {id: System_clock.outFreq, value: 120 MHz}
- {id: TPMCLK.outFreq, value: 8 MHz}
- {id: TRACECLKIN.outFreq, value: 120 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: powerMode, value: HSRUN}
- {id: CLKOUTConfig, value: 'yes'}
- {id: ENETTimeSrcConfig, value: 'yes'}
- {id: LPUARTClkConfig, value: 'yes'}
- {id: MCG.FRDIV.scale, value: '32'}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.PLLS.sel, value: MCG.PLLCS}
- {id: MCG.PRDIV.scale, value: '1', locked: true}
- {id: MCG.VDIV.scale, value: '30', locked: true}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: OSC_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_ERCLKEN_UNDIV_CFG, value: Enabled}
- {id: OSC_CR_SYS_OSC_CAP_LOAD_CFG, value: SC22PF}
- {id: RMIISrcConfig, value: 'yes'}
- {id: RTC_CR_CLKO_CFG, value: Disabled}
- {id: RTC_CR_OSC_CAP_LOAD_CFG, value: SC22PF}
- {id: SDHCClkConfig, value: 'yes'}
- {id: SIM.LPUARTSRCSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.OUTDIV2.scale, value: '2', locked: true}
- {id: SIM.OUTDIV3.scale, value: '2', locked: true}
- {id: SIM.OUTDIV4.scale, value: '5', locked: true}
- {id: SIM.RMIICLKSEL.sel, value: SIM.ENET_1588_CLK_EXT}
- {id: SIM.SDHCSRCSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.TIMESRCSEL.sel, value: SIM.ENET_1588_CLK_EXT}
- {id: SIM.TPMSRCSEL.sel, value: OSC.OSCERCLK}
- {id: TPMClkConfig, value: 'yes'}
- {id: TraceClkConfig, value: 'yes'}
sources:
- {id: OSC.OSC.outFreq, value: 8 MHz, enabled: true}
- {id: SIM.ENET_1588_CLK_EXT.outFreq, value: 50 MHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockHSRUN =
    {
        .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
        .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 32 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselOsc,                 /* Selects System Oscillator (OSCCLK) */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0x0U,                    /* PLL Reference divider: divided by 1 */
                .vdiv = 0xeU,                     /* VCO divider: multiplied by 30 */
            },
        .pllcs = kMCG_PllClkSelPll0,              /* PLL0 output clock is selected */
    };
const sim_clock_config_t simConfig_BOARD_BootClockHSRUN =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGFLLCLK_CLK, /* PLLFLL select: MCGFLLCLK clock */
        .pllFllDiv = 0,                           /* PLLFLLSEL clock divider divisor: divided by 1 */
        .pllFllFrac = 0,                          /* PLLFLLSEL clock divider fraction: multiplied by 1 */
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x1140000U,                    /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /2, OUTDIV3: /2, OUTDIV4: /5 */
    };
const osc_config_t oscConfig_BOARD_BootClockHSRUN =
    {
        .freq = 8000000U,                         /* Oscillator frequency: 8000000Hz */
        .capLoad = (kOSC_Cap2P | kOSC_Cap4P | kOSC_Cap16P),/* Oscillator capacity load: 22pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = kOSC_ErClkEnable,   /* Enable external reference clock, disable external reference clock in STOP mode */
                .erclkDiv = 0,                    /* Divider for OSCERCLK: divided by 1 */
            }
    };

/*******************************************************************************
 * Code for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
void BOARD_BootClockHSRUN(void)
{
    /* Set HSRUN power mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeHsrun(SMC);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateHsrun)
    {
    }
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Initializes OSC0 according to board configuration. */
    CLOCK_InitOsc0(&oscConfig_BOARD_BootClockHSRUN);
    CLOCK_SetXtal0Freq(oscConfig_BOARD_BootClockHSRUN.freq);
    /* Configure the Internal Reference clock (MCGIRCLK). */
    CLOCK_SetInternalRefClkConfig(mcgConfig_BOARD_BootClockHSRUN.irclkEnableMode,
                                  mcgConfig_BOARD_BootClockHSRUN.ircs, 
                                  mcgConfig_BOARD_BootClockHSRUN.fcrdiv);
    /* Configure FLL external reference divider (FRDIV). */
    CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockHSRUN.frdiv);
    /* Set MCG to PEE mode. */
    CLOCK_BootToPeeMode(mcgConfig_BOARD_BootClockHSRUN.oscsel,
                        mcgConfig_BOARD_BootClockHSRUN.pllcs,
                        &mcgConfig_BOARD_BootClockHSRUN.pll0Config);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockHSRUN);
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKHSRUN_CORE_CLOCK;
    /* Set enet timestamp clock source. */
    CLOCK_SetEnetTime0Clock(SIM_ENET_1588T_CLK_SEL_CLKIN_CLK);
    /* Set RMII clock source. */
    CLOCK_SetRmii0Clock(SIM_ENET_RMII_CLK_SEL_CLKIN_CLK);
    /* Set SDHC clock source. */
    CLOCK_SetSdhc0Clock(SIM_SDHC_CLK_SEL_OSCERCLK_CLK);
    /* Set LPUART clock source. */
    CLOCK_SetLpuartClock(SIM_LPUART_CLK_SEL_OSCERCLK_CLK);
    /* Set TPM clock source. */
    CLOCK_SetTpmClock(SIM_TPM_CLK_SEL_OSCERCLK_CLK);
    /* Set CLKOUT source. */
    CLOCK_SetClkOutClock(SIM_CLKOUT_SEL_FLEXBUS_CLK);
    /* Set debug trace clock source. */
    CLOCK_SetTraceClock(SIM_TRACE_CLK_SEL_CORE_SYSTEM_CLK, SIM_TRACE_CLK_DIV_1, SIM_TRACE_CLK_FRAC_1);
}

