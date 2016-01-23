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
#include <gio/gio.h>

#include "pub.h"
#include "common.h"
#include "database.h"
#include "device.h"

/*****************************************************************************/
/*    Общие переменые                                                        */
/*****************************************************************************/

#define ALL_BIT                        0xFFFFFFFFFFFFFFFFL
#define BIT_ENGINE_VERTICAL            0x0000000000000001L
#define BIT_ENGINE_HORIZONTAL          0x0000000000000002L
#define BIT_ACTUATOR_SPRAY             0x0000000000000004L
#define BIT_ACTUATOR_RATE              0x0000000000000008L
#define BIT_ACTUATOR_VEIL              0x0000000000000010L
#define BIT_LIMIT_VERTICAL             0x0000000000000020L
#define BIT_TIC_VERTICAL               0x0000000000000040L
#define BIT_ENCODER_VERTICAL           0x0000000000000080L
#define BIT_AMPERAGE_VERTICAL          0x0000000000000100L
#define BIT_LIMIT_HORIZONTAL           0x0000000000000200L
#define BIT_TIC_HORIZONTAL             0x0000000000000400L
#define BIT_ENCODER_HORIZONTAL         0x0000000000000800L
#define BIT_AMPERAGE_HORIZONTAL        0x0000000000001000L
#define BIT_CONSOLE_SINGLY             0x0000000000002000L
#define BIT_CONSOLE                    0x0000000000004000L
#define BIT_POST                       0x0000000000008000L
#define BIT_SENSOR_FIRE_DRY            0x0000000000010000L
#define BIT_SENSOR_FIRE_485            0x0000000000020000L
#define BIT_SENSOR_FIRE_ETHERNET       0x0000000000040000L
#define BIT_SENSOR_DRY_485             0x0000000000080000L
#define BIT_SENSOR_DRY_ETHERNER        0x0000000000100000L
#define BIT_VALVE_DRY                  0x0000000000200000L
#define BIT_VALVE_ANALOG               0x0000000000400000L
#define BIT_VALVE_LIMIT                0x0000000000800000L
#define BIT_VALVE_FEEDBACK             0x0000000001000000L
#define BIT_CAM_ANALOG_DC              0x0000000002000000L
#define BIT_CAM_DIGITAL_DC             0x0000000004000000L
#define BIT_CAM_DIGITAL_POE            0x0000000008000000L
#define BIT_FIRE_ALARM_01              0x0000000010000000L
#define BIT_FIRE_ALARM_02              0x0000000020000000L
#define BIT_FIRE_ALARM_03              0x0000000040000000L
#define BIT_FIRE_ALARM_04              0x0000000080000000L
#define BIT_FIRE_ALARM_05              0x0000000100000000L
#define BIT_FIRE_ALARM_06              0x0000000200000000L
#define BIT_FIRE_ALARM_07              0x0000000400000000L
#define BIT_FIRE_ALARM_08              0x0000000800000000L
#define BIT_FIRE_ALARM_09              0x0000001000000000L
#define BIT_FIRE_ALARM_10              0x0000002000000000L
#define BIT_FIRE_ALARM_11              0x0000004000000000L
#define BIT_FIRE_ALARM_12              0x0000008000000000L
#define BIT_FIRE_ALARM_13              0x0000010000000000L
#define BIT_FIRE_ALARM_14              0x0000020000000000L
#define BIT_FIRE_ALARM_15              0x0000040000000000L
#define BIT_FIRE_ALARM_16              0x0000080000000000L
#define BIT_DEVICE_01_STATE_0          0x0000100000000000L
#define BIT_DEVICE_01_STATE_1          0x0000200000000000L
#define BIT_DEVICE_02_STATE_0          0x0000400000000000L
#define BIT_DEVICE_02_STATE_1          0x0000800000000000L
#define BIT_DEVICE_03_STATE_0          0x0001000000000000L
#define BIT_DEVICE_03_STATE_1          0x0002000000000000L
#define BIT_DEVICE_04_STATE_0          0x0004000000000000L
#define BIT_DEVICE_04_STATE_1          0x0008000000000000L
#define BIT_DEVICE_05_STATE_0          0x0010000000000000L
#define BIT_DEVICE_05_STATE_1          0x0020000000000000L
#define BIT_DEVICE_06_STATE_0          0x0040000000000000L
#define BIT_DEVICE_06_STATE_1          0x0080000000000000L
#define BIT_DEVICE_07_STATE_0          0x0100000000000000L
#define BIT_DEVICE_07_STATE_1          0x0200000000000000L
#define BIT_DEVICE_08_STATE_0          0x0400000000000000L
#define BIT_DEVICE_08_STATE_1          0x0800000000000000L
#define BIT_RESERVE_0                  0x1000000000000000L
#define BIT_RESERVE_1                  0x2000000000000000L
#define BIT_RESERVE_2                  0x4000000000000000L
#define BIT_RESERVE_3                  0x8000000000000000L

