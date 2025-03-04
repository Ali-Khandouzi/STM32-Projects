/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "includes.h"
#include <string.h>
#include <stdio.h>
#include "retarget.h"
#include <stdlib.h>

/* PDU Definitions -----------------------------------------------------------*/
#define PDU_SIZE           	8
#define PDU_HEADER         	0x5A
#define SID_LED_BLINK  	   	0x18
#define SID_ADC_READ    	0x38
#define SID_BUTTON_EVENT    0x22
#define SID_NEG_RESPONSE    0x7F
#define SID_POS_RESPONSE    0xAA

// PDU Structure Definition
typedef struct {
    uint8_t header; 		// Always 0x5A
    uint8_t sid;			// Subfunction ID
    uint8_t data[PDU_SIZE - 2];		// Payload data
} PDU;

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC3_Init(void);

/* Task Definitions ----------------------------------------------------------*/
void TaskReceive(void* pdata);
void TaskTransmit(void* pdata);
void TaskButton(void* pdata);
void TaskLED(void* pdata);
void TaskADC(void* pdata);

/* Definition of Task Stacks ------------------------------------------------ */
#define TASK_STACKSIZE 		256 * 4
OS_STK	Task_Receive_stk[TASK_STACKSIZE];
OS_STK	Task_Transmit_stk[TASK_STACKSIZE];
OS_STK	Task_Button_stk[TASK_STACKSIZE];
OS_STK	Task_LED_stk[TASK_STACKSIZE];
OS_STK	Task_ADC_stk[TASK_STACKSIZE];

/* Definition of Task Priorities ------------------------------------------------ */
#define TASK_BUTTON_PRIORITY		7
#define TASK_RECEIVE_PRIORITY		9
#define TASK_TRANSMIT_PRIORITY		8
#define TASK_ADC_PRIORITY			5
#define TASK_LED_PRIORITY			6

/* Queue Definitions ------------------------------------------------------------*/
#define QUEUE_SIZE 16
PDU *pdu_queue_buffer[QUEUE_SIZE]; // Queue buffer (array of PDU pointers)

OS_EVENT *tx_queue; // Global queue pointer


/* Event Flags------------------------------------------------------------------ */
#define LED_EVENT   0x01  // Event flag for blink_task
#define ADC_EVENT   0x02  // Event flag for adc_task
#define BUTTON_EVENT 0x04  // Event flag for Button_task

// Event Object
OS_FLAG_GRP *event_flags;

/* Welcome Message -------------------------------------------------------------*/
// State variable to track the current state of the interaction
typedef enum {
    STATE_MAIN_MENU,
    STATE_LED_FREQUENCY,
    STATE_ADC_OFFSET
} InteractionState;

InteractionState current_state = STATE_MAIN_MENU;


const char *welcome_message = "Welcome! Please choose an option:\r\n"
                              "1. LED Blink\r\n"
                              "2. ADC Read\r\n"
                              "Enter your choice (1 or 2): ";
const char *led_frequency_message = "Enter LED blink frequency(ms): ";
const char *adc_offset_message = "Enter ADC offset: ";

