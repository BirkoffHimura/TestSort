
#include "Main.h"


using namespace std;

void swap(vector<int> &v, int index1, int index2)
{
	int temp = static_cast<int>(v[index1]);
	v[index1] = static_cast<int>(v[index2]);
	v[index2] = static_cast<int>(temp);
}


vector<int> sortSlow(vector<int> v, const std::function <bool(int, int)>& f)
{
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = (1 + i); j < v.size(); j++)
		{
			if (f(v[i], v[j]))
			{
				swap(v, i, j);
			}
		}
	}
	return v;
}

int partitionSort(vector<int> &v, int b, int e, const std::function <bool(int, int)>& f)
{
	int pivot = v[e];
	int index = b - 1;
	for (int i = b; i <= e - 1; i++)
	{
		if (f(v[i], pivot))
		{
			index++;
			if (index != i)
			{
				swap(v, index, i);
			}
		}
	}
	if ((index + 1) != e)
	{
		swap(v, index + 1, e);
	}
	return index + 1;
}

void quicksort(vector<int> &v, int b, int e, const std::function <bool(int, int)>& f)
{
	if (b < e)
	{
		int p = partitionSort(v, b, e, f);

		quicksort(v, b, p - 1, f);
		quicksort(v, p + 1, e, f);
	}
}

void createFunction(napi_env env, napi_callback cb, const char * functionName, napi_value &result)
{
	//assert(napi_create_object(env, &result) == napi_ok);
	napi_value exported_function;
	assert(napi_create_function(env,
		functionName,
		NAPI_AUTO_LENGTH,
		cb,
		NULL,
		&exported_function) == napi_ok);
	assert(napi_set_named_property(env,
		result,
		functionName,
		exported_function) == napi_ok);
}
napi_value SortSimple(napi_env env, napi_callback_info info)
{
	napi_status status;
	size_t argc = 2;

	napi_value args[2];
	napi_valuetype valuetype0;
	napi_value sortF;

	status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
		

	status = napi_typeof(env, args[0], &valuetype0);

	napi_value ret;
	status = napi_create_array(env, &ret);
	uint32_t i, length;
	bool typed = false;
	std::vector<int> arr;

	napi_typedarray_type vt;
	int* data = nullptr;
	napi_value arrbuf;
	size_t size, offset, length2;

	napi_is_typedarray(env, args[0], &typed);
	if (!typed)
	{
		status = napi_get_array_length(env, args[0], &length);
		arrbuf = args[0];
		length2 = length;

		arr.reserve(length2);

		for (i = 0; i < length; i++)
		{
			napi_value e;
			int32_t tmp = 0;

			status = napi_get_element(env, args[0], i, &e);

			status = napi_get_value_int32(env, e, &tmp);

			arr.push_back(tmp);
		}
	}
	else
	{		
		status = napi_get_typedarray_info(env, args[0], &vt, &size, (void**)&data, &arrbuf, &offset);
		arr.reserve(size);

		arr = std::vector<int>(data, data + (size));
		
		length = size;
		length2 = length;
	}
	
	
		quicksort(arr, 0, length -1, [env, args](auto  v, auto v1) -> bool {
			napi_value val[2], val2, v_ret;
			napi_handle_scope scope;
			napi_status status = napi_open_handle_scope(env, &scope);

			napi_create_int32(env, v, &val[0]);
			napi_create_int32(env, v1, &val[1]);
			status = napi_call_function(env, args[1], args[1], 2, val, &v_ret);
			bool bret = false;
			status = napi_get_value_bool(env, v_ret, &bret);

			status = napi_close_handle_scope(env, scope);

			return bret;
		});
	


	for (i = 0; i < length; i++)
	{
		napi_value e;

		status = napi_create_int32(env, arr[i], &e);

		status = napi_set_element(env, ret, i, e);
	}

	return ret;
} 

napi_value create_addon(napi_env env) {
	napi_value result;
	assert(napi_create_object(env, &result) == napi_ok);
	napi_value exported_function;
	assert(napi_create_function(env,
		"sortSimple",
		NAPI_AUTO_LENGTH,
		SortSimple,
		NULL,
		&exported_function) == napi_ok);
	assert(napi_set_named_property(env,
		result,
		"sortSimple",
		exported_function) == napi_ok);
	
	return result;
}


static napi_value Init(napi_env env, napi_value exports) {
	return create_addon(env);
}

NAPI_MODULE(SortTest, Init)