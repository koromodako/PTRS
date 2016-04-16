package com.ptrs.util;

public class CalculationBlock {
	
	private static final String BINARY_NAME = "mergesort.jar";
	
	private String bin;
	private String fragmentId;
	
	private CalculationBlockParams params;
	
	public CalculationBlock(String fragmentId, int[] values, int nbPartitions) {
		this(BINARY_NAME, fragmentId, new CalculationBlockParams(values, nbPartitions));
	}
	
	public CalculationBlock(String fragmentId, CalculationBlockParams params) {
		this(BINARY_NAME, fragmentId, params);
	}

	private CalculationBlock(String bin, String fragmentId, CalculationBlockParams params) {
		super();
		this.bin = bin;
		this.fragmentId = fragmentId;
		this.params = params;
	}

	public String getBin() {
		return bin;
	}

	public void setBin(String bin) {
		this.bin = bin;
	}

	public String getFragmentId() {
		return fragmentId;
	}

	public void setFragmentId(String fragmentId) {
		this.fragmentId = fragmentId;
	}

	public CalculationBlockParams getParams() {
		return params;
	}

	public void setParams(CalculationBlockParams params) {
		this.params = params;
	}
	
	

}
