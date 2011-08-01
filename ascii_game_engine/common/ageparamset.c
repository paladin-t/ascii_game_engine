/*
** This source file is part of MY-BASIC
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

#include "ageparamset.h"
#include "ageutil.h"

typedef enum AgeParamType {
	APT_S32,
	APT_U32,
	APT_F32,
	APT_STR,
} AgeParamType;

typedef struct AgeParam {
	AgeParamType type;
	union {
		s32 s32;
		u32 u32;
		f32 f32;
		Str str;
	};
} AgeParam;

static AgeParam* _create_param(AgeParamType _type) {
	AgeParam* result = AGE_MALLOC(AgeParam);
	result->type = _type;

	return result;
}

static void _destroy_param(AgeParam* _param) {
	if(_param->type == APT_STR) {
		AGE_FREE(_param->str);
	}
	AGE_FREE(_param);
}

static s32 _paramset_opt(Ptr _data, Ptr _extra) {
	s32 result = 0;
	AgeParam* par = 0;
	Str str = _extra;

	par = (AgeParam*)_data;
	_destroy_param(par);
	AGE_FREE(str);

	return result;
}

AgeParamSet* create_paramset(void) {
	AgeParamSet* result = 0;

	result = ht_create(PARAM_SET_SIZE, ht_cmp_string, ht_hash_string, _paramset_opt);

	return result;
}

void destroy_paramset(AgeParamSet* _ps) {
	ht_destroy(_ps);
}

bl set_s32_param(AgeParamSet* _ps, const Str _name, s32 _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	node = ht_find(_ps, _name);
	if(node) {
		par = node->data;
	} else {
		par = _create_param(APT_S32);
		ht_set_or_insert(_ps, copy_string(_name), par);
	}
	par->s32 = _data;

	return result;
}

bl get_s32_param(AgeParamSet* _ps, const Str _name, s32* _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	assert(_data);
	if(_data) {
		node = ht_find(_ps, _name);
		assert(node);
		if(node) {
			par = (AgeParam*)(node->data);
			assert(par->type == APT_S32);
			if(par->type == APT_S32) {
				*_data = par->s32;
			} else {
				result = FALSE;
			}
		} else {
			result = FALSE;
		}
	}

	return result;
}

bl set_u32_param(AgeParamSet* _ps, const Str _name, u32 _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	node = ht_find(_ps, _name);
	if(node) {
		par = node->data;
	} else {
		par = _create_param(APT_U32);
		ht_set_or_insert(_ps, copy_string(_name), par);
	}
	par->u32 = _data;

	return result;
}

bl get_u32_param(AgeParamSet* _ps, const Str _name, u32* _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	assert(_data);
	if(_data) {
		node = ht_find(_ps, _name);
		assert(node);
		if(node) {
			par = (AgeParam*)(node->data);
			assert(par->type == APT_U32);
			if(par->type == APT_U32) {
				*_data = par->u32;
			} else {
				result = FALSE;
			}
		} else {
			result = FALSE;
		}
	}

	return result;
}

bl set_f32_param(AgeParamSet* _ps, const Str _name, f32 _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	node = ht_find(_ps, _name);
	if(node) {
		par = node->data;
	} else {
		par = _create_param(APT_F32);
		ht_set_or_insert(_ps, copy_string(_name), par);
	}
	par->f32 = _data;

	return result;
}

bl get_f32_param(AgeParamSet* _ps, const Str _name, f32* _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	assert(_data);
	if(_data) {
		node = ht_find(_ps, _name);
		assert(node);
		if(node) {
			par = (AgeParam*)(node->data);
			assert(par->type == APT_F32);
			if(par->type == APT_F32) {
				*_data = par->f32;
			} else {
				result = FALSE;
			}
		} else {
			result = FALSE;
		}
	}

	return result;
}

bl set_str_param(AgeParamSet* _ps, const Str _name, Str _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	node = ht_find(_ps, _name);
	if(node) {
		par = node->data;
	} else {
		par = _create_param(APT_STR);
		ht_set_or_insert(_ps, copy_string(_name), par);
	}
	if(par->str) {
		AGE_FREE(par->str);
	}
	par->str = copy_string(_data);

	return result;
}

bl get_str_param(AgeParamSet* _ps, const Str _name, Str* _data) {
	bl result = TRUE;
	ls_node_t* node = 0;
	AgeParam* par = 0;

	assert(_data);
	if(_data) {
		node = ht_find(_ps, _name);
		assert(node);
		if(node) {
			par = (AgeParam*)(node->data);
			assert(par->type == APT_STR);
			if(par->type == APT_STR) {
				*_data = par->str;
			} else {
				result = FALSE;
			}
		} else {
			result = FALSE;
		}
	}

	return result;
}

bl remove_param(AgeParamSet* _ps, const Str _name) {
	bl result = TRUE;
	// TODO
	return result;
}

bl clear_param(AgeParamSet* _ps) {
	bl result = TRUE;

	ht_foreach(_ps, _paramset_opt);
	ht_clear(_ps);

	return result;
}
