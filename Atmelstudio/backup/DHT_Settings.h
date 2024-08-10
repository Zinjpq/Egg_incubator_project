/*
 * DHT_Settings.h
 *
 * Created: 19/04/2024 15:44:56
 *  Author: Zinj
 */ 


#ifndef DHT_SETTINGS_H_
#define DHT_SETTINGS_H_
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
#define DHT_Pin		A, 0
//----------------------------------------------//
#endif /* DHT_SETTINGS_H_ */