/*
 * DIO.c
 *
 * Created: 11/1/2019 5:06:26 PM
 *  Author: A.Elkady
 */ 

/*
*	Header files inclusions
*/
#include "DIO.h"

/**
 * \brief  DIO_Pin_Write : Making a certain pin LOW or HIGH 
 *
 * \param  u8Pin_number (Input) : The micro controller pin number (A0 ... D7)
 * \param  u8Value (Input) : LOW or HIGH (Macros) (If other values except for HIGH or LOW is entered the function does nothing)
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/   
extern Error DIO_Pin_Write(uint8_t u8Pin_number , uint8_t u8Value)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK ;

	/*
	*	Making sure that the input pin number is in range
	*/
	if (u8Pin_number < MAX_PINS) 
	{
		if (u8Value == LOW) 
		{
			if(u8Pin_number >= 0u && u8Pin_number < 8u)     
			{
				PORT_A &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if (u8Pin_number >= 8u && u8Pin_number < 16u)    
			{
				PORT_B &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 16u && u8Pin_number < 24u)    
			{
				PORT_C &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 24u && u8Pin_number < 32u)    
			{
				PORT_D &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else
			{
				/* Do nothing as this case is already handled */
			}
		}
		else if (u8Value == HIGH)
		{
			if(u8Pin_number >= 0u && u8Pin_number < 8u)     
			{
				PORT_A |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if (u8Pin_number >= 8u && u8Pin_number < 16u)    
			{
				PORT_B |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 16u && u8Pin_number < 24u)    
			{
				PORT_C |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 24u && u8Pin_number < 32u)    
			{
				PORT_D |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else
			{
				/* Do nothing as this case is already handled */
			}
		}
		else
		{
			/*Handling of entering wrong state*/
			u8Execution_indication = DIO_E_NEITHER_LOW_NOR_HIGH ;
		}
	}
	else
	{
		/*Handling of entering wrong pin number*/
		u8Execution_indication = DIO_E_WRONG_PIN_NUMBER ;
	}
	return u8Execution_indication ;
}

/** 
 * \brief DIO_Pin_Init : Making a certain pin Input or OUTPUT
 *
 * \param u8Pin_number (Input) : The micro controller pin number (A0 ... D7)
 * \param u8State (Input) : Determine the state of the chosen pin whether it's INPUT or OUTPUT (macros) 
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/ 
extern Error DIO_Pin_Init(uint8_t u8Pin_number , uint8_t u8State)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK;

	/*
	*	Making sure that the input pin number is in range
	*/
	if (u8Pin_number < MAX_PINS) 
	{
		if (u8State == INPUT)
		{
			if(u8Pin_number >= 0u && u8Pin_number < 8u)     
			{
				DDR_A &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 8u && u8Pin_number < 16u)    
			{
				DDR_B &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 16u && u8Pin_number < 24u)   
			{
				DDR_C &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 24u && u8Pin_number < 32u)   
			{
				DDR_D &=  ~(1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else
			{
				/* Do nothing as this case is already handled */
			}
		}
		else if (u8State == OUTPUT)
		{
			if(u8Pin_number >= 0u && u8Pin_number < 8u)     
			{
				DDR_A |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 8u && u8Pin_number < 16u)    
			{
				DDR_B |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 16u && u8Pin_number < 24u)   
			{
				DDR_C |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 24u && u8Pin_number < 32u)   
			{
				DDR_D |=   (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else
			{
				/* Do nothing as this case is already handled */
			}
		}
		else
		{
			/*Handling of entering wrong state*/
			u8Execution_indication = DIO_E_NEITHER_INPUT_NOR_OUTPUT ;
		}
	}
	else
	{
		/*Handling of entering wrong pin number*/
		u8Execution_indication = DIO_E_WRONG_PIN_NUMBER ;
		
	}
	return u8Execution_indication ;
}

/** 
 * \brief  DIO_Pin_Read : Reading a certain pin if it's LOW or HIGH and put the vale in the passed address
 *
 * \param  u8Pin_number (Input) : The micro controller pin number (A0 ... D7)
 * \param  *pu8Reading (OUTPUT) : The pin state will be stored in the space that this pointer points to
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/ 
extern Error DIO_Pin_Read(uint8_t u8Pin_number , uint8_t *pu8Reading)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK ;

	uint8_t u8Reading = 0 ; /*A temp variable to store the pin reading in*/
	
	/*
	*	Making sure that the input pin number is in range
	*/
	if (u8Pin_number < MAX_PINS)
	{
		if (0 != pu8Reading)
		{
			if(u8Pin_number >= 0 && u8Pin_number < 8u)    
			{
				u8Reading = PIN_A & (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 8u && u8Pin_number < 16u)   
			{
				u8Reading = PIN_B & (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 16u && u8Pin_number < 24u)  
			{
				u8Reading = PIN_C & (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else if(u8Pin_number >= 24u && u8Pin_number < 32u)  
			{
				u8Reading = PIN_D & (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
			}
			else
			{
				/* Do nothing as this case is already handled */
			}
			
			if (0 != u8Reading )
			{
				*pu8Reading = 1u ;
			} 
			else
			{
				*pu8Reading = 0 ;
			}
		}
		else
		{
			/*Handling of entering void pointer (pu8Reading)*/
			u8Execution_indication = DIO_E_NULL_POINTER ;
		}
	}
	else
	{
		/*Handling of entering wrong pin number*/
		u8Execution_indication = DIO_E_WRONG_PIN_NUMBER ;
	}
	return u8Execution_indication ;
}

/** 
 * \brief  DIO_Port_Write : Making a certain port LOW or HIGH 
 *
 * \param  u8Port_index (Input) : The micro controller port number (A , B , C , D)
 * \param  u8Value (Input) : Writes a certain value to the port
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/	
extern Error DIO_Port_Write(uint8_t u8Port_index , uint8_t u8Value)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK ;

	if(u8Port_index < MAX_PORTS)
	{
		u8Port_index =  u8Value ;
	}
	else
	{
		/*Handling of entering wrong port index*/
		u8Execution_indication = DIO_E_WRONG_PORT_NUMBER ;
	}
	return u8Execution_indication ;
}

/** 
 * \brief DIO_Pin_Activate_pull_up : Activating the pull up resistor of a certain input pin (The chosen pin has to be an input or the function will be meaningless)
 *
 * \param u8Pin_number (Input) : The micro controller pin number (A0 ... D7)
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/ 
extern Error DIO_Pin_Activate_pull_up(uint8_t u8Pin_number)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK ;

	u8Execution_indication = DIO_Pin_Write(u8Pin_number , HIGH) ;
	
	if (DIO_E_OK != u8Execution_indication)
	{
		u8Execution_indication = DIO_E_PULL_UP_NOT_OK ;
	}
	else
	{
		
	}
	
	return u8Execution_indication ;
}

/** 
 * \brief DIO_Port_Activate_pull_up : Activating the pull up resistor of a certain input port (The chosen port has to be an input or the function will be meaningless)
 *
 * \param u8Port_index (Input) : The micro controller port number (A , B , C ,D)
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/ 
extern Error DIO_Port_Activate_pull_up(uint8_t u8Port_index)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = 0 ;

	u8Execution_indication = DIO_Port_Write(u8Port_index , MAX);
	
	if (DIO_E_OK != u8Execution_indication)
	{
		u8Execution_indication = DIO_E_PULL_UP_NOT_OK ;
	}
	else
	{
		
	}
	
	return u8Execution_indication ;
}


/** 
 * \brief DIO_Pin_Toggle : A function that toggle a particular pin ( It has to be output )
 *
 * \param u8Pin_index : The micro controller pin number (A0 ... D7) 
 * \return uint8_t : The return of this function will be an indication of the execution
 *
 **/     
extern Error DIO_Pin_Toggle(uint8_t u8Pin_number) 
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	Error u8Execution_indication  = DIO_E_OK ;

	/*
	*	Making sure that the input pin number is in range
	*/
	if (u8Pin_number < MAX_PINS) 
	{
		if(u8Pin_number >= 0 && u8Pin_number < 8u)     
		{
			PORT_A ^=  (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
		}
		else if (u8Pin_number >= 8u && u8Pin_number < 16u)    
		{
			PORT_B ^=  (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
		}
		else if(u8Pin_number >= 16u && u8Pin_number < 24u)    
		{
			PORT_C ^=  (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
		}
		else if(u8Pin_number >= 24u && u8Pin_number < 32u)    
		{
			PORT_D ^=  (1u << (u8Pin_number%BITS_PER_REGISTER)) ;
		}
		else
		{
			/* Do nothing as this case is already handled */
		}
	}
	else
	{
		/*Handling of entering wrong pin number*/
		u8Execution_indication = DIO_E_WRONG_PIN_NUMBER ;
	}
	return u8Execution_indication ;
}