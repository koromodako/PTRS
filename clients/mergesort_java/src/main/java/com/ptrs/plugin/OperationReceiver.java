package com.ptrs.plugin;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonObject;
import com.ptrs.operation.Calculator;
import com.ptrs.operation.Merger;
import com.ptrs.operation.Splitter;
import com.ptrs.util.CalculationBlockParams;

public class OperationReceiver {
	
	private static final String ACTION_JOIN = "join";
	private static final String ACTION_SPLIT = "split";
	private static final String ACTION_COMPUTE = "calc";
	private static final String ACTION_GET_PARAMS = "get_params";
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();

	public static void main(String[] args) {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		String line = null, action = null, json = "";
		try {
			action = reader.readLine();
			while ((line = reader.readLine()) != null) {
			    json += line;
			}
		} catch (IOException e) {
			System.err.println("An error occured while reading user's standard input : " + e.getMessage());
		}
		
		if(action == null) {
			System.err.println("Program exited before any command was given");
			System.exit(1);
		}
		
		String jsonResult = null;
		switch(action) {
			case ACTION_JOIN:
				jsonResult = Merger.mergeFromJson(json);
				break;
			case ACTION_SPLIT:
				jsonResult = Splitter.splitFromJson(json);
				break;
			case ACTION_COMPUTE:
				jsonResult = Calculator.mergeSortFromJson(json);
				break;
			case ACTION_GET_PARAMS:
				jsonResult = getAcceptedParameters();
				break;
			default:
				// Undefined action
				System.err.println("Undefined action, exiting. List of actions are split, calc, join, and get_params.");
				System.exit(1);
		}
		
		if(jsonResult != null) {
			System.out.println(jsonResult);
		}
		else {
			System.exit(1);
		}
		
		// Exit on success
		System.exit(0);
	}
	
	private static String getAcceptedParameters() {
		JsonObject jsonObject = new JsonObject();
		jsonObject.addProperty(CalculationBlockParams.PARAM_VALUES, CalculationBlockParams.PARAM_TYPE_VALUES);
		jsonObject.addProperty(CalculationBlockParams.PARAM_PARTITIONS, CalculationBlockParams.PARAM_TYPE_PARTITIONS);
		
		return gson.toJson(jsonObject);
	}
}
