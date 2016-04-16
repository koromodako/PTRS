package com.ptrs.util;

public class CalculationBlockParams {
	
	public static final String PARAM_VALUES = "values";
	public static final String PARAM_TYPE_VALUES = "array";
	public static final String PARAM_REGEX_VALUES = "\\[([1-9]\\d*,)*[1-9]\\d*\\]";
	public static final String PARAM_TOOLTIP_VALUES = "List of numbers between brackets, example: [1,3,8,2]";
	public static final String PARAM_PARTITIONS = "partitions";
	public static final String PARAM_TYPE_PARTITIONS = "int";
	public static final int PARAM_MIN_PARTITIONS = 1; //TODO change this to 0 when supported...
	
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
