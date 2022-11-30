#include "main.h"
#include "camera.h"
#include "lcd.h"
#include "profile.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
extern "C" {
#include "genNN.h"
#include "tinyengine_function.h"

#include "ant_ml_runtime_api.h"
#include "network.h"
#include "network_data.h"
}
//#define TESTTENSOR

#include "stm32746g_discovery.h"

static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void MX_GPIO_Init(void);

#define RES_W 64
#define RES_H 64
#define OUTPUT_CH 2

void SystemClock_Config(void);
void StartDefaultTask(void const *argument);

signed char out_int[OUTPUT_CH];

float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// Network that we are testing
extern ant_ml_model_info network_network;
uint32_t __models_section_start__ = (uint32_t)&network_network;
uint32_t __models_section_end__ = (uint32_t)&network_network + sizeof(ant_ml_model_info);
static ant_ml_model_info* _model_p = &network_network;

// Global handle to reference the instantiated NN
static ant_ml_handle _network = ANT_ML_HANDLE_NULL;

#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_10
#define BUTTON2_GPIO_Port GPIOF

static int32_t quantize_val(float val, const ant_ml_quantization_info* quant) {
	float new_val;
	float input_scale = quant->scale[0];
	int32_t input_zero_point = quant->zero_point[0];
	new_val = val / input_scale + input_zero_point;
	return (int32_t)new_val;
}

static float dequantize_val(int32_t val, ant_ml_quantization_info* quant) {
	float new_val;
	float output_scale = quant->scale[0];
	int32_t output_zero_point = quant->zero_point[0];
	new_val = (val - output_zero_point) * output_scale;
	return new_val;
}

//uint16_t *RGBbuf;

int main(void) {
  uint16_t RGBbuf[RES_H*RES_W*10];
  int8_t buf[RES_H*RES_W*10];
  char showbuf[150];
  CPU_CACHE_Enable();
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);


  lcdsetup();

  // ANT network initialize
  ant_ml_status err = ANT_ML_STATUS_OK;
  ant_ml_ptr built_in_activations = ANT_ML_MODEL_activations(_model_p);
  err = ant_ml_create(_model_p, built_in_activations, &_network);
  if (err != ANT_ML_STATUS_OK) {
	  return -1;
  }

  // Get input and output
  ant_ml_tensor* input = ant_ml_get_input(_network, 0);
  if (input == NULL) {
	  return -1;
  }
  int8_t* input_ptr = (int8_t*)input->dltensor.data;
  ant_ml_tensor* output = ant_ml_get_output(_network, 0);
  if (output == NULL) {
	  return -1;
  }
  int8_t* output_ptr = (int8_t*)output->dltensor.data;

  // Get quantization info
  const ant_ml_quantization_info* input_quant = ant_ml_get_quantization(input);
  const ant_ml_quantization_info* output_quant = ant_ml_get_quantization(output);

  int camErr = initCamera();

  uint32_t start, end;
  StartCapture();
  //signed char *input = getInput();

  //RGBbuf = (uint16_t *)&input[80 * 80 * 4];
  int t_mode = 0;
  while (1) {
    start = HAL_GetTick();
    ReadCapture();
    StartCapture();
    DecodeandProcessAndRGB(RES_W, RES_H, buf, RGBbuf, 1);
    for (int i = 0; i < RES_W; i++) {
      for (int j = 0; j < RES_W; j++) {
        uint8_t red = (int32_t)buf[(RES_W * i + j) * 3] + 128;
        uint8_t green = (int32_t)buf[(RES_W * i + j) * 3 + 1] + 128;
        uint8_t blue = (int32_t)buf[(RES_W * i + j) * 3 + 2] + 128;

        uint16_t b = (blue >> 3) & 0x1f;
        uint16_t g = ((green >> 2) & 0x3f) << 5;
        uint16_t r = ((red >> 3) & 0x1f) << 11;

        RGBbuf[j + RES_W * i] = (uint16_t)(r | g | b);

        if (j < 50 && i < 50) {
        	float nval = ((0.2126 * red) + (0.7152 * green) + (0.0722 * blue)) / 255.0;
			int8_t qval = quantize_val(nval, input_quant);
            input_ptr[j + 50 * i] = qval;
        }

        /*
        if (i < 50 && j < 50) {
			float nval_red = ((float)red) / 255.0;
			float nval_green = ((float)green) / 255.0;
			float nval_blue = ((float)blue) / 255.0;
			int8_t qval_red = quantize_val(nval_red, input_quant);
			int8_t qval_green = quantize_val(nval_green, input_quant);
			int8_t qval_blue = quantize_val(nval_blue, input_quant);
			input_ptr[(50 * i + j) * 3] = qval_red;
			input_ptr[(50 * i + j) * 3 + 1] = qval_green;
			input_ptr[(50 * i + j) * 3 + 2] = qval_blue;
			//input_ptr[(RES_W * i + j) * 3] = 0;
			//input_ptr[(RES_W * i + j) * 3 + 1] = 0;
			//input_ptr[(RES_W * i + j) * 3 + 2] = 0;
        }
        */
      }
    }
    loadRGB565LCD(10, 10, RES_W, RES_W, RGBbuf, 3);

  	//invoke_new_weights_givenimg(out_int);
    if (ant_ml_run(_network) != ANT_ML_STATUS_OK) {
    	return -1;
    }
  	int person = 0;
  	if (out_int[0] > out_int[1]) {
  	  person = 0;
  	}
  	else {
  	  person = 1;
  	}
  	end = HAL_GetTick();
  	int idx = 0;
  	volatile int8_t val = output_ptr[0];
  	for (int i = 1; i < 2; i++) {
  		volatile int8_t new_val = output_ptr[i];
  		if (val < new_val) {
  			val = new_val;
  			idx = i;
  		}
  	}
  	person = idx;
  	//volatile float val = output_ptr[0];
  	volatile int decimal = int(val);
  	volatile int floating = (int)((val - (float)decimal) * 1000);
  	sprintf(showbuf, "%d: %d.%03d ", idx, decimal, floating);
  	displaystring(showbuf, 273, 10);
   	detectResponse(person, end - start, t_mode, 0, 0);
  }
}

void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }
}
static void Error_Handler(void) {

  BSP_LED_On(LED1);
  while (1) {
  }
}

static void CPU_CACHE_Enable(void) {

  SCB_EnableICache();

  SCB_EnableDCache();
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin | ARDUINO_D8_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D6_Pin | ULPI_D5_Pin | ULPI_D3_Pin |
                        ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
  HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Audio_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_P_Pin | OTG_FS_N_Pin | OTG_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D7_Pin | ARDUINO_D8_Pin | LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = uSD_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TP3_Pin | NC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_STP_Pin | ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_RXER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_MISO_D12_Pin | ARDUINO_MOSI_PWM_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON2_GPIO_Port, &GPIO_InitStruct);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {

  while (1) {
  }
}
#endif