#define SET_ENGINE_VERTICAL(f)         (f | BIT_ENGINE_VERTICAL)
#define UNSET_ENGINE_VERTICAL(f)       (f & (ALL_BIT^BIT_ENGINE_VERTICAL))
#define ENGINE_VERTICAL(f)             (f & BIT_ENGINE_VERTICAL)
#define SET_ENGINE_HORIZONTAL(f)       (f | BIT_ENGINE_HORIZONTAL)
#define UNSET_ENGINE_HORIZONTAL(f)     (f & (ALL_BIT^BIT_ENGINE_HORIZONTAL))
#define ENGINE_HORIZONTAL(f)           (f & BIT_ENGINE_HORIZONTAL)
#define SET_ACTUATOR_SPRAY(f)          (f | BIT_ACTUATOR_SPRAY)
#define UNSET_ACTUATOR_SPRAY(f)        (f & (ALL_BIT^BIT_ACTUATOR_SPRAY))
#define ACTUATOR_SPRAY(f)              (f & BIT_ACTUATOR_SPRAY)
#define SET_ACTUATOR_RATE(f)           (f | BIT_ACTUATOR_RATE)
#define UNSET_ACTUATOR_RATE(f)         (f & (ALL_BIT^BIT_ACTUATOR_RATE))
#define ACTUATOR_RATE(f)               (f & BIT_ACTUATOR_RATE)
#define SET_ACTUATOR_VEIL(f)           (f | BIT_ACTUATOR_VEIL)
#define UNSET_ACTUATOR_VEIL(f)         (f & (ALL_BIT^BIT_ACTUATOR_VEIL))
#define ACTUATOR_VEIL(f)               (f & BIT_ACTUATOR_VEIL)
#define SET_LIMIT_VERTICAL(f)          (f | BIT_LIMIT_VERTICAL)
#define UNSET_LIMIT_VERTICAL(f)        (f & (ALL_BIT^BIT_LIMIT_VERTICAL))
#define LIMIT_VERTICAL(f)              (f & BIT_LIMIT_VERTICAL)
#define SET_TIC_VERTICAL(f)            (f | BIT_TIC_VERTICAL)
#define UNSET_TIC_VERTICAL(f)          (f & (ALL_BIT^BIT_TIC_VERTICAL))
#define TIC_VERTICAL(f)                (f & BIT_TIC_VERTICAL)
#define SET_ENCODER_VERTICAL(f)        (f | BIT_ENCODER_VERTICAL)
#define UNSET_ENCODER_VERTICAL(f)      (f & (ALL_BIT^BIT_ENCODER_VERTICAL))
#define ENCODER_VERTICAL(f)            (f & BIT_ENCODER_VERTICAL)
#define SET_AMPERAGE_VERTICAL(f)       (f | BIT_AMPERAGE_VERTICAL)
#define UNSET_AMPERAGE_VERTICAL(f)     (f & (ALL_BIT^BIT_AMPERAGE_VERTICAL))
#define AMPERAGE_VERTICAL(f)           (f & BIT_AMPERAGE_VERTICAL)
#define SET_LIMIT_HORIZONTAL(f)        (f | BIT_LIMIT_HORIZONTAL)
#define UNSET_LIMIT_HORIZONTAL(f)      (f & (ALL_BIT^BIT_LIMIT_HORIZONTAL))
#define LIMIT_HORIZONTAL(f)            (f & BIT_LIMIT_HORIZONTAL)
#define SET_TIC_HORIZONTAL(f)          (f | BIT_TIC_HORIZONTAL)
#define UNSET_TIC_HORIZONTAL(f)        (f & (ALL_BIT^BIT_TIC_HORIZONTAL))
#define TIC_HORIZONTAL(f)              (f & BIT_TIC_HORIZONTAL)
#define SET_ENCODER_HORIZONTAL(f)      (f | BIT_ENCODER_HORIZONTAL)
#define UNSET_ENCODER_HORIZONTAL(f)    (f & (ALL_BIT^BIT_ENCODER_HORIZONTAL))
#define ENCODER_HORIZONTAL(f)          (f & BIT_ENCODER_HORIZONTAL)
#define SET_AMPERAGE_HORIZONTAL(f)     (f | BIT_AMPERAGE_HORIZONTAL)
#define UNSET_AMPERAGE_HORIZONTAL(f)   (f & (ALL_BIT^BIT_AMPERAGE_HORIZONTAL))
#define AMPERAGE_HORIZONTAL(f)         (f & BIT_AMPERAGE_HORIZONTAL)
#define SET_CONSOLE_SINGLY(f)          (f | BIT_CONSOLE_SINGLY)
#define UNSET_CONSOLE_SINGLY(f)        (f & (ALL_BIT^BIT_CONSOLE_SINGLY))
#define CONSOLE_SINGLY(f)              (f & BIT_CONSOLE_SINGLY)
#define SET_CONSOLE(f)                 (f | BIT_CONSOLE)
#define UNSET_CONSOLE(f)               (f & (ALL_BIT^BIT_CONSOLE))
#define CONSOLE(f)                     (f & BIT_CONSOLE)
#define SET_POST(f)                    (f | BIT_POST)
#define UNSET_POST(f)                  (f & (ALL_BIT^BIT_POST))
#define POST(f)                        (f & BIT_POST)
#define SET_SENSOR_FIRE_DRY(f)         (f | BIT_SENSOR_FIRE_DRY)
#define UNSET_SENSOR_FIRE_DRY(f)       (f & (ALL_BIT^BIT_SENSOR_FIRE_DRY))
#define SENSOR_FIRE_DRY(f)             (f & BIT_SENSOR_FIRE_DRY)
#define SET_SENSOR_FIRE_485(f)         (f | BIT_SENSOR_FIRE_485)
#define UNSET_SENSOR_FIRE_485(f)       (f & (ALL_BIT^BIT_SENSOR_FIRE_485))
#define SENSOR_FIRE_485(f)             (f & BIT_SENSOR_FIRE_485)
#define SET_SENSOR_FIRE_ETHERNET(f)    (f | BIT_SENSOR_FIRE_ETHERNET)
#define UNSET_SENSOR_FIRE_ETHERNET(f)  (f & (ALL_BIT^BIT_SENSOR_FIRE_ETHERNET))
#define SENSOR_FIRE_ETHERNET(f)        (f & BIT_SENSOR_FIRE_ETHERNET)
#define SET_SENSOR_DRY_485(f)          (f | BIT_SENSOR_DRY_485)
#define UNSET_SENSOR_DRY_485(f)        (f & (ALL_BIT^BIT_SENSOR_DRY_485))
#define SENSOR_DRY_485(f)              (f & BIT_SENSOR_DRY_485)
#define SET_SENSOR_DRY_ETHERNET(f)     (f | BIT_SENSOR_DRY_ETHERNER)
#define UNSET_SENSOR_DRY_ETHERNET(f)   (f & (ALL_BIT^BIT_SENSOR_DRY_ETHERNER))
#define SENSOR_DRY_ETHERNET(f)         (f & BIT_SENSOR_DRY_ETHERNER)
#define SET_VALVE_DRY(f)               (f | BIT_VALVE_DRY)
#define UNSET_VALVE_DRY(f)             (f & (ALL_BIT^BIT_VALVE_DRY))
#define VALVE_DRY(f)                   (f & BIT_VALVE_DRY)
#define SET_VALVE_ANALOG(f)            (f | BIT_VALVE_ANALOG)
#define UNSET_VALVE_ANALOG(f)          (f & (ALL_BIT^BIT_VALVE_ANALOG))
#define VALVE_ANALOG(f)                (f & BIT_VALVE_ANALOG)
#define SET_VALVE_LIMIT(f)             (f | BIT_VALVE_LIMIT)
#define UNSET_VALVE_LIMIT(f)           (f & (ALL_BIT^BIT_VALVE_LIMIT))
#define VALVE_LIMIT(f)                 (f & BIT_VALVE_LIMIT)
#define SET_VALVE_FEEDBACK(f)          (f | BIT_VALVE_FEEDBACK)
#define UNSET_VALVE_FEEDBACK(f)        (f & (ALL_BIT^BIT_VALVE_FEEDBACK))
#define VALVE_FEEDBACK(f)              (f & BIT_VALVE_FEEDBACK)
#define SET_CAM_ANALOG_DC(f)           (f | BIT_CAM_ANALOG_DC)
#define UNSET_CAM_ANALOG_DC(f)         (f & (ALL_BIT^BIT_CAM_ANALOG_DC))
#define CAM_ANALOG_DC(f)               (f & BIT_CAM_ANALOG_DC)
#define SET_CAM_DIGITAL_DC(f)          (f | BIT_CAM_DIGITAL_DC)
#define UNSET_CAM_DIGITAL_DC(f)        (f & (ALL_BIT^BIT_CAM_DIGITAL_DC))
#define CAM_DIGITAL_DC(f)              (f & BIT_CAM_DIGITAL_DC)
#define SET_CAM_DIGITAL_POE(f)         (f | BIT_CAM_DIGITAL_POE)
#define UNSET_CAM_DIGITAL_POE(f)       (f & (ALL_BIT^BIT_CAM_DIGITAL_POE))
#define CAM_DIGITAL_POE(f)             (f & BIT_CAM_DIGITAL_POE)
#define SET_FIRE_ALARM_01(f)           (f | BIT_FIRE_ALARM_01)
#define UNSET_FIRE_ALARM_01(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_01))
#define FIRE_ALARM_01(f)               (f & BIT_FIRE_ALARM_01)
#define SET_FIRE_ALARM_02(f)           (f | BIT_FIRE_ALARM_02)
#define UNSET_FIRE_ALARM_02(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_02))
#define FIRE_ALARM_02(f)               (f & BIT_FIRE_ALARM_02)
#define SET_FIRE_ALARM_03(f)           (f | BIT_FIRE_ALARM_03)
#define UNSET_FIRE_ALARM_03(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_03))
#define FIRE_ALARM_03(f)               (f & BIT_FIRE_ALARM_03)
#define SET_FIRE_ALARM_04(f)           (f | BIT_FIRE_ALARM_04)
#define UNSET_FIRE_ALARM_04(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_04))
#define FIRE_ALARM_04(f)               (f & BIT_FIRE_ALARM_04)
#define SET_FIRE_ALARM_05(f)           (f | BIT_FIRE_ALARM_05)
#define UNSET_FIRE_ALARM_05(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_05))
#define FIRE_ALARM_05(f)               (f & BIT_FIRE_ALARM_05)
#define SET_FIRE_ALARM_06(f)           (f | BIT_FIRE_ALARM_06)
#define UNSET_FIRE_ALARM_06(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_06))
#define FIRE_ALARM_06(f)               (f & BIT_FIRE_ALARM_06)
#define SET_FIRE_ALARM_07(f)           (f | BIT_FIRE_ALARM_07)
#define UNSET_FIRE_ALARM_07(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_07))
#define FIRE_ALARM_07(f)               (f & BIT_FIRE_ALARM_07)
#define SET_FIRE_ALARM_08(f)           (f | BIT_FIRE_ALARM_08)
#define UNSET_FIRE_ALARM_08(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_08))
#define FIRE_ALARM_08(f)               (f & BIT_FIRE_ALARM_08)
#define SET_FIRE_ALARM_09(f)           (f | BIT_FIRE_ALARM_09)
#define UNSET_FIRE_ALARM_09(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_09))
#define FIRE_ALARM_09(f)               (f & BIT_FIRE_ALARM_09)
#define SET_FIRE_ALARM_10(f)           (f | BIT_FIRE_ALARM_10)
#define UNSET_FIRE_ALARM_10(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_10))
#define FIRE_ALARM_10(f)               (f & BIT_FIRE_ALARM_10)
#define SET_FIRE_ALARM_11(f)           (f | BIT_FIRE_ALARM_11)
#define UNSET_FIRE_ALARM_11(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_11))
#define FIRE_ALARM_11(f)               (f & BIT_FIRE_ALARM_11)
#define SET_FIRE_ALARM_12(f)           (f | BIT_FIRE_ALARM_12)
#define UNSET_FIRE_ALARM_12(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_12))
#define FIRE_ALARM_12(f)               (f & BIT_FIRE_ALARM_12)
#define SET_FIRE_ALARM_13(f)           (f | BIT_FIRE_ALARM_13)
#define UNSET_FIRE_ALARM_13(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_13))
#define FIRE_ALARM_13(f)               (f & BIT_FIRE_ALARM_13)
#define SET_FIRE_ALARM_14(f)           (f | BIT_FIRE_ALARM_14)
#define UNSET_FIRE_ALARM_14(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_14))
#define FIRE_ALARM_14(f)               (f & BIT_FIRE_ALARM_14)
#define SET_FIRE_ALARM_15(f)           (f | BIT_FIRE_ALARM_15)
#define UNSET_FIRE_ALARM_15(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_15))
#define FIRE_ALARM_15(f)               (f & BIT_FIRE_ALARM_15)
#define SET_FIRE_ALARM_16(f)           (f | BIT_FIRE_ALARM_16)
#define UNSET_FIRE_ALARM_16(f)         (f & (ALL_BIT^BIT_FIRE_ALARM_16))
#define FIRE_ALARM_16(f)               (f & BIT_FIRE_ALARM_16)
#define SET_DEVICE_01_STATE_0(f)       (f | BIT_DEVICE_01_STATE_0)
#define UNSET_DEVICE_01_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_01_STATE_0))
#define DEVICE_01_STATE_0(f)           (f & BIT_DEVICE_01_STATE_0)
#define SET_DEVICE_01_STATE_1(f)       (f | BIT_DEVICE_01_STATE_1)
#define UNSET_DEVICE_01_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_01_STATE_1))
#define DEVICE_01_STATE_1(f)           (f & BIT_DEVICE_01_STATE_1)
#define SET_DEVICE_02_STATE_0(f)       (f | BIT_DEVICE_02_STATE_0)
#define UNSET_DEVICE_02_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_02_STATE_0))
#define DEVICE_02_STATE_0(f)           (f & BIT_DEVICE_02_STATE_0)
#define SET_DEVICE_02_STATE_1(f)       (f | BIT_DEVICE_02_STATE_1)
#define UNSET_DEVICE_02_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_02_STATE_1))
#define DEVICE_02_STATE_1(f)           (f & BIT_DEVICE_02_STATE_1)
#define SET_DEVICE_03_STATE_0(f)       (f | BIT_DEVICE_03_STATE_0)
#define UNSET_DEVICE_03_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_03_STATE_0))
#define DEVICE_03_STATE_0(f)           (f & BIT_DEVICE_03_STATE_0)
#define SET_DEVICE_03_STATE_1(f)       (f | BIT_DEVICE_03_STATE_1)
#define UNSET_DEVICE_03_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_03_STATE_1))
#define DEVICE_03_STATE_1(f)           (f & BIT_DEVICE_03_STATE_1)
#define SET_DEVICE_04_STATE_0(f)       (f | BIT_DEVICE_04_STATE_0)
#define UNSET_DEVICE_04_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_04_STATE_0))
#define DEVICE_04_STATE_0(f)           (f & BIT_DEVICE_04_STATE_0)
#define SET_DEVICE_04_STATE_1(f)       (f | BIT_DEVICE_04_STATE_1)
#define UNSET_DEVICE_04_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_04_STATE_1))
#define DEVICE_04_STATE_1(f)           (f & BIT_DEVICE_04_STATE_1)
#define SET_DEVICE_05_STATE_0(f)       (f | BIT_DEVICE_05_STATE_0)
#define UNSET_DEVICE_05_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_05_STATE_0))
#define DEVICE_05_STATE_0(f)           (f & BIT_DEVICE_05_STATE_0)
#define SET_DEVICE_05_STATE_1(f)       (f | BIT_DEVICE_05_STATE_1)
#define UNSET_DEVICE_05_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_05_STATE_1))
#define DEVICE_05_STATE_1(f)           (f & BIT_DEVICE_05_STATE_1)
#define SET_DEVICE_06_STATE_0(f)       (f | BIT_DEVICE_06_STATE_0)
#define UNSET_DEVICE_06_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_06_STATE_0))
#define DEVICE_06_STATE_0(f)           (f & BIT_DEVICE_06_STATE_0)
#define SET_DEVICE_06_STATE_1(f)       (f | BIT_DEVICE_06_STATE_1)
#define UNSET_DEVICE_06_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_06_STATE_1))
#define DEVICE_06_STATE_1(f)           (f & BIT_DEVICE_06_STATE_1)
#define SET_DEVICE_07_STATE_0(f)       (f | BIT_DEVICE_07_STATE_0)
#define UNSET_DEVICE_07_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_07_STATE_0))
#define DEVICE_07_STATE_0(f)           (f & BIT_DEVICE_07_STATE_0)
#define SET_DEVICE_07_STATE_1(f)       (f | BIT_DEVICE_07_STATE_1)
#define UNSET_DEVICE_07_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_07_STATE_1))
#define DEVICE_07_STATE_1(f)           (f & BIT_DEVICE_07_STATE_1)
#define SET_DEVICE_08_STATE_0(f)       (f | BIT_DEVICE_08_STATE_0)
#define UNSET_DEVICE_08_STATE_0(f)     (f & (ALL_BIT^BIT_DEVICE_08_STATE_0))
#define DEVICE_08_STATE_0(f)           (f & BIT_DEVICE_08_STATE_0)
#define SET_DEVICE_08_STATE_1(f)       (f | BIT_DEVICE_08_STATE_1)
#define UNSET_DEVICE_08_STATE_1(f)     (f & (ALL_BIT^BIT_DEVICE_08_STATE_1))
#define DEVICE_08_STATE_1(f)           (f & BIT_DEVICE_08_STATE_1)
#define SET_RESERVE_0(f)               (f | BIT_RESERVE_0)
#define UNSET_RESERVE_0(f)             (f & (ALL_BIT^BIT_RESERVE_0))
#define RESERVE_0(f)                   (f & BIT_RESERVE_0)
#define SET_RESERVE_1(f)               (f | BIT_RESERVE_1)
#define UNSET_RESERVE_1(f)             (f & (ALL_BIT^BIT_RESERVE_1))
#define RESERVE_1(f)                   (f & BIT_RESERVE_1)
#define SET_RESERVE_2(f)               (f | BIT_RESERVE_2)
#define UNSET_RESERVE_2(f)             (f & (ALL_BIT^BIT_RESERVE_2))
#define RESERVE_2(f)                   (f & BIT_RESERVE_2)
#define SET_RESERVE_3(f)               (f | BIT_RESERVE_3)
#define UNSET_RESERVE_3(f)             (f & (ALL_BIT^BIT_RESERVE_3))
#define RESERVE_3(f)                   (f & BIT_RESERVE_3)

