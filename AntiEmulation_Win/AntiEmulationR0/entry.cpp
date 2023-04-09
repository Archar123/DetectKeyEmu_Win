#include "globals.h"

extern "C" 
NTSTATUS DriverEntry(PVOID a1, PVOID a2)
{
	if (!NT_SUCCESS(init_mouse_service()))
		return STATUS_UNSUCCESSFUL;

	if(!NT_SUCCESS(!init_hook()))
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;	
}
