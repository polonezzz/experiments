
#include "stdafx.h"

#include <iostream>
#include <atlbase.h>
#include <wbemcli.h>
#include <comdef.h> 

int main(int argc, char* argv[])
{
    bool ret = SUCCEEDED(CoInitializeEx(0, COINIT_MULTITHREADED));
	{
		HRESULT hr;
		 
		wchar_t networkResource[] = L"\\\\media\\root\\virtualization\\v2";
		//_bstr_t networkResource = L"\\\\media\\root\\cimv2";
		wchar_t domain[] = L"media"; 
		//_bstr_t user = L"Администратор";
		//_bstr_t login = L"media\\Администратор";
		wchar_t user[] = L"WMI";
		wchar_t login[] = L"media\\WMI";
		wchar_t pwd[] = L"autoplay";
		wchar_t vmName[] = L"AutoTest-Servise";
		 
		 ret = ret && SUCCEEDED(CoInitializeSecurity(
			 NULL,
			 -1,                          // COM authentication
			 NULL,                        // Authentication services
			 NULL,                        // Reserved
			 RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
			 RPC_C_IMP_LEVEL_IDENTIFY,    // Default Impersonation  
			 NULL,                        // Authentication info
			 EOAC_NONE,                   // Additional capabilities
			 NULL                         // Reserved
			 ));

		 CComPtr<IWbemLocator> locator;
		 ret = ret && SUCCEEDED(CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator,(LPVOID*)&locator));

		 CComPtr<IWbemServices> services;
		 ret = ret && SUCCEEDED(hr = locator->ConnectServer(
			 _bstr_t(networkResource),         // Object path of WMI namespace
			 _bstr_t(login),                    // User name. NULL = current user
			 _bstr_t(pwd),					  // User password. NULL = current
			 0,                       // Locale. NULL indicates current
			 NULL,                    // Security flags.
			 0,                       // Authority (e.g. Kerberos)
			 0,                       // Context object
			 &services                // pointer to IWbemServices proxy
			 ));
		
		COAUTHIDENTITY authIdent;
		memset(&authIdent, 0, sizeof(COAUTHIDENTITY));
    	authIdent.User = (USHORT*)user;
        authIdent.UserLength = wcslen(user);
		authIdent.Password = (USHORT*)pwd;
		authIdent.PasswordLength = wcslen(pwd);
        authIdent.Domain = (USHORT*)domain;
        authIdent.DomainLength = wcslen(domain);
        authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
		
		ret = ret && SUCCEEDED(hr = CoSetProxyBlanket(
			services,						// Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&authIdent,						// client identity
			EOAC_NONE						// proxy capabilities
			));
		/*
		 CComPtr<IEnumWbemClassObject> pEnumerator1;
		 ret = ret && SUCCEEDED(hr = services->ExecQuery(L"WQL",L"SELECT * FROM Msvm_SystemDevice",
			 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator1));
		 
		 ret = ret && SUCCEEDED(hr = CoSetProxyBlanket(
			pEnumerator1,						// Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&authIdent,						// client identity
			EOAC_NONE						// proxy capabilities
			));
		  
		  ULONG uReturn = 0;
		  while (pEnumerator1)
		  {
			CComPtr<IWbemClassObject> cim;
			hr = pEnumerator1->Next(WBEM_INFINITE,1,&cim,&uReturn);
			CComVariant vtg, vtp;
			hr = cim->Get(L"GroupComponent",0,&vtg,0,0);
			hr = cim->Get(L"PartComponent",0,&vtp,0,0);
			CComBSTR cimg = vtg.bstrVal;
			CComBSTR cimp = vtp.bstrVal;
		  }
		*/
		 CComPtr<IEnumWbemClassObject> pEnumerator;
		 ret = ret && SUCCEEDED(hr = services->ExecQuery(L"WQL",L"SELECT * FROM Msvm_ComputerSystem",
			 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator));
		 
		 ret = ret && SUCCEEDED(hr = CoSetProxyBlanket(
			pEnumerator,						// Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name 
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx 
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&authIdent,						// client identity
			EOAC_NONE						// proxy capabilities
			));
	
		 ULONG uReturn = 0;
		 while(pEnumerator)
		 {
			 CComPtr<IWbemClassObject> pclsObj;

			 if(FAILED(pEnumerator->Next(WBEM_INFINITE,1,&pclsObj,&uReturn)) || 0 == uReturn)
				 break;

			 CComVariant vtProp;
			 if (SUCCEEDED(hr = pclsObj->Get(L"ElementName",0,&vtProp,0,0)))
			 {
				_bstr_t vm = vtProp.bstrVal;
				if (vm == _bstr_t(vmName))
				{
					//CComPtr<IWbemClassObject> vmObj;
					//ret = ret && SUCCEEDED(hr = pclsObj->SpawnInstance(0, &vmObj));

					CComVariant vtPath;
					hr = pclsObj->Get(L"__PATH",0, &vtPath, 0, 0);
					CComBSTR objPath = vtPath.bstrVal;

					CComPtr<IWbemClassObject> inst, inParams, outParams;
					
					hr = services->GetObjectW(L"Msvm_ComputerSystem", WBEM_FLAG_RETURN_WBEM_COMPLETE, NULL, &inst, NULL);

					{
						SAFEARRAY* pNames = nullptr; 
						ret = ret && SUCCEEDED(hr = inst->GetNames(NULL, 0, NULL, &pNames));
					
						LONG count = 0;
						BSTR pName = nullptr;
						SafeArrayGetUBound(pNames, 1, &count);
						SafeArrayDestroy(pNames);
					}
					
					ret = ret && SUCCEEDED(hr = inst->GetMethod(L"RequestStateChange", 0, &inParams, &outParams));
					hr = inParams->BeginEnumeration(WBEM_FLAG_NONSYSTEM_ONLY);

					CComBSTR param;
					CComVariant value;
					CIMTYPE type;
					while (WBEM_S_NO_ERROR == inParams->Next(0, &param, &value, &type, 0))
					{
						if (param == L"RequestedState")
						{
							value = 3;
							hr = inParams->Put(param, 0, &value, type);
							break;
						}
					}
					hr = inParams->EndEnumeration();
					hr = services->ExecMethod(objPath, L"RequestStateChange", 0, NULL, inParams, NULL, NULL);

					CComPtr<IWbemClassObject> vms, in, out;
					ret = ret && SUCCEEDED(hr = services->GetObjectW(L"Msvm_VirtualSystemManagementService", 
								WBEM_FLAG_RETURN_WBEM_COMPLETE, NULL, &vms, NULL));
					
					CComBSTR methName;
					hr = vms->BeginMethodEnumeration(0);
					while (WBEM_S_NO_ERROR == vms->NextMethod(0, &methName, &in, &out))
					{
						if (methName == L"DestroySystem")
							break;
						in.Release();
						out.Release();
					}
					hr = vms->EndMethodEnumeration();

				
					CComVariant vmPath;
					CIMTYPE vmType, vmType2;
					ret = ret && SUCCEEDED(hr = in->Get(L"AffectedSystem",0,0,&vmType,0));
					ret = ret && SUCCEEDED(hr = in->Get(L"Job",0,0,&vmType2,0));

					//Msvm_ComputerSystem.CreationClassName="Msvm_ComputerSystem",Name="48833A94-2091-4CAD-ABD7-CCE6B8365DBA"
					objPath = L"Msvm_ComputerSystem.CreationClassName=\"Msvm_ComputerSystem\",Name=\"1639C12F-1F64-4564-8F98-125E3892B391\"";
					vmPath = objPath;
					
					hr = in->Put(L"AffectedSystem", 0, &vmPath, vmType);
					hr = services->ExecMethod(L"Msvm_VirtualSystemManagementService", methName, 0, NULL, in, NULL, NULL);
					
					break;
				}
				
			 }
		 }
    
	}
    CoUninitialize();
    return 0; 
	}


/*

Msvm_ComputerSystem.Name="48833A94-2091-4Cad-abd7-cce6b8365dba"

;$VMList = $oService.ExecQuery("SELECT * FROM Msvm_VirtualSystemManagementService WHERE ElementName='" & $VMName & "'")
;$VMList.ItemIndex(0).DestroyVirtualSystem(1)

;$VMList = $oService.ExecQuery("SELECT * FROM Msvm_ComputerSystem WHERE ElementName='" & $VMName & "'")
;$VM0 = $VMList.ItemIndex(0).Name

;$mgmt = $oService.Get("Msvm_VirtualSystemManagementService")
;MsgBox(0,'',@error)

;managementService = WmiUtilities.GetVirtualMachineManagementService(scope))

;ObjCreateInterface("Msvm_ComputerSystem", "")


;$ShutdownList = $oService.ExecQuery("SELECT * FROM Msvm_ShutdownComponent WHERE SystemName='" & $VM0 & "'")
;$Result = $ShutdownList.ItemIndex(0).InitiateShutdown(True, "Because I said so")

$VMList = $oService.ExecQuery("SELECT * FROM Msvm_ComputerSystem WHERE ElementName='" & $VMName & "'")
$VMList.ItemIndex(0).RequestStateChange(2)
*/


