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

#include <gtk/gtk.h>

/*****************************************************************************/
/*    Общие переменые                                                        */
/*****************************************************************************/

char STR_NAME_PROGRAMM[] = "Система Управления Лафетными Стволами";

char STR_TYPE_GROUP[] = "группа";
char STR_TYPE_VIDEOCAMERE[] = "видеокамера";
char STR_TYPE_CONTROLLER[] = "контроллер";

char MESSAGE_STATUS_OFF[]           = "Нет управления";
char MESSAGE_STATUS_ON_NORM[]       = "Норма";
char MESSAGE_STATUS_ON_CRASH[]      = "Авария";
char MESSAGE_STATUS_ON_WARNING[]    = "Предупреждение";
char MESSAGE_STATUS_ON_LINK_OFF[]   = "Нет подключения";

GdkRGBA color_black =      {0    ,0    ,0    ,1};
GdkRGBA color_green =      {0    ,1    ,0    ,1};
GdkRGBA color_red =        {1    ,0    ,0    ,1};
GdkRGBA color_white =      {1    ,1    ,1    ,1};
GdkRGBA color_lite_blue  = {0.2  ,0.1  ,1    ,1};
GdkRGBA color_lite_red   = {1    ,0.2  ,0.1  ,1};
GdkRGBA color_lite_green = {0.2  ,1    ,0.1  ,1};
GdkRGBA color_background = {0.36 ,0.462,0.56 ,1};

char STR_EMPTY[] = "empty";
/*****************************************************************************/

char STR_MODULE_MAIN_WINDOW[] = "main_window";
/*****************************************************************************/

