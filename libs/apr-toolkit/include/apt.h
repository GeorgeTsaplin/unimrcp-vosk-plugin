/*
 * Copyright 2008 Arsen Chaloyan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __APT_H__
#define __APT_H__

/**
 * @file apt.h
 * @brief APR Toolkit Definitions
 */ 

#include <apr.h>
#include <apr_pools.h>
#include <apr_strings.h>

/* extern "C" defines */
#ifdef __cplusplus
#define APT_BEGIN_EXTERN_C       extern "C" {
#define APT_END_EXTERN_C         }
#else
#define APT_BEGIN_EXTERN_C
#define APT_END_EXTERN_C
#endif 

/** lib export/import defines (win32) */
#ifdef WIN32
#ifdef APT_STATIC_LIB
#define APT_DECLARE(type)   type __stdcall
#else
#ifdef APT_LIB_EXPORT
#define APT_DECLARE(type)   __declspec(dllexport) type __stdcall
#else
#define APT_DECLARE(type)   __declspec(dllimport) type __stdcall
#endif
#endif
#else
#define APT_DECLARE(type) type
#endif

/** Boolean value */
typedef int apt_bool_t;

static APR_INLINE apt_bool_t apt_str_compare(const char *str1, const char *str2)
{
	return (strcasecmp(str1,str2) == 0) ? TRUE : FALSE;
}

#endif /*__APT_H__*/