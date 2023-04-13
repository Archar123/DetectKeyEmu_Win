#include <intrin.h>
#include <ntifs.h>

#ifndef CUSTOMTYPES
#define CUSTOMTYPES
typedef ULONG_PTR QWORD;
typedef ULONG DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
#endif


typedef struct {
	QWORD base, size;
} IMAGE_INFO_TABLE;

//
// vmware
//
IMAGE_INFO_TABLE vmusbmouse;

//
// DriverEntry/DriverUnload
//
PDRIVER_OBJECT gDriverObject;

// MouseClassServiceCallbackHook
//
#pragma warning(disable : 4201)
typedef struct _MOUSE_INPUT_DATA {
	USHORT UnitId;
	USHORT Flags;
	union {
		ULONG Buttons;
		struct {
			USHORT ButtonFlags;
			USHORT ButtonData;
		};
	};
	ULONG  RawButtons;
	LONG   LastX;
	LONG   LastY;
	ULONG  ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;

typedef QWORD
(*MouseClassServiceCallbackFn)(
	PDEVICE_OBJECT DeviceObject,
	PMOUSE_INPUT_DATA InputDataStart,
	PMOUSE_INPUT_DATA InputDataEnd,
	PULONG InputDataConsumed
	);

typedef struct _MOUSE_OBJECT
{
	PDEVICE_OBJECT              mouse_device;
	MouseClassServiceCallbackFn service_callback;
	BOOL                        hook;
	QWORD                       hid;
	QWORD                       hid_length;
} MOUSE_OBJECT, * PMOUSE_OBJECT;

MOUSE_OBJECT gMouseObject;

BOOL mouse_hook(void);
void mouse_unhook(void);

QWORD GetModuleHandle(PWCH module_name, QWORD* SizeOfImage);

extern "C"
NTKERNELAPI
PCHAR
PsGetProcessImageFileName(PEPROCESS Process);

extern "C"
NTSYSCALLAPI
NTSTATUS
ObReferenceObjectByName(
	__in PUNICODE_STRING ObjectName,
	__in ULONG Attributes,
	__in_opt PACCESS_STATE AccessState,
	__in_opt ACCESS_MASK DesiredAccess,
	__in POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__inout_opt PVOID ParseContext,
	__out PVOID* Object
);

extern "C"
NTSYSCALLAPI
POBJECT_TYPE* IoDriverObjectType;

PVOID callback_addr = NULL;
MouseClassServiceCallbackFn callback_addr_org = NULL;

QWORD MouseClassServiceCallbackHook(
	PDEVICE_OBJECT DeviceObject,
	PMOUSE_INPUT_DATA InputDataStart,
	PMOUSE_INPUT_DATA InputDataEnd,
	PULONG InputDataConsumed)
{
	QWORD address = (QWORD)_ReturnAddress();
	if (address < (QWORD)gMouseObject.hid || address >(QWORD)((QWORD)gMouseObject.hid + gMouseObject.hid_length))
	{
		// extra check for vmware virtual machine
		//if (address < (QWORD)vmusbmouse.base || address >(QWORD)((QWORD)vmusbmouse.base + vmusbmouse.size))
		{
			QWORD thread = (QWORD)PsGetCurrentThread();
			QWORD host_process = *(QWORD*)(thread + 0x220);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][%s][%ld] Thread is manipulating mouse [%llx]\n",
				PsGetProcessImageFileName((PEPROCESS)host_process),
				(DWORD)(QWORD)PsGetThreadId((PETHREAD)thread),
				thread
			);
		}
	}

	return callback_addr_org(DeviceObject,
		InputDataStart,
		InputDataEnd,
		InputDataConsumed);
}


void NtSleep(DWORD milliseconds)
{
	QWORD ms = milliseconds;
	ms = (ms * 1000) * 10;
	ms = ms * -1;
	KeDelayExecutionThread(KernelMode, 0, (PLARGE_INTEGER)&ms);
}

VOID
DriverUnload(
	_In_ struct _DRIVER_OBJECT* DriverObject)
{
	(DriverObject);

	mouse_unhook();

	NtSleep(1000);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][+] Anti-Cheat.sys is closed\n");
}

extern "C"
NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject,
	_In_ PUNICODE_STRING RegistryPath)
{
	(DriverObject);
	(RegistryPath);

	gDriverObject = DriverObject;
	DriverObject->DriverUnload = DriverUnload;

	//vmusbmouse.sys   vmmouse.sys
	vmusbmouse.base = GetModuleHandle(L"vmusbmouse.sys", &vmusbmouse.size);

	mouse_hook();
	return STATUS_SUCCESS;
}

typedef struct _KLDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	VOID* ExceptionTable;
	UINT32 ExceptionTableSize;
	VOID* GpValue;
	VOID* NonPagedDebugInfo;
	VOID* ImageBase;
	VOID* EntryPoint;
	UINT32 SizeOfImage;
	UNICODE_STRING FullImageName;
	UNICODE_STRING BaseImageName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

QWORD GetModuleHandle(PWCH module_name, QWORD* SizeOfImage)
{
	PLDR_DATA_TABLE_ENTRY ldr = (PLDR_DATA_TABLE_ENTRY)gDriverObject->DriverSection;
	for (PLIST_ENTRY pListEntry = ldr->InLoadOrderLinks.Flink; pListEntry != &ldr->InLoadOrderLinks; pListEntry = pListEntry->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (pEntry->BaseImageName.Buffer && wcscmp(pEntry->BaseImageName.Buffer, module_name) == 0) {
			if (SizeOfImage) {
				*SizeOfImage = 0;
				*SizeOfImage = pEntry->SizeOfImage;
			}
			return (QWORD)pEntry->ImageBase;
		}
	}
	return 0;
}


