/*
 * DHT_Settings.h
 *
 * Created: 19/04/2024 18:59:39
 *  Author: Zinj
 */ 

#ifndef DHT_SETTINGS_H_INCLUDED
#define DHT_SETTINGS_H_INCLUDED
/*
||
||  Filename:	 		DHT_Settings.h
||  Title: 			    DHTxx Driver Settings
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	Settings for the DHTxx driver. Pick a model
||	and the desirable pin.
||
*/

//----- Configuration --------------------------//
#define DHT_Type	DHT11          //DHT11 or DHT22
#define DHT_Pin		A, 7
//----------------------------------------------//
#endif /* DHT_SETTINGS._H_ */