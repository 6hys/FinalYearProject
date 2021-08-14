// Fill out your copyright notice in the Description page of Project Settings.


#include "SeedData.h"

bool FSeedData::operator==(const FSeedData& b) const
{
	// only check for the row name when comparing
	return(RowName == b.RowName);
}

bool FSeedData::operator!=(const FSeedData& b) const
{
	// only check for the row name when comparing
	return(RowName != b.RowName);
}