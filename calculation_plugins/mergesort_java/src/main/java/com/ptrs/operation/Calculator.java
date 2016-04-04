package com.ptrs.operation;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;
import com.ptrs.algorithm.MergeSort;
import com.ptrs.util.CalculationBlock;
import com.ptrs.util.CalculationBlockParams;
import com.ptrs.util.CalculationResultBlock;

public class Calculator {
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();

	public static String mergeSortFromJson(String json) {
		JsonElement jsonElement = null;
		try {
			jsonElement = new JsonParser().parse(json);
		} 
		catch (JsonParseException e) {
			System.err.println("Misformed JSON, can't parse it : " + e.getMessage());
			return null;
		}
		
		if(jsonElement == null || !jsonElement.isJsonObject()) {
			System.err.println("Misformed JSON, can't further proceed fragment : " + json);
			if(jsonElement.isJsonArray()) {
				System.err.println("Fragment needs to be a single JSON object, not an array");
			}
			return null;
		}
		
		JsonElement paramsJson = jsonElement.getAsJsonObject();
		
		CalculationBlock calculationBlock = gson.fromJson(paramsJson, CalculationBlock.class);
		
		if(calculationBlock == null) {
			return null;
		}
		
		CalculationBlockParams cbParams = calculationBlock.getParams();
		
		int[] arrayToSort = cbParams.getValues();
		
		// Merge sort it
		MergeSort.mergeSort(arrayToSort);
		
		CalculationResultBlock calculationResultBlock = new CalculationResultBlock(calculationBlock.getFragmentId(), arrayToSort);
		
		return gson.toJson(calculationResultBlock);
	}
	
}