/* Main Function -------------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  RetargetInit(&huart2);
  MX_ADC3_Init();
  OS_ERR err;

  // Initialize µC/OS-II
  OSInit();

  // Create Event Object
  event_flags = OSFlagCreate(0x00, &err); // Initial flags = 0

  // Create the queue
  tx_queue = OSQCreate((void **)&pdu_queue_buffer[0], QUEUE_SIZE);
  if (tx_queue == NULL) {
	  printf("Queue initialization failed\r\n");
	  Error_Handler();
  }
  //printf("Queue initialized at address: %p\r\n", tx_queue); // Debug print

  OSTaskCreateExt(TaskReceive,
    		  	  	  NULL,
    				  (void *)&Task_Receive_stk[TASK_STACKSIZE],
					  TASK_RECEIVE_PRIORITY,
					  TASK_RECEIVE_PRIORITY,
					  Task_Receive_stk,
    				  TASK_STACKSIZE,
    				  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskTransmit,
      		  	  	  NULL,
      				  (void *)&Task_Transmit_stk[TASK_STACKSIZE],
      				  TASK_TRANSMIT_PRIORITY,
      				  TASK_TRANSMIT_PRIORITY,
      				  Task_Transmit_stk,
      				  TASK_STACKSIZE,
      				  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskButton,
      		  	  	  NULL,
      				  (void *)&Task_Button_stk[TASK_STACKSIZE],
      				  TASK_BUTTON_PRIORITY,
      				  TASK_BUTTON_PRIORITY,
      				  Task_Button_stk,
      				  TASK_STACKSIZE,
      				  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskLED,
      		  	  	  NULL,
      				  (void *)&Task_LED_stk[TASK_STACKSIZE],
      				  TASK_LED_PRIORITY,
      				  TASK_LED_PRIORITY,
      				  Task_LED_stk,
      				  TASK_STACKSIZE,
      				  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskADC,
      		  	  	  NULL,
      				  (void *)&Task_ADC_stk[TASK_STACKSIZE],
      				  TASK_ADC_PRIORITY,
      				  TASK_ADC_PRIORITY,
      				  Task_ADC_stk,
      				  TASK_STACKSIZE,
      				  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSStart();
  return 0;
}

/* Definition of Task Transmit ----------------------------------------------------------- */
void TaskTransmit(void *pdata) {
	HAL_StatusTypeDef hal_status;
	PDU *pdu_rx;
	INT8U err;
	//printf("Task Transmit start\r\n");
    while (1) {
    	// Wait for a PDU from the queue
        pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
        if (err != OS_ERR_NONE) {
        	printf("transmit receive queue error\r\n");
			Error_Handler();
		}

        // Print the PDU contents
        printf("Transmit Task received a PDU:\r\n");
		printf("  Header: 0x%02X\r\n", pdu_rx->header);
		printf("  SID: 0x%02X\r\n", pdu_rx->sid);
		printf("  Payload: ");
		for (int i = 0; i < 6; i++) {
			printf("0x%02X ", pdu_rx->data[i]);
		}
		printf("\r\n");

		// Delay or wait for the next PDU
		OSTimeDlyHMSM(0, 0, 1, 0); // Delay for 1s

		// Handle negative responses
		if (pdu_rx->sid == SID_NEG_RESPONSE) {
			printf("Error: Negative response received\r\n");
		}

		// Transmit the PDU over UART
		hal_status = HAL_UART_Transmit(&huart2, (uint8_t*)pdu_rx, sizeof(PDU), HAL_MAX_DELAY);
		if (hal_status != HAL_OK) {
			Error_Handler(); 			// Call Error_Handler on failure
		}
    }
}

