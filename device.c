/*****************************************************************************/
/*                                                                           */
/* сулс - система управления лафетными стволами                              */
/*  Авторское Право (C) <2015> <Кузьмин Ярослав>                             */
/*                                                                           */
/*  Эта программа является свободным программным обеспечением:               */
/*  вы можете распространять и/или изменять это в соответствии с             */
/*  условиями в GNU General Public License, опубликованной                   */
/*  Фондом свободного программного обеспечения, как версии 3 лицензии,       */
/*  или (по вашему выбору) любой более поздней версии.                       */
/*                                                                           */
/*  Эта программа распространяется в надежде, что она будет полезной,        */
/*  но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемой гарантии            */
/*  Или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНЫХ ЦЕЛЕЙ. См GNU General Public License      */
/*  для более подробной информации.                                          */
/*                                                                           */
/*  Вы должны были получить копию GNU General Public License                 */
/*  вместе с этой программой. Если нет, см <http://www.gnu.org/licenses/>    */
/*                                                                           */
/*  Адрес для контактов: kuzmin.yaroslav@gmail.com                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* scfm - system control fire monitors                                       */
/*  Copyright (C) <2015> <Kuzmin Yaroslav>                                   */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/*  Email contact: kuzmin.yaroslav@gmail.com                                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/

#include "pub.h"
#include "common.h"
#include "modbus.h"

/*****************************************************************************/
/*    Общие переменые                                                        */
/*****************************************************************************/

/*****************************************************************************/
/*    Локальные функции                                                      */
/*****************************************************************************/

#define REG_D300            0
#define AMOUNT_BIT_D300     16

#define REG_D301            1
#define AMOUNT_BIT_D301     16

#define REG_D302            2
#define AMOUNT_BIT_D302     16

#define REG_D303            3
#define AMOUNT_BIT_D303     5

#define REG_D304            4
#define AMOUNT_BIT_D304     4

#define REG_D305            5
#define AMOUNT_BIT_D305     3

#define REG_D306            6
#define AMOUNT_BIT_D306     16

#define REG_D307            7
#define AMOUNT_BIT_D307     16

#define AMOUNT_BIT_CONFIG   (AMOUNT_BIT_D300 + AMOUNT_BIT_D301 + AMOUNT_BIT_D302\
                            +AMOUNT_BIT_D303 + AMOUNT_BIT_D304 + AMOUNT_BIT_D305\
                            +AMOUNT_BIT_D306 + AMOUNT_BIT_D307)
#if !(AMOUNT_BIT_CONFIG > 64)
#error Увеличить колличество бит
#endif

static int set_config_controller(uint16_t * reg,config_controller_s * config)
{
	uint64_t temp;
	uint32_t type;
	uint64_t flag;

	type = reg[REG_D300];
	type <<= AMOUNT_BIT_D300;
	type += reg[REG_D301];

	flag = reg[REG_D302];

	temp = reg[REG_D303];
	temp <<= AMOUNT_BIT_D302;
	flag += temp;

	temp = reg[REG_D304];
	temp <<= (AMOUNT_BIT_D302 + AMOUNT_BIT_D303);
	flag += temp;

	temp = reg[REG_D305];
	temp <<= (AMOUNT_BIT_D302 + AMOUNT_BIT_D303 +AMOUNT_BIT_D304);
	flag += temp;

	temp = reg[REG_D306];
	temp <<= (AMOUNT_BIT_D302 + AMOUNT_BIT_D303 + AMOUNT_BIT_D304 + AMOUNT_BIT_D305);
	flag += temp;

	temp = reg[REG_D307];
	temp <<= (AMOUNT_BIT_D302 + AMOUNT_BIT_D303 + AMOUNT_BIT_D304 + AMOUNT_BIT_D305 + AMOUNT_BIT_D306);
	flag += temp;

	config->type = type;
	config->flag = flag;

	return SUCCESS;
}

