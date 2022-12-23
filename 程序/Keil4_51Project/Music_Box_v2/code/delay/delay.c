#include "delay/delay.h"

#if defined(USE_STC89C52RC)		/* 12T 51单片机 */
	#if MAIN_FOSC == 22118400L
		void Delay1ms()		//@22.1184MHz
		{
			unsigned char i, j;

			_nop_();
			i = 4;
			j = 146;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@22.1184MHz
		{
			unsigned char i, j, k;

			_nop_();
			i = 2;
			j = 103;
			k = 147;
			do
			{
				do
				{
					while (--k);
				} while (--j);
			} while (--i);
		}
	#elif MAIN_FOSC == 12000000L
		void Delay1ms()		//@12.000MHz
		{
			unsigned char i, j;

			i = 2;
			j = 239;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@12.000MHz
		{
			unsigned char i, j;

			i = 195;
			j = 138;
			do
			{
				while (--j);
			} while (--i);
		}
	#elif MAIN_FOSC == 11059200L
		void Delay1ms()		//@11.0592MHz
		{
			unsigned char i, j;

			_nop_();
			i = 2;
			j = 199;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@11.0592MHz
		{
			unsigned char i, j;

			i = 180;
			j = 73;
			do
			{
				while (--j);
			} while (--i);
		}
	#endif
#elif defined(USE_STC12C5A60S2)	/* 1T 51单片机 */
	#if MAIN_FOSC == 22118400L
		void Delay1ms()		//@22.1184MHz
		{
			unsigned char i, j;

			i = 22;
			j = 128;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@22.1184MHz
		{
			unsigned char i, j, k;

			i = 9;
			j = 104;
			k = 139;
			do
			{
				do
				{
					while (--k);
				} while (--j);
			} while (--i);
		}
	#elif MAIN_FOSC == 12000000L
		void Delay1ms()		//@12.000MHz
		{
			unsigned char i, j;

			_nop_();
			_nop_();
			i = 12;
			j = 168;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@12.000MHz
		{
			unsigned char i, j, k;

			i = 5;
			j = 144;
			k = 71;
			do
			{
				do
				{
					while (--k);
				} while (--j);
			} while (--i);
		}
	#elif MAIN_FOSC == 11059200L
		void Delay1ms()		//@11.0592MHz
		{
			unsigned char i, j;

			_nop_();
			i = 11;
			j = 190;
			do
			{
				while (--j);
			} while (--i);
		}
		void Delay100ms()		//@11.0592MHz
		{
			unsigned char i, j, k;

			i = 5;
			j = 52;
			k = 195;
			do
			{
				do
				{
					while (--k);
				} while (--j);
			} while (--i);
		}
			#endif
#endif