/* Definition of Task Receive ----------------------------------------------------------- */
void TaskReceive(void* pdata) {
	uint8_t rx_buffer[10] = {0}; // Buffer to store user input
	uint8_t rx_index = 0;
	uint8_t choice = 0;
	int16_t offset = 0;
	INT8U err;
	HAL_StatusTypeDef hal_status;
	PDU pdu_tx;

	while (1) {
		// Handle input based on current state
		switch (current_state) {
			case STATE_MAIN_MENU:
				OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1s
				printf("MAIN MENU start\r\n"); // Debug print
				// Send Welcome Message
				HAL_UART_Transmit(&huart2, (uint8_t*)welcome_message, strlen(welcome_message), HAL_MAX_DELAY);

				// Receive a single character
				hal_status = HAL_UART_Receive(&huart2, &rx_buffer[rx_index], 1, HAL_MAX_DELAY);
				if (hal_status != HAL_OK) {
					Error_Handler(); 			// Call Error_Handler on failure
				}

				OSTimeDlyHMSM(0, 0, 5, 0); // Delay for 1 second
				//printf("Received character: %c\r\n", rx_buffer[rx_index]); // Debug print

				// Process menu choice
				if (rx_buffer[rx_index] >= '0' && rx_buffer[rx_index] <= '9') {
					choice = rx_buffer[rx_index] - '0'; // Convert character to number
					printf("Parsed choice: %d\r\n", choice); // Debug print

					switch (choice) {
						case 1:
							// LED Control chosen
							current_state = STATE_LED_FREQUENCY;
							printf("State changed to STATE_LED_FREQUENCY\r\n"); // Debug print
							// Reset buffer and index
							rx_index = 0;
							memset(rx_buffer, 0, sizeof(rx_buffer));
							break;

						case 2:
							// ADC Reading chosen
							current_state = STATE_ADC_OFFSET;
							printf("State changed to STATE_ADC_OFFSET\r\n"); // Debug print
							// Reset buffer and index
							rx_index = 0;
							memset(rx_buffer, 0, sizeof(rx_buffer));
							// Display ADC offset options
							const char *adc_offset_options = "Choose ADC offset:\r\n"
															 "1. 0\r\n"
															 "2. 5\r\n"
															 "3. 10\r\n"
															 "Enter your choice (1, 2, or 3): ";
							hal_status = HAL_UART_Transmit(&huart2, (uint8_t*)adc_offset_options, strlen(adc_offset_options), HAL_MAX_DELAY);
							if (hal_status != HAL_OK) {
								Error_Handler();
							}
							break;

						default:
							// Invalid choice: Send negative response
							printf("Invalid choice\r\n"); // Debug print
							PDU neg_response_pdu = {PDU_HEADER, SID_NEG_RESPONSE, {0}};
							err = OSQPost(tx_queue, (void*)&neg_response_pdu);
							if (err != OS_ERR_NONE) {
								Error_Handler();
							}
							// Reset state and re-display welcome message
							current_state = STATE_MAIN_MENU;
							rx_index = 0;
							memset(rx_buffer, 0, sizeof(rx_buffer));
							break;
					}
				} else {
					// Invalid input: Send negative response
					printf("Invalid input\r\n"); // Debug print
					PDU neg_response_pdu = {PDU_HEADER, SID_NEG_RESPONSE, {0}};
					err = OSQPost(tx_queue, (void*)&neg_response_pdu);
					if (err != OS_ERR_NONE) {
						Error_Handler();
					}
					// Reset state and re-display welcome message
					current_state = STATE_MAIN_MENU;
					rx_index = 0;
					memset(rx_buffer, 0, sizeof(rx_buffer));
				}
				break;

			case STATE_LED_FREQUENCY:
				printf("Enter LED blink frequency (3 digits): \r\n");
				// Read 3 digits from the user
				for (rx_index = 0; rx_index < 3; rx_index++) {
					hal_status = HAL_UART_Receive(&huart2, &rx_buffer[rx_index], 1, HAL_MAX_DELAY);
					if (hal_status != HAL_OK) {
						Error_Handler();
					}

					// Echo the character back to the terminal
					HAL_UART_Transmit(&huart2, &rx_buffer[rx_index], 1, HAL_MAX_DELAY);

					// Validate input (must be a digit)
					if (rx_buffer[rx_index] < '0' || rx_buffer[rx_index] > '9') {
						printf("\r\nInvalid input. Please enter 3 digits.\r\n");

						// Invalid input: Send negative response
						PDU neg_response_pdu = {PDU_HEADER, SID_NEG_RESPONSE, {0}};
						err = OSQPost(tx_queue, (void*)&neg_response_pdu);
						if (err != OS_ERR_NONE) {
							printf("Error in sending neg response from receive LED frequency! ");
							Error_Handler();
						}

						current_state = STATE_MAIN_MENU; // Return to main menu
						rx_index = 0;
						memset(rx_buffer, 0, sizeof(rx_buffer));
						break;
					}
				}
				// If 3 digits were successfully entered
				if (rx_index == 3) {
					// Convert the 3-digit input to an integer
					uint16_t frequency = (rx_buffer[0] - '0') * 100 +
										 (rx_buffer[1] - '0') * 10 +
										 (rx_buffer[2] - '0');

					printf("\r\nFrequency set to: %d ms\r\n", frequency);
					// Set LED frequency
					pdu_tx.header = PDU_HEADER;
					//printf("Header added");
					pdu_tx.sid = SID_LED_BLINK;
					//printf("SID added");
					memset(pdu_tx.data, 0, sizeof(pdu_tx.data)); // Clear previous data
					//printf("Data cleared");
					memcpy(pdu_tx.data, &frequency, sizeof(frequency)); // Store frequency in PDU payload
					//printf("LED PDU prepared!\r\n");
					OSFlagPost(event_flags, LED_EVENT, OS_FLAG_SET, &err); // Trigger LED blink
					//printf("LED Flag Post! \r\n");
					if (err != OS_ERR_NONE) {
						Error_Handler();
					}

					// Send Positive Response
					err = OSQPost(tx_queue, (void *)&pdu_tx);
					if (err != OS_ERR_NONE) {
						printf("Error posting to queue: %d\r\n", err);
						Error_Handler();
					}
					//printf("Receive task Queue post no err\r\n");

					OSTimeDlyHMSM(0, 1, 0, 0);
					//printf("Post 1");
					// Reset state and buffer
					current_state = STATE_MAIN_MENU;
					rx_index = 0;
					memset(rx_buffer, 0, sizeof(rx_buffer));
					//printf("Returned to MAIN MENU\r\n"); // Debug print
				}
				break;

			case STATE_ADC_OFFSET:
				// Wait for user input
				hal_status = HAL_UART_Receive(&huart2, &rx_buffer[rx_index], 1, HAL_MAX_DELAY);
				if (hal_status != HAL_OK) {
					Error_Handler();
				}
				// Handle ADC offset choice
				if (rx_buffer[rx_index] >= '1' && rx_buffer[rx_index] <= '3') {
					choice = rx_buffer[rx_index] - '0'; // Convert character to number
					printf("Parsed ADC offset choice: %d\r\n", choice); // Debug print

					// Set offset based on choice
					switch (choice) {
						case 1:
							offset = 0;
							break;
						case 2:
							offset = 5;
							break;
						case 3:
							offset = 10;
							break;
					}
					// Set LED frequency
					pdu_tx.header = PDU_HEADER;
					//printf("Header added");
					pdu_tx.sid = SID_ADC_READ;
					//printf("SID added");
					memset(pdu_tx.data, 0, sizeof(pdu_tx.data)); // Clear previous data
					//printf("Data cleared");
					pdu_tx.data[0] = 2;  // Payload Type 2 (send ADC value)
					memcpy(&pdu_tx.data[1], &offset, sizeof(offset)); // Store offset in PDU payload
					//printf("Copy handled, sent offset = %d\r\n", offset);
					OSFlagPost(event_flags, ADC_EVENT, OS_FLAG_SET, &err); // Trigger ADC read
					if (err != OS_ERR_NONE) {
						Error_Handler();
					}
					//pdu_tx.data[0] = 0x01; // Example payload

					// Send Positive Response
					err = OSQPost(tx_queue, (void *)&pdu_tx);
					if (err != OS_ERR_NONE) {
						printf("Error posting to queue: %d\r\n", err);
						Error_Handler();
					}
					//printf("Receive task Queue post no err \r\n");
					OSTimeDlyHMSM(0, 1, 0, 0);
					//printf("Post 1");
					// Reset state and buffer
					current_state = STATE_MAIN_MENU;
					rx_index = 0;
					memset(rx_buffer, 0, sizeof(rx_buffer));
					//printf("Returned to MAIN MENU\r\n"); // Debug print
				} else {
					// Invalid input: Send negative response
					printf("Invalid input\r\n"); // Debug print
					PDU neg_response_pdu = {PDU_HEADER, SID_NEG_RESPONSE, {0}};
					err = OSQPost(tx_queue, (void*)&neg_response_pdu);
					if (err != OS_ERR_NONE) {
						Error_Handler();
					}
					// Reset state and re-display welcome message
					current_state = STATE_MAIN_MENU;
					rx_index = 0;
					memset(rx_buffer, 0, sizeof(rx_buffer));
				}
				break;
		}

		// Reset buffer and index
		rx_index = 0;
		memset(rx_buffer, 0, sizeof(rx_buffer));
	}
}
/* Button Event Task ----------------------------------------------------------------*/
void TaskButton(void *pdata) {
	PDU pdu_tx = {PDU_HEADER, SID_BUTTON_EVENT, {0}};
	INT8U err;
	OS_FLAGS flags;
	//printf("Task Button\r\n");

	while (1) {
		// Wait for BUTTON_EVENT (button press)
		flags = OSFlagPend(event_flags, BUTTON_EVENT, OS_FLAG_WAIT_SET_ALL, 0, &err);
		if (err != OS_ERR_NONE) {
			Error_Handler();
		}

		if (flags & BUTTON_EVENT) {
			// Process Event
			printf("Button: The push button is pressed!\r\n");
			OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

			pdu_tx.sid = SID_BUTTON_EVENT;

			// Send Button Response
			err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
			if (err != OS_ERR_NONE) {
				Error_Handler();
			}

			OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second
			// Prepare PDU sending Pos response
			pdu_tx.sid = SID_POS_RESPONSE;
			memset(pdu_tx.data, 0, sizeof(pdu_tx.data)); // Clear previous data
			//printf("Prepare PDU Positive response\r\n");

			// Send PDU
			err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
			if (err != OS_ERR_NONE) {
				printf("Error! Button Positive response task sends Queue\r\n");
				Error_Handler();
			}
			//printf("Button Positive response sends Queue\r\n");
			OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

			OSFlagPost(event_flags, BUTTON_EVENT, OS_FLAG_CLR, &err); // Manually clear the flag
			current_state = STATE_MAIN_MENU; // Return to main menu
			//printf("MAIN MENU start\r\n"); // Debug print
			// Send Welcome Message
			HAL_UART_Transmit(&huart2, (uint8_t*)welcome_message, strlen(welcome_message), HAL_MAX_DELAY);
		}
	}
}