#define REG_D100    0
#define REG_D101    1
#define REG_D102    2
#define REG_D103    3
#define REG_D104    4
#define REG_D105    5
#define REG_D106    6
#define REG_D107    7
#define REG_D108    8
#define REG_D109    9
#define REG_D110   10
#define REG_D111   11
#define REG_D112   12
#define REG_D113   13
#define REG_D114   14
#define REG_D115   15
#define REG_D116   16
#define REG_D117   17

static int set_state_controller(uint16_t * dest,state_controller_s *state)
{
	state->lafet               = dest[REG_D100];
	state->tic_vertical        = dest[REG_D101];
	state->tic_horizontal      = dest[REG_D102];
	state->encoder_vertical    = dest[REG_D103];
	state->encoder_horizontal  = dest[REG_D104];
	state->pressure            = dest[REG_D105];
	state->amperage_vertical   = dest[REG_D106];
	state->amperage_horizontal = dest[REG_D107];
   state->valve               = dest[REG_D110];
   state->tic_valve           = dest[REG_D111];
   state->fire_sensor         = dest[REG_D114];
   state->fire_alarm          = dest[REG_D116];

	return SUCCESS;
}

static int connect_tcp(link_s * link)
{
	int rc;
	char * address = link->address;
	uint16_t port = link->port;
	uint8_t id = link->id;
	modbus_t *ctx;

	ctx = modbus_new_tcp(address,port);
	if (ctx == NULL) {
		return FAILURE;
	}

	modbus_set_error_recovery(ctx,(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));

	modbus_set_slave(ctx,id);

	rc = modbus_connect(ctx);
	if(rc == -1){
		g_warning("Несмог подключится к : %d",id);
		modbus_free(ctx);
		return FAILURE;
	}
	link->connect = ctx;
	link->dest = g_slice_alloc0(MODBUS_TCP_MAX_ADU_LENGTH);
	return SUCCESS;
}

static int connect_uart(link_s * link)
{
	int rc;
	char * device = link->device;
	uint32_t baud = link->baud;
	int8_t parity = link->parity;
	uint8_t data_bit = link->data_bit;
	uint8_t stop_bit = link->stop_bit;
	uint8_t id = link->id;
	modbus_t *ctx;

	ctx = modbus_new_rtu(device,baud,parity,data_bit,stop_bit);
	if (ctx == NULL) {
		return FAILURE;
	}

	modbus_set_error_recovery(ctx,(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));

	modbus_set_slave(ctx,id);

	rc = modbus_connect(ctx);
	if(rc == -1){
		g_warning("Несмог подключится к : %d",id);
		modbus_free(ctx);
		return FAILURE;
	}
	link->connect = ctx;
	link->dest = g_slice_alloc0(MODBUS_RTU_MAX_ADU_LENGTH);
	return SUCCESS;
}
/*****************************************************************************/
/*    Общие функции                                                          */
/*****************************************************************************/

int link_disconnect_controller(link_s * link)
{
	uint16_t * dest = link->dest;
	modbus_t * ctx = (modbus_t*)link->connect;
	if(ctx != NULL){
    	modbus_close(ctx);
		modbus_free(ctx);
		g_slice_free1(MODBUS_TCP_MAX_ADU_LENGTH,dest);
	}
	link->connect = NULL;

	return SUCCESS;
}

int check_config_controller(config_controller_s * config_c,config_controller_s * config_d)
{
	if(config_c->type != config_d->type){
		return FAILURE;
	}
	if(config_c->flag != config_d->flag){
		return FAILURE;
	}
	return SUCCESS;
}


