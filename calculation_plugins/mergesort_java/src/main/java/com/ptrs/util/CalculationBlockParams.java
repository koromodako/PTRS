package com.ptrs.util;

public class CalculationBlockParams {
	
	public static final String PARAM_VALUES = "values";
	public static final String PARAM_TYPE_VALUES = "array";
	public static final String PARAM_PARTITIONS = "partitions";
	public static final String PARAM_TYPE_PARTITIONS = "int";
	
	private int[] values;
	private int partitions;
	
	public CalculationBlockParams(int[] values) {
		super();
		this.values = values;
		this.partitions = -1;
	}
	
	public CalculationBlockParams(int[] values, int partitions) {
		super();
		this.values = values;
		this.partitions = partitions;
	}
	
	public int[] getValues() {
		return values;
	}
	public void setValues(int[] values) {
		this.values = values;
	}
	public int getPartitions() {
		return partitions;
	}
	public void setPartitions(int partitions) {
		this.partitions = partitions;
	}
	
	
}