struct _block_info_controller_s
{
	GtkLabel * label_name;
	GtkWidget * box_engine_vertical;
	GtkWidget * box_engine_horizontal;
	GtkWidget * box_actuator_spray;
	GtkWidget * box_actuator_rate;
	GtkWidget * box_actuator_veil;
	GtkWidget * box_limit_vertical;
	GtkWidget * box_tic_vertical;
	GtkEntryBuffer * buf_tic_vertical;
	GtkWidget * box_encoder_vertical;
	GtkEntryBuffer * buf_encoder_vertical;
	GtkWidget * box_amperage_vertical;
	GtkEntryBuffer * buf_amperage_vertical;
	GtkWidget * box_limit_horizontal;
	GtkWidget * box_tic_horizontal;
	GtkEntryBuffer * buf_tic_horizontal;
	GtkWidget * box_encoder_horizontal;
	GtkEntryBuffer * buf_encoder_horizontal;
	GtkWidget * box_amperage_horizontal;
	GtkEntryBuffer * buf_amperage_horizontal;
	GtkWidget * box_console_singly;
	GtkWidget * box_console;
	GtkWidget * box_post;
	GtkWidget * box_sensor_fire_dry;
	GtkWidget * box_sensor_fire_485;
	GtkWidget * box_sensor_fire_ethernet;
	GtkWidget * box_sensor_dry_485;
	GtkWidget * box_sensor_dry_etherner;
	GtkWidget * box_valve_dry;
	GtkWidget * box_valve_analog;
	GtkEntryBuffer * buf_valve_analog;
	GtkWidget * box_valve_limit;
	GtkWidget * box_valve_feedback;
	GtkWidget * box_cam_analog_dc;
	GtkWidget * box_cam_digital_dc;
	GtkWidget * box_cam_digital_poe;
	GtkWidget * box_fire_alarm_01;
	GtkWidget * box_fire_alarm_02;
	GtkWidget * box_fire_alarm_03;
	GtkWidget * box_fire_alarm_04;
	GtkWidget * box_fire_alarm_05;
	GtkWidget * box_fire_alarm_06;
	GtkWidget * box_fire_alarm_07;
	GtkWidget * box_fire_alarm_08;
	GtkWidget * box_fire_alarm_09;
	GtkWidget * box_fire_alarm_10;
	GtkWidget * box_fire_alarm_11;
	GtkWidget * box_fire_alarm_12;
	GtkWidget * box_fire_alarm_13;
	GtkWidget * box_fire_alarm_14;
	GtkWidget * box_fire_alarm_15;
	GtkWidget * box_fire_alarm_16;
	GtkWidget * box_device_01_state_0;
	GtkWidget * box_device_01_state_1;
	GtkWidget * box_device_02_state_0;
	GtkWidget * box_device_02_state_1;
	GtkWidget * box_device_03_state_0;
	GtkWidget * box_device_03_state_1;
	GtkWidget * box_device_04_state_0;
	GtkWidget * box_device_04_state_1;
	GtkWidget * box_device_05_state_0;
	GtkWidget * box_device_05_state_1;
	GtkWidget * box_device_06_state_0;
	GtkWidget * box_device_06_state_1;
	GtkWidget * box_device_07_state_0;
	GtkWidget * box_device_07_state_1;
	GtkWidget * box_device_08_state_0;
	GtkWidget * box_device_08_state_1;
};
typedef struct _block_info_controller_s block_info_controller_s;

