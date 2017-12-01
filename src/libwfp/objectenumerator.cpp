#include "stdafx.h"
#include "objectenumerator.h"
#include <stdexcept>
#include <fwpmu.h>

namespace wfp
{

//static
bool ObjectEnumerator::Sessions(
	std::shared_ptr<FilterEngine> engine,
	std::function<bool(const FWPM_SESSION0&)> callback)
{
	HANDLE enumHandle = INVALID_HANDLE_VALUE;

	auto status = FwpmSessionCreateEnumHandle0(
		(*engine).session(),
		nullptr,
		&enumHandle
	);

	if (ERROR_SUCCESS != status)
	{
		throw new std::runtime_error("Unable to create enumeration context for sessions");
	}

	FWPM_SESSION0** sessions = nullptr;
	UINT32 sessionsReturned = 0;

	status = FwpmSessionEnum0(
		(*engine).session(),
		enumHandle,
		100,
		&sessions,
		&sessionsReturned
	);

	if (ERROR_SUCCESS != status)
	{
		// TODO: Exception safety
		throw std::runtime_error("Unable to enumerate sessions");
	}

	for (UINT32 i = 0; i < sessionsReturned - 1; ++i)
	{
		auto session = *sessions[i];

		if (callback(session) == false)
		{
			return false;
		}
	}

	FwpmFreeMemory0((void**)&sessions);
	FwpmSessionDestroyEnumHandle0((*engine).session(), enumHandle);

	return true;
}

//static
bool ObjectEnumerator::Providers(std::shared_ptr<FilterEngine> engine,
	std::function<bool(const FWPM_PROVIDER0&)> callback)
{
	HANDLE enumHandle = INVALID_HANDLE_VALUE;

	auto status = FwpmProviderCreateEnumHandle0(
		(*engine).session(),
		nullptr,
		&enumHandle
	);

	if (ERROR_SUCCESS != status)
	{
		throw new std::runtime_error("Unable to create enumeration context for providers");
	}

	FWPM_PROVIDER0** providers = nullptr;
	UINT32 providersReturned = 0;

	status = FwpmProviderEnum0(
		(*engine).session(),
		enumHandle,
		100,
		&providers,
		&providersReturned
	);

	if (ERROR_SUCCESS != status)
	{
		// TODO: Exception safety
		throw std::runtime_error("Unable to enumerate providers");
	}

	for (UINT32 i = 0; i < providersReturned - 1; ++i)
	{
		auto provider = *providers[i];

		if (callback(provider) == false)
		{
			return false;
		}
	}

	FwpmFreeMemory0((void**)&providers);
	FwpmProviderDestroyEnumHandle0((*engine).session(), enumHandle);

	return true;
}

} // namespace wfp