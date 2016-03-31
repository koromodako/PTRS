package com.ptrs.operation;

import java.util.ArrayList;
import java.util.List;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;
import com.ptrs.json.PtrsConstants;
import com.ptrs.util.CalculationBlock;
import com.ptrs.util.CalculationBlockParams;

public class Splitter {
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();
	
	public static String splitFromJson(String json) {
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
			return null;
		}
		
		JsonElement paramsJson = jsonElement.getAsJsonObject().get(PtrsConstants.CALC_PARAMS);
		CalculationBlockParams cbParams = gson.fromJson(paramsJson, CalculationBlockParams.class);
		
		if(cbParams == null) {
			return null;
		}
		
		int nbPartitions = cbParams.getPartitions();
		if(nbPartitions == 0) {
			// TODO Get number of nodes in the cluster
		}
		
		int[] valuesToSplit = cbParams.getValues();
		int valuesLength = valuesToSplit.length;
		
		// If the requested number of partition is actually greater than the length of the actual values, just take the length
		if(nbPartitions > valuesLength) {
			nbPartitions = valuesLength;
		}
		
		int nbValuesPerPartition = valuesLength / nbPartitions;
		int extraValues = valuesLength % nbPartitions;
		
		List<CalculationBlock> calculationBlocks = new ArrayList<>();
		
		for(int i = 0; i < nbPartitions; i++) {
			int partitionPosition = i * nbValuesPerPartition;
			int nbValues = nbValuesPerPartition;
			
			// Add last values to last partition
			if(partitionPosition == nbPartitions - 1) {
				nbValues += extraValues;
			}
			
			// Copy partition values into designated array
			int[] partitionedArray = new int[nbValues];
			System.arraycopy(valuesToSplit, partitionPosition, partitionedArray, 0, nbValues);
			
			CalculationBlock calculationBlock = new CalculationBlock(i, new CalculationBlockParams(partitionedArray));
			calculationBlocks.add(calculationBlock);
		}
		
		return gson.toJson(calculationBlocks);
	}
	
}
