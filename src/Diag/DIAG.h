#ifndef DIAG_h
#define DIAG_h

/**
 * @file Diag.h
 * @author Gregor Baues
 * @author Chris Harlow
 * @brief  Diagnotic/Debugging etc routines adapted from the original work from © 2020, Chris Harlow (aka Asbelos on github).
 * @version 0.1
 * @date 2020-07-08
 * 
 * @copyright Copyright (c) 2020; All rights reserved 
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details <https://www.gnu.org/licenses/>.
 */


#include "StringFormatter.h"
#include <Diag/MemoryFree.h>

#ifndef DEBUG
    #define DEBUG
    #ifndef LOGLEVEL
        #define LOGLEVEL 50
    #endif
#endif

#define DIAG  StringFormatter::print

#define LOGV_DEBUG 51
#define LOGV_ERROR 41
#define LOGV_INFO  31
#define LOGV_SILENT 0

#ifdef LOGLEVEL
    #if LOGLEVEL == LOGV_SILENT
        #define INFO( message... ) ;  
        #define DBG( message... ) ; 
        #define ERR( message... ) ;  
    #endif
    #if LOGLEVEL < LOGV_INFO && LOGLEVEL > 0
        #define INFO( message... ) { DIAG(F("::[INFO]:%d:%s:%d : "), freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }   
        #define DBG( message... ) ; 
        #define ERR( message... ) ; 
    #endif
    #if LOGLEVEL < LOGV_ERROR && LOGLEVEL > LOGV_INFO
        #define INFO( message... ) { DIAG(F("::[INFO]:%d:%s:%d : "),freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }   
        #define DBG( message... ) ; 
        #define ERR( message... ) { DIAG(F("::[ERROR]:%d:%s:%d : "), freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }  
    #endif
     #if LOGLEVEL < LOGV_DEBUG && LOGLEVEL > LOGV_ERROR
        #define INFO( message... ) { DIAG(F("::[INFO]:%d:%s:%d : "), freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }   
        #define DBG( message... )  { DIAG(F("::[DBG]:%d:%s:%d : "), freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }
        #define ERR( message... ) { DIAG(F("::[ERROR]:%d:%s:%d : "), freeMemory(), __FILE__, __LINE__); DIAG( message ); Serial.println(); }  
    #endif

 #endif

#endif