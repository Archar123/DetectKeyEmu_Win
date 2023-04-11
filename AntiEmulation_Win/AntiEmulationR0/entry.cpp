#include "globals.h"

VOID driverUnload(IN PDRIVER_OBJECT driver_object)
{
	uninit_hook();

	DbgPrintEx(0, 0, "MouseClassServiceCallback Finish.\n");
}

extern "C" 
NTSTATUS DriverEntry(IN PDRIVER_OBJECT driver_object, IN PUNICODE_STRING registry_path)
{
	if (!NT_SUCCESS(init_mouse_service()))
		return STATUS_UNSUCCESSFUL;

	if(!NT_SUCCESS(!init_hook()))
		return STATUS_UNSUCCESSFUL;

	driver_object->DriverUnload = driverUnload;
	DbgPrintEx(0, 0, "MouseClassServiceCallback Start.\n");
	return STATUS_SUCCESS;	
}
