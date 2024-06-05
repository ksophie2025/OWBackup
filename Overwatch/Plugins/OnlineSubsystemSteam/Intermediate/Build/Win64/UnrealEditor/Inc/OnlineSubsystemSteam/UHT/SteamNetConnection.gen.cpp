// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Classes/SteamNetConnection.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSteamNetConnection() {}
// Cross Module References
	ONLINESUBSYSTEMSTEAM_API UClass* Z_Construct_UClass_USteamNetConnection();
	ONLINESUBSYSTEMSTEAM_API UClass* Z_Construct_UClass_USteamNetConnection_NoRegister();
	ONLINESUBSYSTEMUTILS_API UClass* Z_Construct_UClass_UIpConnection();
	UPackage* Z_Construct_UPackage__Script_OnlineSubsystemSteam();
// End Cross Module References
	void USteamNetConnection::StaticRegisterNativesUSteamNetConnection()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(USteamNetConnection);
	UClass* Z_Construct_UClass_USteamNetConnection_NoRegister()
	{
		return USteamNetConnection::StaticClass();
	}
	struct Z_Construct_UClass_USteamNetConnection_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bIsPassthrough_MetaData[];
#endif
		static void NewProp_bIsPassthrough_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsPassthrough;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USteamNetConnection_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UIpConnection,
		(UObject* (*)())Z_Construct_UPackage__Script_OnlineSubsystemSteam,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNetConnection_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USteamNetConnection_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "SteamNetConnection.h" },
		{ "ModuleRelativePath", "Classes/SteamNetConnection.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Is this net connection passthrough to IpConnection */" },
#endif
		{ "ModuleRelativePath", "Classes/SteamNetConnection.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Is this net connection passthrough to IpConnection" },
#endif
	};
#endif
	void Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough_SetBit(void* Obj)
	{
		((USteamNetConnection*)Obj)->bIsPassthrough = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough = { "bIsPassthrough", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(USteamNetConnection), &Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough_MetaData), Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USteamNetConnection_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USteamNetConnection_Statics::NewProp_bIsPassthrough,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_USteamNetConnection_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USteamNetConnection>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_USteamNetConnection_Statics::ClassParams = {
		&USteamNetConnection::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_USteamNetConnection_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_USteamNetConnection_Statics::PropPointers),
		0,
		0x000000ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNetConnection_Statics::Class_MetaDataParams), Z_Construct_UClass_USteamNetConnection_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNetConnection_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_USteamNetConnection()
	{
		if (!Z_Registration_Info_UClass_USteamNetConnection.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_USteamNetConnection.OuterSingleton, Z_Construct_UClass_USteamNetConnection_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_USteamNetConnection.OuterSingleton;
	}
	template<> ONLINESUBSYSTEMSTEAM_API UClass* StaticClass<USteamNetConnection>()
	{
		return USteamNetConnection::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(USteamNetConnection);
	USteamNetConnection::~USteamNetConnection() {}
	struct Z_CompiledInDeferFile_FID_Users_admin_Documents_NetTPS_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_admin_Documents_NetTPS_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_USteamNetConnection, USteamNetConnection::StaticClass, TEXT("USteamNetConnection"), &Z_Registration_Info_UClass_USteamNetConnection, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(USteamNetConnection), 3718608802U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_admin_Documents_NetTPS_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_2186920111(TEXT("/Script/OnlineSubsystemSteam"),
		Z_CompiledInDeferFile_FID_Users_admin_Documents_NetTPS_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_admin_Documents_NetTPS_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