struct _block_setting_controller_s
{
	GtkEntryBuffer * address;
	GtkEntryBuffer * port;
	GtkEntryBuffer * id;

	block_info_controller_s * block_info;

	link_s * link;
	char * name;
	config_controller_s * config;
	state_controller_s * state;
};
typedef struct _block_setting_controller_s block_setting_controller_s;

struct _block_controller_s
{
	GtkLabel * name;
	GtkLabel * lafet;
	GtkLabel * tic_vertical;
	GtkLabel * tic_horizontal;
	GtkLabel * encoder_vertical;
	GtkLabel * encoder_horizontal;
	GtkLabel * pressure;
	GtkLabel * amperage_vertical;
	GtkLabel * amperage_horizontal;
   GtkLabel * valve;
   GtkLabel * tic_valve;
   GtkLabel * fire_sensor;
   GtkLabel * fire_alarm;
};
typedef struct _block_controller_s block_controller_s;

struct _all_controller_s
{
	GSList * list;
	controller_s * current;
	uint32_t timeout_current;
	uint32_t timeout_all;
	uint32_t timeout_config;
};
typedef struct _all_controller_s all_controller_s;
/*****************************************************************************/
/* локальные функции                                                         */
/*****************************************************************************/
static int check_link_controller(link_s * link,config_controller_s * config,state_controller_s * state)
{
	int rc;
	rc = link_connect_controller(link);
	if(rc == FAILURE){
		return rc;
	}
	rc = link_config_controller(link,config);
	if(rc == FAILURE){
		return rc;
	}
	rc = link_state_controller(link,state);
	if(rc == FAILURE){
		return rc;
	}
	return SUCCESS;
}