char * get_name_controller(config_controller_s * config)
{
	uint16_t name_robot;
	uint16_t liter_robot;
	uint32_t type = config->type;

	name_robot = type >> AMOUNT_BIT_D300;
	/*Регистр D301 литраж установки */
	liter_robot = type;

	/*Регистр D300 названия установки*/
	switch(name_robot){
		case 1:
			g_string_printf(pub,"ЛСД-С%04dУ",liter_robot);
			break;
		case 2:
			g_string_printf(pub,"ЛСД-С%04dУ-Ех",liter_robot);
			break;
		case 3:
			g_string_printf(pub,"ЛСД-С%04dУ-ИК",liter_robot);
			break;
		case 4:
			g_string_printf(pub,"ЛСД-С%04dУ-Ех-ИК",liter_robot);
			break;
		case 5:
			g_string_printf(pub,"ЛСД-С%04dУ-ТВ",liter_robot);
			break;
		case 6:
			g_string_printf(pub,"ЛСД-С%04dУ-Ех-ТВ",liter_robot);
			break;
		case 7:
			g_string_printf(pub,"ПР-ЛСД-С%04dУ-ИК-ТВ",liter_robot);
			break;
		case 8:
			g_string_printf(pub,"ПР-ЛСД-С%04dУ-Ех-ИК-ТВ",liter_robot);
			break;
		default:
			g_string_printf(pub,"ЛСД-%04d",liter_robot);
			break;
	}
	return g_strdup(pub->str);
}

static uint16_t reg_D100 = 0x1064;
/*static uint16_t reg_D101 = 0x1065;*/
/*static uint16_t reg_D102 = 0x1066;*/
/*static uint16_t reg_D103 = 0x1067;*/
/*static uint16_t reg_D104 = 0x1068;*/
/*static uint16_t reg_D105 = 0x1069;*/
/*static uint16_t reg_D106 = 0x106A;*/
/*static uint16_t reg_D107 = 0x106B;*/
/*static uint16_t reg_D108 = 0x106C;*/
/*static uint16_t reg_D109 = 0x106D;*/
/*static uint16_t reg_D110 = 0x106E;*/
/*static uint16_t reg_D111 = 0x106F;*/
/*static uint16_t reg_D112 = 0x1070;*/
/*static uint16_t reg_D113 = 0x1071;*/
/*static uint16_t reg_D114 = 0x1072;*/
/*static uint16_t reg_D115 = 0x1073;*/
/*static uint16_t reg_D116 = 0x1074;*/
/*static uint16_t reg_D117 = 0x1075;*/
#define AMOUNT_STATE_REGISTER    18

/*считать состояние*/
int link_state_controller(link_s * link,state_controller_s * state)
{
	int rc;
	uint16_t * dest = link->dest;
	modbus_t * ctx = (modbus_t*)link->connect;

	if(ctx == NULL){
		return FAILURE;
	}
	rc = modbus_read_registers(ctx,reg_D100,AMOUNT_STATE_REGISTER,dest);
	if(rc == -1){
		link_disconnect_controller(link);
		return FAILURE;
	}
	set_state_controller(dest,state);
	return SUCCESS;
}

static uint16_t reg_D300 = 0x112C;
/*static uint16_t reg_D301 = 0x112D;*/
/*static uint16_t reg_D302 = 0x112E;*/
/*static uint16_t reg_D303 = 0x112F;*/
/*static uint16_t reg_D304 = 0x1130;*/
/*static uint16_t reg_D305 = 0x1131;*/
/*static uint16_t reg_D306 = 0x1132;*/
/*static uint16_t reg_D307 = 0x1133;*/
#define AMOUNT_CONFIG_REGISTER    8

/*считать конфигурацию*/
int link_config_controller(link_s * link,config_controller_s * config)
{
	int rc;
	uint16_t * dest = link->dest;
	modbus_t * ctx = (modbus_t*)link->connect;

	if(ctx == NULL){
		return FAILURE;
	}
	rc = modbus_read_registers(ctx,reg_D300,AMOUNT_CONFIG_REGISTER,dest);
	if(rc == -1){
		link_disconnect_controller(link);
		return FAILURE;
	}
	set_config_controller(dest,config);

	return SUCCESS;
}

int link_connect_controller(link_s * link)
{
	int rc;
	switch(link->type){
		case TYPE_LINK_TCP:
			rc = connect_tcp(link);
			break;
		case TYPE_LINK_UART:
			rc = connect_uart(link);
			break;
		default:
			rc = FAILURE;
			break;
	}
	return rc;
}

/*****************************************************************************/