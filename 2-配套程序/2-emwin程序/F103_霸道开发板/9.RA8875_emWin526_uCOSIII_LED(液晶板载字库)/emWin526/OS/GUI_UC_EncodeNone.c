/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              ��C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_UC_EncodeNone.c
Purpose     : Encoding routines for non unicode systems (default)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Type.h"

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetCharCode
*
* Purpose:
*   Return the UNICODE character code of the current character.
*/
static U16 _GetCharCode(const char GUI_UNI_PTR * s) {
    if((*s) > 0xA0)
    {
        U16 c = (s[0] << 8) | s[1];
        return c;
    }
    return *(const U8 GUI_UNI_PTR *)s;
}

/*********************************************************************
*
*       _GetCharSize
*
* Purpose:
*   Return the number of bytes of the current character.
*/
static int _GetCharSize(const char GUI_UNI_PTR * s) {
    GUI_USE_PARA(s);
    if((*s) > 0xA0)
    {
        return 2;
    }
    return 1;
}

/*********************************************************************
*
*       _CalcSizeOfChar
*
* Purpose:
*   Return the number of bytes needed for the given character.
*/
static int _CalcSizeOfChar(U16 Char) {
    GUI_USE_PARA(Char);
    if(Char > 0xA0A0)
    {
        return 2;
    }
    return 1;
}

/*********************************************************************
*
*       _Encode
*
* Purpose:
*   Encode character into 1/2/3 bytes.
*/
static int _Encode(char *s, U16 Char) {
    if(Char > 0xA0A0)
    {
        *s++ = (Char >> 8);
        *s++ = (Char & 0xFF);
        return 2;
    }
    *s = (U8)(Char);
    return 1;
}

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _API_Table
*/
const GUI_UC_ENC_APILIST GUI_UC_None = {
  _GetCharCode,     /*  return character code as U16 */
  _GetCharSize,     /*  return size of character: 1 */
  _CalcSizeOfChar,  /*  return size of character: 1 */
  _Encode           /*  Encode character */
};


/*********************************************************************
*
*       GUI_UC_SetEncodeNone
*/
void GUI_UC_SetEncodeNone(void) {
  #if GUI_SUPPORT_UNICODE
    GUI_LOCK();
    GUI_Context.pUC_API = &GUI_UC_None;
    GUI_UNLOCK();
  #endif
}