static int fill_block_info(block_setting_controller_s * bsc)
{
	GtkLabel * label;
	GtkWidget * box;
	char * name;
	block_info_controller_s * block_info = bsc->block_info;
	config_controller_s * config = bsc->config;
	uint64_t flag;

	if(config == NULL){
		flag = 0;
	}
	else{
		flag = config->flag;
	}

	name = bsc->name;
	label = block_info->label_name;
	gtk_label_set_text(label,name);

	box = block_info->box_engine_vertical;
	if(ENGINE_VERTICAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_engine_horizontal;
	if(ENGINE_HORIZONTAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_actuator_spray;
	if(ACTUATOR_SPRAY(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_actuator_rate;
	if(ACTUATOR_RATE(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_actuator_veil;
	if(ACTUATOR_VEIL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_limit_vertical;
	if(LIMIT_VERTICAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_tic_vertical;
	if(TIC_VERTICAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_encoder_vertical;
	if(ENCODER_VERTICAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_amperage_vertical;
	if(AMPERAGE_VERTICAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_limit_horizontal;
	if(LIMIT_HORIZONTAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_tic_horizontal;
	if(TIC_HORIZONTAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_encoder_horizontal;
	if(ENCODER_HORIZONTAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_amperage_horizontal;
	if(AMPERAGE_HORIZONTAL(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_console_singly;
	if(CONSOLE_SINGLY(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_console;
	if(CONSOLE(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_post;
	if(POST(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_sensor_fire_dry;
	if(SENSOR_FIRE_DRY(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_sensor_fire_485;
	if(SENSOR_FIRE_485(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_sensor_fire_ethernet;
	if(SENSOR_FIRE_ETHERNET(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_sensor_dry_485;
	if(SENSOR_DRY_485(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_sensor_dry_etherner;
	if(SENSOR_DRY_ETHERNET(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_valve_dry;
	if(VALVE_DRY(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_valve_analog;
	if(VALVE_ANALOG(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_valve_limit;
	if(VALVE_LIMIT(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_valve_feedback;
	if(VALVE_FEEDBACK(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_cam_analog_dc;
	if(CAM_ANALOG_DC(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_cam_digital_dc;
	if(CAM_DIGITAL_DC(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_cam_digital_poe;
	if(CAM_DIGITAL_POE(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_01;
	if(FIRE_ALARM_01(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_02;
	if(FIRE_ALARM_02(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_03;
	if(FIRE_ALARM_03(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_04;
	if(FIRE_ALARM_04(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_05;
	if(FIRE_ALARM_05(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_06;
	if(FIRE_ALARM_06(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_07;
	if(FIRE_ALARM_07(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_08;
	if(FIRE_ALARM_08(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_09;
	if(FIRE_ALARM_09(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_10;
	if(FIRE_ALARM_10(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_11;
	if(FIRE_ALARM_11(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_12;
	if(FIRE_ALARM_12(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_13;
	if(FIRE_ALARM_13(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_14;
	if(FIRE_ALARM_14(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_15;
	if(FIRE_ALARM_15(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_fire_alarm_16;
	if(FIRE_ALARM_16(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_01_state_0;
	if(DEVICE_01_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_01_state_1;
	if(DEVICE_01_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_02_state_0;
	if(DEVICE_02_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_02_state_1;
	if(DEVICE_02_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_03_state_0;
	if(DEVICE_03_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_03_state_1;
	if(DEVICE_03_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_04_state_0;
	if(DEVICE_04_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_04_state_1;
	if(DEVICE_04_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_05_state_0;
	if(DEVICE_05_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_05_state_1;
	if(DEVICE_05_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_06_state_0;
	if(DEVICE_06_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_06_state_1;
	if(DEVICE_06_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_07_state_0;
	if(DEVICE_07_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_07_state_1;
	if(DEVICE_07_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_08_state_0;
	if(DEVICE_08_STATE_0(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	box = block_info->box_device_08_state_1;
	if(DEVICE_08_STATE_1(flag)){
		gtk_widget_show(box);
	}
	else{
		gtk_widget_hide(box);
	}

	return SUCCESS;
}

block_info_controller_s block_info_controller;
static char STR_NAME[] = "Наименование : ";
static char STR_NAME_DEFAULT[] = "Нет информации";

static char STR_ENGINE_VERTICAL[]      = "Двигатель вертикальной оси";
static char STR_ENGINE_HORIZONTAL[]    = "Двигатель горизонтальной оси";
static char STR_ACTUATOR_SPRAY[]       = "Актуатор (Распыл)";
static char STR_ACTUATOR_RATE[]        = "Актуатор (Расход)";
static char STR_ACTUATOR_VEIL[]        = "Актуатор (Завеса)";
static char STR_LIMIT_VERTICAL[]       = "Концевики вертикальной оси";
static char STR_TIC_VERTICAL[]         = "Датчик импульсов вертикальной оси";
static char STR_ENCODER_VERTICAL[]     = "Энкодер вертикальной оси";
static char STR_AMPERAGE_VERTICAL[]    = "Датчик тока вертикальной оси";
static char STR_LIMIT_HORIZONTAL[]     = "Концевики горизонтальной оси";
static char STR_TIC_HORIZONTAL[]       = "Датчик импульсов горизонтальной оси";
static char STR_ENCODER_HORIZONTAL[]   = "Энкодер горизонтальной оси";
static char STR_AMPERAGE_HORIZONTAL[]  = "Датчик тока горизонтальной оси";
static char STR_CONSOLE_SINGLY[]       = "Пульт ПДУ-П (отдельно)";
static char STR_CONSOLE[]              = "Пульт ПДУ-П (на панели шкафа)";
static char STR_POST[]                 = "Пост ПДУ";
static char STR_SENSOR_FIRE_DRY[]      = "Датчик пламени (сухой контакт)";
static char STR_SENSOR_FIRE_485[]      = "Датчик пламени (интерфейс RS-485)";
static char STR_SENSOR_FIRE_ETHERNET[] = "Датчик пламени (интерфейс Ethernet)";
static char STR_SENSOR_DRY_485[]       = "Датчик (Сухой контакт + Интерфейс RS-485)";
static char STR_SENSOR_DRY_ETHERNER[]  = "Датчик (Сухой контакт + Интерфейс Ethernet)";
static char STR_VALVE_DRY[]            = "Заслонка (Сухой контакт)";
static char STR_VALVE_ANALOG[]         = "Заслонка (Аналоговый сигнал)";
static char STR_VALVE_LIMIT[]          = "Заслонка (Концевики)";
static char STR_VALVE_FEEDBACK[]       = "Заслонка (Обратная связь)";
static char STR_CAM_ANALOG_DC[]        = "Аналоговая видеокамера (DC)";
static char STR_CAM_DIGITAL_DC[]       = "Цифровая видеокамера (DC)";
static char STR_CAM_DIGITAL_POE[]      = "Цифровая видеокамера (POE)";
static char STR_FIRE_ALARM_01[]        = "1 шлейф ПС";
static char STR_FIRE_ALARM_02[]        = "2 шлейф ПС";
static char STR_FIRE_ALARM_03[]        = "3 шлейф ПС";
static char STR_FIRE_ALARM_04[]        = "4 шлейф ПС";
static char STR_FIRE_ALARM_05[]        = "5 шлейф ПС";
static char STR_FIRE_ALARM_06[]        = "6 шлейф ПС";
static char STR_FIRE_ALARM_07[]        = "7 шлейф ПС";
static char STR_FIRE_ALARM_08[]        = "8 шлейф ПС";
static char STR_FIRE_ALARM_09[]        = "9 шлейф ПС";
static char STR_FIRE_ALARM_10[]        = "10 шлейф ПС";
static char STR_FIRE_ALARM_11[]        = "11 шлейф ПС";
static char STR_FIRE_ALARM_12[]        = "12 шлейф ПС";
static char STR_FIRE_ALARM_13[]        = "13 шлейф ПС";
static char STR_FIRE_ALARM_14[]        = "14 шлейф ПС";
static char STR_FIRE_ALARM_15[]        = "15 шлейф ПС";
static char STR_FIRE_ALARM_16[]        = "16 шлейф ПС";
static char STR_DEVICE_01_STATE_0[]    = "Устройство 1 (Состояние А)";
static char STR_DEVICE_01_STATE_1[]    = "Устройство 1 (Состояние Б)";
static char STR_DEVICE_02_STATE_0[]    = "Устройство 2 (Состояние А)";
static char STR_DEVICE_02_STATE_1[]    = "Устройство 2 (Состояние Б)";
static char STR_DEVICE_03_STATE_0[]    = "Устройство 3 (Состояние А)";
static char STR_DEVICE_03_STATE_1[]    = "Устройство 3 (Состояние Б)";
static char STR_DEVICE_04_STATE_0[]    = "Устройство 4 (Состояние А)";
static char STR_DEVICE_04_STATE_1[]    = "Устройство 4 (Состояние Б)";
static char STR_DEVICE_05_STATE_0[]    = "Устройство 5 (Состояние А)";
static char STR_DEVICE_05_STATE_1[]    = "Устройство 5 (Состояние Б)";
static char STR_DEVICE_06_STATE_0[]    = "Устройство 6 (Состояние А)";
static char STR_DEVICE_06_STATE_1[]    = "Устройство 6 (Состояние Б)";
static char STR_DEVICE_07_STATE_0[]    = "Устройство 7 (Состояние А)";
static char STR_DEVICE_07_STATE_1[]    = "Устройство 7 (Состояние Б)";
static char STR_DEVICE_08_STATE_0[]    = "Устройство 8 (Состояние А)";
static char STR_DEVICE_08_STATE_1[]    = "Устройство 8 (Состояние Б)";

static int min_width_block_info = 300;
static int min_height_block_info = 400;

static GtkWidget * create_block_info(block_setting_controller_s * bsc)
{
	GtkWidget * scrwin;
	GtkWidget * box_main;
	GtkWidget * box;
	GtkWidget * label;
	GtkWidget * entry;

	block_info_controller_s * block_info = &block_info_controller;

	scrwin = gtk_scrolled_window_new(NULL,NULL);
	layout_widget(scrwin,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrwin),min_width_block_info);
	gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrwin),min_height_block_info);

	box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	layout_widget(box_main,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);

	/*наименование*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_NAME);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	label = gtk_label_new(STR_NAME_DEFAULT);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->label_name = GTK_LABEL(label);

	/*двигатель вертикальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ENGINE_VERTICAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_engine_vertical = box;

	/*двигатель горизонтальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ENGINE_HORIZONTAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_engine_horizontal = box;

	/*Актуатор Распыление*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ACTUATOR_SPRAY);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_actuator_spray = box;

	/*Актуатор распыл*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ACTUATOR_RATE);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_actuator_rate = box;

	/*Актуатор завеса*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ACTUATOR_VEIL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_actuator_veil = box;

	/*концевики вертикальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_LIMIT_VERTICAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_limit_vertical = box;

	/*датчик импульсов вертикальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_TIC_VERTICAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_tic_vertical = box;
	block_info->buf_tic_vertical = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Энкодер вертикальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ENCODER_VERTICAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_encoder_vertical = box;
	block_info->buf_encoder_vertical = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*датчик тока вертикальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_AMPERAGE_VERTICAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_amperage_vertical = box;
	block_info->buf_amperage_vertical = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Концевики горизонтальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_LIMIT_HORIZONTAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_limit_horizontal = box;

	/*Датчик импульсов горизонтальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_TIC_HORIZONTAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_tic_horizontal = box;
	block_info->buf_tic_horizontal = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Энкодер горизонтальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_ENCODER_HORIZONTAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_encoder_horizontal = box;
	block_info->buf_encoder_horizontal = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Датчик тока горизонтальной оси*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_AMPERAGE_HORIZONTAL);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_amperage_horizontal = box;
	block_info->buf_amperage_horizontal = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Пулт ПДУ-П отдельно */
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_CONSOLE_SINGLY);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_console_singly = box;

	/*Пульт ПДУ */
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_CONSOLE);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_console = box;

	/*Пост ПДУ*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_POST);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_post = box;

	/*Датчик пламени сухой контакт*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_SENSOR_FIRE_DRY);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_sensor_fire_dry = box;

	/*Датчик пламени RS-485*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_SENSOR_FIRE_485);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_sensor_fire_485 = box;

	/*датчик пламени Ethernet*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_SENSOR_FIRE_ETHERNET);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_sensor_fire_ethernet = box;

	/*Датчик суой контакт RS-485*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_SENSOR_DRY_485);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_sensor_dry_485 = box;

	/*Датчик сухой контакт Ethernet*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_SENSOR_DRY_ETHERNER);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_sensor_dry_etherner = box;

	/*Заслонка сухой контакт*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_VALVE_DRY);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	block_info->box_valve_dry = box;

	/*Заслонка с управлением*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_VALVE_ANALOG);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);

	/*TODO настройки ввода*/
	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);

	block_info->box_valve_analog = box;
	block_info->buf_valve_analog = gtk_entry_get_buffer(GTK_ENTRY(entry));

	/*Заслонка концевики */
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_VALVE_LIMIT);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_valve_limit = box;

	/*Заслонка обратнаясвязь*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_VALVE_FEEDBACK);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_valve_feedback = box;

	/*Видеокамера аналоговая*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_CAM_ANALOG_DC);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_cam_analog_dc = box;

	/*Видеокамера цифровая*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_CAM_DIGITAL_DC);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_cam_digital_dc = box;

	/*Видеокамера цифровая питание по Ethernet*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_CAM_DIGITAL_POE);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_cam_digital_poe = box;

	/*ПС шлейф 01*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_01);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_01 = box;

	/*ПС шлейф 02*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_02);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_02 = box;

	/*ПС шлейф 03*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_03);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_03 = box;

	/*ПС шлейф 04*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_04);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_04 = box;

	/*ПС шлейф 05*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_05);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_05 = box;

	/*ПС шлейф 06*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_06);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_06 = box;

	/*ПС шлейф 07*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_07);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_07 = box;

	/*ПС шлейф 08*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_08);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_08 = box;

	/*ПС шлейф 09*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_09);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_09 = box;

	/*ПС шлейф 10*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_10);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_10 = box;

	/*ПС шлейф 11*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_11);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_11 = box;

	/*ПС шлейф 12*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_12);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_12 = box;

	/*ПС шлейф 13*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_13);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_13 = box;

	/*ПС шлейф 14*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_14);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_14 = box;

	/*ПС шлейф 15*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_15);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_15 = box;

	/*ПС шлейф 16*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_FIRE_ALARM_16);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_fire_alarm_16 = box;

	/*Устройство 1 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_01_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_01_state_0 = box;

	/*Устройство 1 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_01_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_01_state_1 = box;

	/*Устройство 2 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_02_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_02_state_0 = box;

	/*Устройство 2 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_02_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_02_state_1 = box;

	/*Устройство 3 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_03_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_03_state_0 = box;

	/*Устройство 3 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_03_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_03_state_1 = box;

	/*Устройство 4 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_04_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_04_state_0 = box;

	/*Устройство 4 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_04_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_04_state_1 = box;

	/*Устройство 5 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_05_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_05_state_0 = box;

	/*Устройство 5 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_05_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_05_state_1 = box;

	/*Устройство 6 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_06_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_06_state_0 = box;

	/*Устройство 6 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_06_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_06_state_1 = box;

	/*Устройство 7 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_07_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_07_state_0 = box;

	/*Устройство 7 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_07_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_07_state_1 = box;

	/*Устройство 8 стутас 0*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_08_STATE_0);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_08_state_0 = box;

	/*Устройство 8 стутас 1*/
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_START,TRUE,FALSE);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(box_main),box,TRUE,TRUE,0);

	label = gtk_label_new(STR_DEVICE_08_STATE_1);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_START,FALSE,FALSE);
	gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,0);
	gtk_widget_show(label);
	block_info->box_device_08_state_1 = box;


	gtk_container_add(GTK_CONTAINER(scrwin),box_main);

	gtk_widget_show(box_main);
	gtk_widget_show(scrwin);

	bsc->block_info = block_info;
	fill_block_info(bsc);

	return scrwin;
}

static char * check_address(const char * address)
{
	char * str;
	GInetAddress * inet_address = g_inet_address_new_from_string(address);
	if(inet_address == NULL){
		return NULL;
	}
	str = g_inet_address_to_string(inet_address);
	g_object_unref(inet_address);
	return str;
}

static uint16_t check_port(const char * port)
{
	uint16_t p = 0;
	p = g_ascii_strtoull(port,NULL,10);
	return p;
}

static uint8_t check_id(const char * id)
{
	uint8_t t = 0;
	t = g_ascii_strtoull(id,NULL,10);
	if(t < MIN_ID){
		t = 0;
	}
	else{
		if(t > MAX_ID){
			t = 0;
		}
	}
	return t;
}

static GtkWidget * create_block_entry(char * name,GtkEntryBuffer ** buf)
{
	GtkWidget * box;
	GtkWidget * label;
	GtkWidget * entry;

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_CENTER,TRUE,FALSE);

	label = gtk_label_new(name);
	layout_widget(label,GTK_ALIGN_START,GTK_ALIGN_CENTER,FALSE,FALSE);

	entry = gtk_entry_new();
	layout_widget(entry,GTK_ALIGN_START,GTK_ALIGN_CENTER,TRUE,FALSE);
	*buf = gtk_entry_get_buffer(GTK_ENTRY(entry));

	gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(box),entry,TRUE,TRUE,0);

	gtk_widget_show(box);
	gtk_widget_show(label);
	gtk_widget_show(entry);

	return box;
}

static int check_rate_controller(block_info_controller_s * block_info,config_controller_s * config)
{
	const char * str;
	double rate;
	GtkEntryBuffer * buf;

	buf = block_info->buf_tic_vertical;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_tic_vertical = rate;

	buf = block_info->buf_encoder_vertical;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_encoder_vertical = rate;

	buf = block_info->buf_amperage_vertical;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_amperage_vertical = rate;

	buf = block_info->buf_tic_horizontal;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_tic_horizontal = rate;

	buf = block_info->buf_encoder_horizontal;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_encoder_horizontal = rate;

	buf = block_info->buf_amperage_horizontal;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_amperage_horizontal = rate;

	buf = block_info->buf_valve_analog;
	str = gtk_entry_buffer_get_text(buf);
	rate = g_strtod(str,NULL);
	config->rate_valve_analog = rate;

	return SUCCESS;
}

static void clicked_button_check(GtkButton * button,gpointer ud)
{
	int rc;
	char * address = NULL;
	uint16_t port;
	uint8_t id;
	const char * str;
	link_s * link;
	config_controller_s * config;
	state_controller_s * state;
	block_setting_controller_s * bsc = (block_setting_controller_s*)ud;

	/*TODO при повторном нажатии утечка памяти*/
	bsc->link = NULL;
	/*TODO вывод сообщений*/
	str = gtk_entry_buffer_get_text(bsc->address);
	address = check_address(str);
	if(address == NULL){
		g_warning("Адрес контролера не корректный!");
		return ;
	}
	str = gtk_entry_buffer_get_text(bsc->port);
	port = check_port(str);
	if(port == 0){
		g_warning("Порт контролера не корректный!");
		return ;
	}
	str = gtk_entry_buffer_get_text(bsc->id);
	id = check_id(str);
	if(id == 0){
		g_warning("Индификатор контролера не корректный!");
		return ;
	}

	link = g_slice_alloc0(sizeof(link_s));
	config = g_slice_alloc0(sizeof(config_controller_s));
	state = g_slice_alloc0(sizeof(state_controller_s));

	/*TODO добавление других типов соединений*/
	link->type = TYPE_LINK_TCP;
	link->connect = NULL;
	link->address = address;
	link->port = port;
	link->id = id;

	rc = check_link_controller(link,config,state);
	if(rc == FAILURE){
		g_slice_free1(sizeof(link_s),link);
		g_slice_free1(sizeof(config_controller_s),config);
		g_slice_free1(sizeof(state_controller_s),state);
		link_disconnect_controller(link);
		return;
	}

	bsc->link = link;
	bsc->config = config;
	bsc->state = state;
	bsc->name = get_name_controller(config);
	/*TODO сообщенийние что проверка корректна*/
	fill_block_info(bsc);

	link_disconnect_controller(link);
}
/*****************************************************************************/
/*    Общие функции                                                          */
/*****************************************************************************/
static block_setting_controller_s block_setting_controller;

void * new_property_controller(void)
{
	int rc;
	controller_s * controller;
	link_s * link = block_setting_controller.link;
	config_controller_s * config = block_setting_controller.config;
	state_controller_s * state = block_setting_controller.state;
	block_info_controller_s * block_info = block_setting_controller.block_info;

	/*TODO вывод сообщений*/
	if(link == NULL){
		g_warning("Соединение не проверено!");
		return NULL;
	}

	rc = check_rate_controller(block_info,config);
	if(rc == FAILURE){
		g_warning("Некоректный коэффициенты");
		return NULL;
	}
	controller = g_slice_alloc0(sizeof(controller_s));
	controller->name = block_setting_controller.name;
	controller->link = link;
	controller->config = config;
	controller->state = state;

	return controller;
}

static char STR_NAME_CONTROLLER[] = "Контроллер";
static char STR_NAME_ADDRESS[] = "Адрес";
static char STR_NAME_PORT[] = "Порт";
static char STR_NAME_ID[] = "Номер";
static char STR_NAME_CHECK[] = "Поиск";

GtkWidget * create_block_setting_controller(void)
{
	GtkWidget * grid;
	GtkWidget * label;
	GtkWidget * block_address;
	GtkWidget * block_port;
	GtkWidget * block_id;
	GtkWidget * block_info;
	GtkEntryBuffer * buf;
	GtkWidget * but_check;

	block_setting_controller.link = NULL;
	block_setting_controller.config = NULL;
	block_setting_controller.name = STR_NAME_DEFAULT;

	grid = gtk_grid_new();
	layout_widget(grid,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);

	label = gtk_label_new(STR_NAME_CONTROLLER);
	layout_widget(label,GTK_ALIGN_CENTER,GTK_ALIGN_CENTER,TRUE,FALSE);

	block_address = create_block_entry(STR_NAME_ADDRESS,&buf);
	block_setting_controller.address = buf;

	block_port = create_block_entry(STR_NAME_PORT,&buf);
	block_setting_controller.port = buf;

	block_id = create_block_entry(STR_NAME_ID,&buf);
	block_setting_controller.id = buf;

	but_check = gtk_button_new_with_label(STR_NAME_CHECK);
	layout_widget(but_check,GTK_ALIGN_CENTER,GTK_ALIGN_CENTER,FALSE,FALSE);
	g_signal_connect(but_check,"clicked",G_CALLBACK(clicked_button_check),&block_setting_controller);

	block_info = create_block_info(&block_setting_controller);

	gtk_grid_attach(GTK_GRID(grid),label        ,0,0,2,1);
	gtk_grid_attach(GTK_GRID(grid),block_address,0,1,1,1);
	gtk_grid_attach(GTK_GRID(grid),block_port   ,0,2,1,1);
	gtk_grid_attach(GTK_GRID(grid),block_id     ,0,3,1,1);
	gtk_grid_attach(GTK_GRID(grid),but_check    ,0,4,2,1);
	gtk_grid_attach(GTK_GRID(grid),block_info   ,0,5,2,1);

	gtk_widget_show(grid);
	gtk_widget_show(label);
	gtk_widget_show(but_check);

	return grid;
}

/*****************************************************************************/
#define DEFAULT_TIMEOUT_CURRENR        500     /* 500 милесекунд*/
#define DEFAULT_TIMEOUT_ALL            3000    /* 3 секунды */
#define DEFAULT_TIMEOUT_CONFIG         600000  /* 600 секунд*/
all_controller_s all_controller;
int init_all_controller(void)
{
	all_controller.list = g_slist_alloc();
	all_controller.current = NULL;
	all_controller.timeout_current =  DEFAULT_TIMEOUT_CURRENR;
	all_controller.timeout_all = DEFAULT_TIMEOUT_ALL;
	all_controller.timeout_config = DEFAULT_TIMEOUT_CONFIG;

	return SUCCESS;
}

int deinit_all_controller(void)
{
	g_slist_free(all_controller.list);
	return SUCCESS;
}

static int connect_controller(controller_s * controller)
{
	int rc;
	link_s * link = controller->link;
	config_controller_s check;
	config_controller_s * config = controller->config;
	state_controller_s * state = controller->state;


	if(link->connect != NULL){
		g_warning("Контролер подключен!");
		return SUCCESS;
	}
	rc = check_link_controller(link,&check,state);
	if(rc == FAILURE){
		g_warning("Нет подключения к контроллеру!");
		return rc;
	}
	rc = check_config_controller(&check,config);
	if(rc == FAILURE){
		/*TODO перезапись базы данных*/
		g_warning("Данные из контроллера не совпадают с данными из базы данных!");
		link_disconnect_controller(link);
		return rc;
	}

	return SUCCESS;
}

/*TODO считывание данных из базыданных*/
controller_s * init_controller(uint32_t number)
{
	int rc;
	controller_s * controller = NULL;

	controller = g_slice_alloc0(sizeof(controller_s));
	controller->link = g_slice_alloc0(sizeof(link_s));
	controller->config = g_slice_alloc0(sizeof(config_controller_s));
	controller->state = g_slice_alloc0(sizeof(state_controller_s));
	/*память для обектов выделяется при чтении из базыданых*/
	rc = read_database_controller(number,controller);
	if(rc != SUCCESS){
		g_slice_free1(sizeof(controller_s),controller);
		g_slice_free1(sizeof(link_s),controller->link);
		g_slice_free1(sizeof(config_controller_s),controller->config);
		g_slice_free1(sizeof(state_controller_s),controller->state);
		controller = NULL;
		return NULL;
	}
	controller->name = get_name_controller(controller->config);

	all_controller.list = g_slist_append(all_controller.list,controller);

	g_debug(" :> %d",controller->config->type);
	g_debug(" :> %#lx",controller->config->flag);
	g_debug(" :> %f",controller->config->rate_tic_vertical);
	g_debug(" :> %f",controller->config->rate_encoder_vertical);
	g_debug(" :> %f",controller->config->rate_amperage_vertical);
	g_debug(" :> %f",controller->config->rate_tic_horizontal);
	g_debug(" :> %f",controller->config->rate_encoder_horizontal);
	g_debug(" :> %f",controller->config->rate_amperage_horizontal);
	g_debug(" :> %f",controller->config->rate_valve_analog);

	return controller;
}

int del_property_controller(controller_s * property)
{
	char * str;
	link_s * link;
	config_controller_s * config;
	state_controller_s * state;

	if(property == NULL){
		return SUCCESS;
	}
	str = property->name;
	g_free(str);

	link = property->link;
	link_disconnect_controller(link);
	str = link->address;
	g_free(str);
	g_slice_free1(sizeof(link_s),link);

	config = property->config;
	g_slice_free1(sizeof(config_controller_s),config);
	state = property->state;
	g_slice_free1(sizeof(state_controller_s),state);
	g_slice_free1(sizeof(controller_s),property);

	return SUCCESS;
}

static char STR_STATE[] = "Информация";
static GtkWidget * create_block_state(block_controller_s * block)
{
	GtkWidget * frame;
	GtkWidget * grid;
	GtkWidget * label_name;
	GtkWidget * label_value;

	frame = gtk_frame_new(STR_STATE);
	layout_widget(frame,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);

	grid = gtk_grid_new();
	layout_widget(grid,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid),TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);

#if 1
	label_name = gtk_label_new(NULL);
	layout_widget(label_name,GTK_ALIGN_CENTER,GTK_ALIGN_START,TRUE,TRUE);
	gtk_widget_show(label_name);
	gtk_grid_attach(GTK_GRID(grid),label_name,0,0,2,1);
	block->name = GTK_LABEL(label_name);

	label_name = gtk_label_new("Датчики лафета");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,1,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,1,1,1);
	block->lafet = GTK_LABEL(label_value);

	label_name = gtk_label_new("Вертикальная ось");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,2,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,2,1,1);
	block->tic_vertical = GTK_LABEL(label_value);

	label_name = gtk_label_new("Горизонтальная ось");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,3,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,3,1,1);
	block->tic_horizontal = GTK_LABEL(label_value);

	label_name = gtk_label_new("Энкодер вертикальной оси");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,4,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,4,1,1);
	block->encoder_vertical = GTK_LABEL(label_value);

	label_name = gtk_label_new("Энкодер горизонтальной оси");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,5,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,5,1,1);
	block->encoder_horizontal = GTK_LABEL(label_value);

	label_name = gtk_label_new("Давление");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,6,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,6,1,1);
	block->pressure = GTK_LABEL(label_value);

	label_name = gtk_label_new("Ток вертикальной оси");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,7,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,7,1,1);
	block->amperage_vertical = GTK_LABEL(label_value);

	label_name = gtk_label_new("Ток горизонтально оси");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,8,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,8,1,1);
	block->amperage_horizontal = GTK_LABEL(label_value);

	label_name = gtk_label_new("Заслонка");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,9,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,9,1,1);
	block->valve = GTK_LABEL(label_value);

	label_name = gtk_label_new("Заслонка положение");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,10,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,10,1,1);
	block->tic_valve = GTK_LABEL(label_value);

	label_name = gtk_label_new("Датчик пламени");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,11,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,11,1,1);
	block->fire_sensor = GTK_LABEL(label_value);

	label_name = gtk_label_new("Пожарная сигнализация");
	layout_widget(label_name,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	label_value = gtk_label_new(NULL);
	layout_widget(label_value,GTK_ALIGN_START,GTK_ALIGN_FILL,FALSE,TRUE);
	gtk_widget_show(label_name);
	gtk_widget_show(label_value);
	gtk_grid_attach(GTK_GRID(grid),label_name ,0,12,1,1);
	gtk_grid_attach(GTK_GRID(grid),label_value,1,12,1,1);
	block->fire_alarm = GTK_LABEL(label_value);

#endif
	gtk_container_add(GTK_CONTAINER(frame),grid);

	gtk_widget_show(frame);
	gtk_widget_show(grid);

	return frame;
}

static char STR_CONTROL[] = "Управление";
static GtkWidget * create_block_control(block_controller_s * block)
{
	GtkWidget * frame;
	frame = gtk_frame_new(STR_CONTROL);
	layout_widget(frame,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_widget_show(frame);
	return frame;
}


block_controller_s block_controller;

int show_block_controler(gpointer data)
{
	controller_s * controller = (controller_s*)data;

	return FALSE; /*завершить работу*/
	return TRUE; /* продолжаем работу */
}

int select_block_controller(controller_s * controller)
{
	state_controller_s * state;
	GtkLabel * label;
	/*через mutex*/
	all_controller.current = controller;

	if(controller == NULL){
		/*если таймер запушен то остановить*/
		return SUCCESS;
	}
	/*запустить таймер*/

	state = controller->state;
	label = block_controller.name;
	gtk_label_set_text(label,controller->name);
#if 1

	label = block_controller.lafet;
	g_string_printf(pub,"%#x",state->lafet);
	gtk_label_set_text(label,pub->str);

	label = block_controller.tic_vertical;
	g_string_printf(pub,"%#x",state->tic_vertical);
	gtk_label_set_text(label,pub->str);

	label = block_controller.tic_horizontal;
	g_string_printf(pub,"%#x",state->tic_horizontal);
	gtk_label_set_text(label,pub->str);

	label = block_controller.encoder_vertical;
	g_string_printf(pub,"%#x",state->encoder_vertical);
	gtk_label_set_text(label,pub->str);

	label = block_controller.encoder_horizontal;
	g_string_printf(pub,"%#x",state->encoder_horizontal);
	gtk_label_set_text(label,pub->str);

	label = block_controller.pressure;
	g_string_printf(pub,"%#x",state->pressure);
	gtk_label_set_text(label,pub->str);

	label = block_controller.amperage_vertical;
	g_string_printf(pub,"%#x",state->amperage_vertical);
	gtk_label_set_text(label,pub->str);

	label = block_controller.amperage_horizontal;
	g_string_printf(pub,"%#x",state->amperage_horizontal);
	gtk_label_set_text(label,pub->str);

	label = block_controller.valve;
	g_string_printf(pub,"%#x",state->valve);
	gtk_label_set_text(label,pub->str);

	label = block_controller.tic_valve;
	g_string_printf(pub,"%#x",state->tic_valve);
	gtk_label_set_text(label,pub->str);

	label = block_controller.fire_sensor;
	g_string_printf(pub,"%#x",state->fire_sensor);
	gtk_label_set_text(label,pub->str);

	label = block_controller.fire_alarm;
	g_string_printf(pub,"%#x",state->fire_alarm);
	gtk_label_set_text(label,pub->str);
#endif
	return SUCCESS;
}


GtkWidget * create_block_controller(void)
{
	GtkWidget * box;
	GtkWidget * frame_state;
	GtkWidget * frame_control;

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	layout_widget(box,GTK_ALIGN_FILL,GTK_ALIGN_FILL,TRUE,TRUE);
	gtk_box_set_homogeneous(GTK_BOX(box),TRUE);

	frame_state = create_block_state(&block_controller);

	frame_control = create_block_control(&block_controller);

	gtk_box_pack_start(GTK_BOX(box),frame_state,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(box),frame_control,TRUE,TRUE,0);

	gtk_widget_show(box);

	return box;
}

/*****************************************************************************/
