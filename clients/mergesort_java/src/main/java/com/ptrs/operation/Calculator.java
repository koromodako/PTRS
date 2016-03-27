package com.ptrs.operation;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
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
		JsonElement paramsJson = new JsonParser().parse(json).getAsJsonObject();
		
		CalculationBlock calculationBlock = gson.fromJson(paramsJson, CalculationBlock.class);
		CalculationBlockParams cbParams = calculationBlock.getParams();
		
		int[] arrayToSort = cbParams.getValues();
		
		// Merge sort it
		MergeSort.mergeSort(arrayToSort);
		
		CalculationResultBlock calculationResultBlock = new CalculationResultBlock(calculationBlock.getFragmentId(), arrayToSort);
		
		return gson.toJson(calculationResultBlock);
	}
	
}
