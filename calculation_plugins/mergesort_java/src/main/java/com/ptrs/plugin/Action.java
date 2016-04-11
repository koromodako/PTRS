package com.ptrs.plugin;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.ptrs.util.CalculationBlockParams;

public class Action {
	
	public static final String JOIN = "join";
	public static final String SPLIT = "split";
	public static final String COMPUTE = "calc";
	public static final String GET_PARAMS = "get_params";
	
	public static final String PARAMS_NAME = "name";
	public static final String PARAMS_TYPE = "type";
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();
	
	public static String getAcceptedParameters() {
		JsonArray jsonArray = new JsonArray();
		JsonObject jsonObject = new JsonObject();
		jsonObject.addProperty(PARAMS_NAME, CalculationBlockParams.PARAM_VALUES);
		jsonObject.addProperty(PARAMS_TYPE, CalculationBlockParams.PARAM_TYPE_VALUES);
		jsonArray.add(jsonObject);
		
		jsonObject = new JsonObject();
		jsonObject.addProperty(PARAMS_NAME,CalculationBlockParams.PARAM_PARTITIONS);
		jsonObject.addProperty(PARAMS_TYPE, CalculationBlockParams.PARAM_TYPE_PARTITIONS);
		jsonArray.add(jsonObject);
		
		return gson.toJson(jsonArray);
	}

}
