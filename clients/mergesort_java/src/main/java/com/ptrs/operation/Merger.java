package com.ptrs.operation;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonParser;
import com.ptrs.algorithm.MergeSort;
import com.ptrs.util.CalculationResultBlock;

public class Merger {
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();
	
	public static String mergeFromJson(String json) {
		JsonArray resultsArray = new JsonParser().parse(json).getAsJsonArray();
		for(int i = 0; i < resultsArray.size() - 1; i++) {
			JsonElement firstResultJson = resultsArray.get(i);
			JsonElement secondResultJson = resultsArray.get(i+1);
			
			int[] sortedArray = mergeTwoJsonElements(firstResultJson, secondResultJson);
			// TODO To finish
		}
		
		return gson.toJson(new CalculationResultBlock(0, null));
	}
	
	private static int[] mergeTwoJsonElements(JsonElement firstResultJson, JsonElement secondResultJson) {
		CalculationResultBlock calculationFirstResultBlock = gson.fromJson(firstResultJson, CalculationResultBlock.class);
		int[] firstResult = calculationFirstResultBlock.getResult();
		
		CalculationResultBlock calculationSecondResultBlock = gson.fromJson(firstResultJson, CalculationResultBlock.class);
		int[] secondResult = calculationSecondResultBlock.getResult();
		
		return MergeSort.merge(firstResult, secondResult);
	}
}
