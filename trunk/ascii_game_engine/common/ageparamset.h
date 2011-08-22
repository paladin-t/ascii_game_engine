/*
** This source file is part of AGE
**
** For the latest info, see http://code.google.com/p/ascii-game-engine/
**
** Copyright (c) 2011 Tony & Tony's Toy Game Development Team
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __AGE_PARAM_SET_H__
#define __AGE_PARAM_SET_H__

#include "../ageconfig.h"
#include "agetype.h"
#include "ageallocator.h"
#include "agehashtable.h"

#define PARAM_SET_SIZE 193

/**
 * @brief parameter set
 */
typedef ht_node_t AgeParamSet;

/**
 * @brief create a parameter set object
 *
 * @return - created parameter set object
 */
AGE_API AgeParamSet* create_paramset(void);
/**
 * @brief destroy a parameter set object
 *
 * @param[in] _ps - parameter set object to be destroyed
 */
AGE_API void destroy_paramset(AgeParamSet* _ps);

/**
 * @brief set a s32 typed parameter
 *
 * @param[in] _ps   - parameter set object
 * @param[in] _name - parameter name to be operated
 * @param[in] _data - source data
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_s32_param(AgeParamSet* _ps, const Str _name, s32 _data);
/**
 * @brief get a s32 typed parameter
 *
 * @param[in] _ps    - parameter set object
 * @param[in] _name  - parameter name to be operated
 * @param[out] _data - target data pointer
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl get_s32_param(AgeParamSet* _ps, const Str _name, s32* _data);

/**
 * @brief set a u32 typed parameter
 *
 * @param[in] _ps   - parameter set object
 * @param[in] _name - parameter name to be operated
 * @param[in] _data - source data
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_u32_param(AgeParamSet* _ps, const Str _name, u32 _data);
/**
 * @brief get a u32 typed parameter
 *
 * @param[in] _ps    - parameter set object
 * @param[in] _name  - parameter name to be operated
 * @param[out] _data - target data pointer
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl get_u32_param(AgeParamSet* _ps, const Str _name, u32* _data);

/**
 * @brief set a f32 typed parameter
 *
 * @param[in] _ps   - parameter set object
 * @param[in] _name - parameter name to be operated
 * @param[in] _data - source data
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_f32_param(AgeParamSet* _ps, const Str _name, f32 _data);
/**
 * @brief get a f32 typed parameter
 *
 * @param[in] _ps    - parameter set object
 * @param[in] _name  - parameter name to be operated
 * @param[out] _data - target data pointer
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl get_f32_param(AgeParamSet* _ps, const Str _name, f32* _data);

/**
 * @brief set a Str typed parameter
 *
 * @param[in] _ps   - parameter set object
 * @param[in] _name - parameter name to be operated
 * @param[in] _data - source data
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_str_param(AgeParamSet* _ps, const Str _name, Str _data);
/**
 * @brief get a Str typed parameter
 *
 * @param[in] _ps    - parameter set object
 * @param[in] _name  - parameter name to be operated
 * @param[out] _data - target data pointer
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl get_str_param(AgeParamSet* _ps, const Str _name, Str* _data);

/**
 * @brief remove a parameter in a set
 *
 * @param[in] _ps   - parameter set object
 * @param[in] _name - parameter name to be removed
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl remove_param(AgeParamSet* _ps, const Str _name);
/**
 * @brief clear all parameters in a set
 *
 * @param[in] _ps - parameter set object
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl clear_param(AgeParamSet* _ps);

#endif /* __AGE_PARAM_SET_H__ */
