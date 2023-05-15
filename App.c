/*************************************************
*  FILE  DESCRIPTION
*------------------------------------------------
*     File: App.c
*     Description: Contains source code for the calculator
*
****************************************************/

#include "std_types.h"
#include "util/delay.h"
#include "BIT_MATH.h"
#include "Dio_interface.h"
#include "KPAD.h"
#include "LCD_interface.h"
#include "EXTI_interface.h"




#define REESOLUTION    6

u16 Scan_Input();
u16 Reading_Function();
void Display_result(u16 Displayed_Value);
u8 Operation_Fn();
void Initializations();

u8 Operation ;
u8 Operation2 ;

u16 Opr1;
u16 Opr2;
u16 Result = 0;
u16 Temp_result ;
u16 Resolution_Array[REESOLUTION] = {1, 10, 100, 1000, 10000};
u16 Result_Array[REESOLUTION];
u16 arr[50] = {0,0,0,1};

u16 operation_counter = 1;
u8 first_M = 0;

void array_mul(u8 Resolution)
/*This function reverse the order of numbers taken from Keypad*/
{
	Result = 0;
	for(u8 o = 0; o < Resolution ; o++)
	{
		Result = Result + (Result_Array[Resolution-o-1] * Resolution_Array[o]);
	}
}

int main(void)
{
	u8 counter=0;


	Initializations();

	while(1){

		do{
			arr[counter] = Reading_Function();
			counter++ ;
			arr[counter] = Operation ;
			counter++;
		}while(Operation != '=');

		Operation_Fn();

	}//end while
}//end main
/*************************************************************************************************
 **************************************************************************************************/
u16 Scan_Input(){

	s8 Local_input= 0;

	Local_input = KPAD_Read() - 0x30 ;

	return Local_input;
}

/*************************************************************************************************
 **************************************************************************************************/
u16 Reading_Function(){

	u16 input = 0;
	u8 counter = 0;
	Result = 0;

	do{
		input = KPAD_Read() - 0x30 ;

		Operation = input + 0x30;
		if( ((Operation == '+')||(Operation == '-')||(Operation == '*')||
			(Operation == '/')||(Operation == '=')||(Operation == 'c') ) ) {

			if( (Operation == '=')||(Operation == 'c')) break; // prevents it from displaying the = sign on LCD
			LCD_voidSendData(input+0x30); break;
		}
		LCD_voidSendData(input+0x30);

		Result_Array[counter] = input;

		counter++;


	}while( 1 );

	array_mul(counter);

	return Result;
}
/*************************************************************************************************
 **************************************************************************************************/
void Display_result(u16 Displayed_Value){
	u8 Local_resolution = 0;
	u16 temp = Displayed_Value;
	while(temp != 0 )
	{
		if( (temp = temp/10)==0)  break;
		Local_resolution++;
	}

	temp = Local_resolution;
	Local_resolution = 0;

	while(Local_resolution <= temp )
	{

		Result_Array[temp - Local_resolution] = (  Displayed_Value - ( (Displayed_Value/10 )*10 )  );
		Displayed_Value = (Displayed_Value/10 );
		Local_resolution++;
	}

	Local_resolution = 0;

	for(int i = 0; i <= temp ;i++)
	{
		LCD_voidSendData(Result_Array[i] + '0');
	}


}
/*************************************************************************************************
 **************************************************************************************************/
u8 Operation_Fn(){

	u8 Local_Check = 0;
	LCD_voidGotoxy(1,0);
	for(int i = 0; i < 50;i+=4)
	{

		if( arr[operation_counter] == '+')
		{
			if(first_M == 0){Temp_result = arr[i] + arr[i+2] ; first_M = 1;}
			else { Temp_result = Temp_result + arr[i] ;}
			operation_counter +=2;

			//break;
		}
		else if( arr[operation_counter] == '-')
		{
			if(first_M == 0){Temp_result = arr[i] - arr[i+2] ; first_M = 1;}
			else {Temp_result =  Temp_result - arr[i]    ;}
			operation_counter +=2;

			//break;
		}
		else if( arr[operation_counter] == '*')
		{
			if(first_M == 0){Temp_result = arr[i] * arr[i+2] ; first_M = 1;}
			else {Temp_result =  Temp_result * arr[i]    ;}
			operation_counter +=2;

			//break;
		}
		else if( arr[operation_counter] == '/')
		{
			if(first_M == 0){Temp_result = arr[i] / arr[i+2] ; first_M = 1;}
			else {Temp_result =  Temp_result / arr[i]    ;}
			operation_counter +=2;

			//break;
		}

		if( arr[operation_counter] == '=' )
		{
			LCD_voidSendString("= ");
			Display_result(Temp_result);
			DIO_voidSetPortValue(DIO_u8PORT_B, Temp_result);

			operation_counter  = 1;
			first_M = 0;
			break;

		}
	}
	return Local_Check;
}
/*************************************************************************************************
 **************************************************************************************************/
void Initializations()
{

	KPAD_Init();
	LCD_voidInit();
	DIO_voidSetPortDirection(DIO_u8PORT_B, 0xff);
	//EXTI_u8INT1SetCallback( Clear_Calculator );
	//EXTI1_voidInit(EXTI_RISING_EDGE);
}