BOOL IsInValidModuleRange(QWORD address)
{
	PLDR_DATA_TABLE_ENTRY ldr = (PLDR_DATA_TABLE_ENTRY)gDriverObject->DriverSection;
	for (PLIST_ENTRY pListEntry = ldr->InLoadOrderLinks.Flink; pListEntry != &ldr->InLoadOrderLinks; pListEntry = pListEntry->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (pEntry->ImageBase == 0)
			continue;

		if (address >= (QWORD)pEntry->ImageBase && address <= (QWORD)((QWORD)pEntry->ImageBase + pEntry->SizeOfImage + 0x1000))
		{
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][*] InModule= %wZ\n", pEntry->BaseImageName);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL mouse_hook(void)
{
	// https://github.com/nbqofficial/norsefire
	if (gMouseObject.hook == 0) {

		UNICODE_STRING class_string;
		RtlInitUnicodeString(&class_string, L"\\Driver\\MouClass");


		PDRIVER_OBJECT class_driver_object = NULL;
		NTSTATUS status = ObReferenceObjectByName(&class_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&class_driver_object);
		if (!NT_SUCCESS(status)) {
			return 0;
		}

		UNICODE_STRING hid_string;
		RtlInitUnicodeString(&hid_string, L"\\Driver\\MouHID");


		PDRIVER_OBJECT hid_driver_object = NULL;

		status = ObReferenceObjectByName(&hid_string, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&hid_driver_object);
		if (!NT_SUCCESS(status))
		{
			if (class_driver_object) {
				ObfDereferenceObject(class_driver_object);
			}
			return 0;
		}

		gMouseObject.hid = (QWORD)hid_driver_object->DriverStart;
		gMouseObject.hid_length = (QWORD)hid_driver_object->DriverSize;

		PVOID class_driver_base = NULL;


		PDEVICE_OBJECT hid_device_object = hid_driver_object->DeviceObject;
		while (hid_device_object && !gMouseObject.service_callback)
		{
			PDEVICE_OBJECT class_device_object = class_driver_object->DeviceObject;
			while (class_device_object && !gMouseObject.service_callback)
			{
				if (!class_device_object->NextDevice && !gMouseObject.mouse_device)
				{
					gMouseObject.mouse_device = class_device_object;
				}

				PULONG_PTR device_extension = (PULONG_PTR)hid_device_object->DeviceExtension;
				ULONG_PTR device_ext_size = ((ULONG_PTR)hid_device_object->DeviceObjectExtension - (ULONG_PTR)hid_device_object->DeviceExtension) / 4;
				class_driver_base = class_driver_object->DriverStart;
				for (ULONG_PTR i = 0; i < device_ext_size; i++)
				{
					if (device_extension[i] == (ULONG_PTR)class_device_object && device_extension[i + 1] > (ULONG_PTR)class_driver_object)
					{
						gMouseObject.service_callback = (MouseClassServiceCallbackFn)(device_extension[i + 1]);
						callback_addr = &(device_extension[i + 1]);
						DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][+] callback_pointer_addr = %p, org_service_callback = %p\n",
							callback_addr, gMouseObject.service_callback);
						break;
					}
				}
				class_device_object = class_device_object->NextDevice;
			}
			hid_device_object = hid_device_object->AttachedDevice;
		}

		if (!gMouseObject.mouse_device)
		{
			PDEVICE_OBJECT target_device_object = class_driver_object->DeviceObject;
			while (target_device_object)
			{
				if (!target_device_object->NextDevice)
				{
					gMouseObject.mouse_device = target_device_object;
					break;
				}
				target_device_object = target_device_object->NextDevice;
			}
		}

		ObfDereferenceObject(class_driver_object);
		ObfDereferenceObject(hid_driver_object);

		if (gMouseObject.mouse_device && gMouseObject.service_callback)
        {		
			callback_addr_org = (MouseClassServiceCallbackFn)InterlockedExchangePointer((PVOID*)callback_addr, (void*)MouseClassServiceCallbackHook);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][*] callback_pointer_addr = %p, org_service_callback = %p, current_service_callback = %p\n", 
				callback_addr, callback_addr_org, MouseClassServiceCallbackHook);

			IsInValidModuleRange((QWORD)callback_addr_org);
			gMouseObject.hook = 1;

			return 1;
		}
	}
	else {
		return 1;
	}

	return 0;
}

void mouse_unhook(void)
{
	if (gMouseObject.hook)
	{
		gMouseObject.hook = 0;
		//保存的和现在是否一致，如果不一致,判断地址还是否存在，如果不存在就不管了，如果还存在模块，则替换
		if (callback_addr_org != (MouseClassServiceCallbackFn)((PVOID*)callback_addr))
		{
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[Anti][!] callback_addr is Modify.org_service_callback=%p, current_service_callback=%p\n",
				callback_addr_org, (PVOID*)callback_addr);

            if (!MmIsAddressValid((PVOID)callback_addr_org))
            {
				return;
            }

            if (!IsInValidModuleRange((QWORD)callback_addr_org))
            {
				return;
            }
            
		}

        InterlockedExchangePointer((PVOID*)callback_addr, (void*)callback_addr_org);
	}
}