/* LED_Blink Task -------------------------------------------------------------------*/
void TaskLED(void *pdata) {
    PDU pdu_tx = {PDU_HEADER, SID_LED_BLINK, {0}};
    PDU *pdu_rx;
    INT8U err;
    OS_FLAGS flags;
    uint16_t blink_frequency;
    //printf("Task LED\r\n");

    // LED Blink Function
    void LEDBlink(INT32U blink_frequency_ms) {
        INT32U start_time = OSTimeGet();
        INT32U current_time;
        INT32U total_duration_ms = 5000; // Blink for 5 seconds

        while (1) {
        	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            OSTimeDlyHMSM(0, 0, 0, blink_frequency_ms);

            current_time = OSTimeGet();
            if ((current_time - start_time) >= total_duration_ms) {
                break; // Exit loop if total duration has elapsed
            }
        }
        // Ensure the LED is off after blinking completes
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Turn LED off
    }

    while (1) {
        // Wait for LED_EVENT
        flags = OSFlagPend(event_flags, LED_EVENT, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
        printf("Pend happened LED\r\n");
        if (err != OS_ERR_NONE) {
			Error_Handler();			 // Call Error_Handler on failure
		}

        if (flags & LED_EVENT) {
        	printf("LED flag condition TRUE\r\n");
        	//Get the PDU pointer (address)
			//pdu_rx = &led_pdu;
        	// Wait for a PDU from the queue
			pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
			if (err != OS_ERR_NONE) {
				printf("LED receive queue error\r\n");
				Error_Handler();
			}
			//printf("LED received the queue\r\n");

			// Extract frequency from PDU payload
			memcpy(&blink_frequency, pdu_rx->data, sizeof(blink_frequency));
			printf("frequency = %d\r\n", blink_frequency);

            // Blink LED
			LEDBlink(blink_frequency);
			printf("Payane blink\r\n");
			pdu_tx.sid = SID_LED_BLINK;
			pdu_tx.data[0] = 0x01; // Example payload

            // Send LED Response
			err = OSQPost(tx_queue, (void *)&pdu_tx);
            if (err != OS_ERR_NONE) {
				printf("Error posting to queue: %d\r\n", err);
				Error_Handler();
			}
            //printf("LED Queue post no err\r\n");
            OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

            // Prepare PDU sending Pos response
			pdu_tx.sid = SID_POS_RESPONSE;
			memset(pdu_tx.data, 0, sizeof(pdu_tx.data)); // Clear previous data
			//printf("Prepare PDU Positive response\r\n");

			// Send PDU
			err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
			if (err != OS_ERR_NONE) {
				printf("Error! LED Positive response task sends Queue\r\n");
				Error_Handler();
			}
			//printf("LED Positive response sends Queue\r\n");
			OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

            OSFlagPost(event_flags, LED_EVENT, OS_FLAG_CLR, &err); // Manually clear the flag
            //printf("LED Clear flag\r\n");
        }
    }
}

/* ADC_Read Task -------------------------------------------------------------------*/
void TaskADC(void *pdata) {
    uint16_t adc_value;
    int16_t offset;
    OS_FLAGS flags;
    float temp;
    PDU pdu_tx;
    PDU *pdu_rx;
    INT8U err;
    //printf("Task ADC\r\n");

    while (1) {
        // Wait for ADC_EVENT
        flags = OSFlagPend(event_flags, ADC_EVENT, OS_FLAG_WAIT_SET_ALL, 0, &err);
        //printf("Pend happened ADC\r\n");
        if (err != OS_ERR_NONE) {
			Error_Handler();
		}

        if (flags & ADC_EVENT) {
        	printf("ADC flag condition TRUE\r\n");
            // Read ADC Value
        	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
            HAL_ADC_Start(&hadc3);
            HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc3);
            printf("ADC value measured: %d\r\n", adc_value);
            HAL_ADC_Stop(&hadc3);

            temp = ((float)adc_value) / 4095 * 3300;
            temp = ((temp - 760.0) / 2.5) + 25;
            printf("temp value measured = %f\r\n", temp);

            //Get the PDU from Queue
            pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
			if (err != OS_ERR_NONE) {
				printf("ADC receive queue error\r\n");
				Error_Handler();
			}
			//printf("ADC received the queue\r\n");

			// Extract frequency from PDU payload
			memcpy(&offset, &pdu_rx->data[1], sizeof(offset));
			printf("Offset = %d\r\n", offset);
			temp = temp + offset;
			printf("temp + offset = %f\r\n", temp);

			// Prepare PDU with ADC Value
            pdu_tx.header = PDU_HEADER;
            pdu_tx.sid = SID_ADC_READ;
            pdu_tx.data[0] = 2;  // Payload Type 2 (send ADC value)

            memcpy(&pdu_tx.data[1], &temp, sizeof(temp));  // Store temp in payload
            //printf("ADC task make PDU\r\n");
            OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second
            // Send PDU
            err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
            if (err != OS_ERR_NONE) {
            	printf("Error! ADC task sends Queue\r\n");
				Error_Handler();
			}
            //printf("ADC task sends Queue\r\n");
            OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

            // Prepare PDU sending Pos response
			pdu_tx.sid = SID_POS_RESPONSE;
			memset(pdu_tx.data, 0, sizeof(pdu_tx.data)); // Clear previous data
			//printf("Prepare PDU Positive response\r\n");

			// Send PDU
			err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
			if (err != OS_ERR_NONE) {
				printf("Error! ADC Positive response task sends Queue\r\n");
				Error_Handler();
			}
			//printf("ADC Positive response sends Queue\r\n");
			OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second

            OSFlagPost(event_flags, ADC_EVENT, OS_FLAG_CLR, &err); // Manually clear the flag
            OSTimeDlyHMSM(0, 0, 10, 0); // Delay for 1 second
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    OS_ERR err;
    if(GPIO_Pin == GPIO_PIN_13)
    	OSFlagPost(event_flags, BUTTON_EVENT, OS_FLAG_SET, &err);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 9;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
